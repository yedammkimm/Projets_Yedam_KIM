from flask import Flask, jsonify, request, render_template_string, send_from_directory,render_template, redirect, url_for
import sqlite3
from datetime import datetime
import requests
from flask_cors import CORS

app = Flask(__name__,template_folder= 'templates')
CORS(app)


# Fonction pour se connecter à la base de données
def get_db_connection():
    conn = sqlite3.connect('logement.db')
    conn.row_factory = sqlite3.Row
    return conn

@app.route('/')
def index():
    conn = get_db_connection()
    rooms = conn.execute('SELECT * FROM Piece').fetchall()

    room_states = []
    scale_factor = 100  # Définir un facteur de mise à l'échelle pour les coordonnées
    current_x = 50  # Coordonnée de départ pour x
    current_y = 50  # Coordonnée de départ pour y
    default_offset_x = 250  # Espace horizontal minimum entre les pièces
    default_offset_y = 200  # Espace vertical minimum si on change de ligne
    positions_occupied = []  # Pour garder une trace des positions occupées

    # Récupérer la liste des noms des pièces
    room_names = [room['nom'] for room in rooms]

    print("Rooms fetched from the database:")
    for room in rooms:
        print(f"Nom: {room['nom']}, Coord_x: {room['coord_x']}, Coord_y: {room['coord_y']}, Coord_z: {room['coord_z']}")

    for room in rooms:
        room_name = room['nom']
        room_id = room['piece_id']

        # Appliquer un facteur d'échelle aux coordonnées
        coord_x = (room['coord_x'] * scale_factor) if room['coord_x'] is not None else current_x
        coord_y = (room['coord_y'] * scale_factor) if room['coord_y'] is not None else current_y
        coord_z = room['coord_z'] if room['coord_z'] is not None else 0

        # Vérifier s'il y a une superposition potentielle
        while (coord_x, coord_y, coord_z) in positions_occupied:
            # Si les coordonnées sont déjà utilisées, on décale la position
            coord_x += default_offset_x
            if coord_x > 800:  # Si x dépasse la largeur, on passe à la ligne suivante
                coord_x = 50
                coord_y += default_offset_y

        # Ajouter les coordonnées actuelles aux positions occupées
        positions_occupied.append((coord_x, coord_y, coord_z))

        # Ajouter les informations de la pièce
        room_states.append({
            'piece_id': room_id,
            'nom': room_name,
            'coord_x': coord_x,
            'coord_y': coord_y,
            'coord_z': coord_z,
            'capteurs': 0  # Supposons qu'il n'y ait pas de capteurs pour le moment
        })

    conn.close()
 
    return render_template('partie3.html', rooms=room_states, room_names=room_names,capteur_states = capteur_states)

    
# Route pour récupérer et afficher un graphique des factures
@app.route('/factures_p3', methods=['GET'])
def factures_p3():
    conn = get_db_connection()
    factures = conn.execute('SELECT type_fac, SUM(montant) as montant_total FROM Facture GROUP BY type_fac').fetchall()
    conn.close()

    # Vérifier si des données sont présentes
    if not factures:
        return jsonify([])

    return jsonify([{'type_fac': facture['type_fac'], 'montant_total': facture['montant_total']} for facture in factures])


# Route pour récupérer et afficher un graphique des factures
@app.route('/factures', methods=['GET'])
def factures_chart():
    conn = get_db_connection()
    factures = conn.execute('SELECT type_fac, SUM(montant) as montant_total FROM Facture GROUP BY type_fac').fetchall()
    conn.close()

    # Vérifier si des données sont présentes
    if not factures:
        return "<h1>Aucune donnée disponible pour les factures.</h1>"

    # Préparer les données pour le graphique
    data_chart = [["Type de facture", "Montant"]]
    for facture in factures:
        data_chart.append([facture["type_fac"], facture["montant_total"]])

    # Modèle HTML pour la page du graphique
    html_template = '''
    <!DOCTYPE html>
    <html>
      <head>
        <title>Graphique des Factures</title>
        <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
        <script type="text/javascript">
          google.charts.load('current', {'packages':['corechart']});
          google.charts.setOnLoadCallback(drawChart);

          function drawChart() {
            var data = google.visualization.arrayToDataTable({{ data_chart | tojson }});

            var options = {
              title: 'Répartition des Factures',
              is3D: true
            };

            var chart = new google.visualization.PieChart(document.getElementById('piechart'));

            chart.draw(data, options);
          }
        </script>
      </head>
      <body>
        <h1>Répartition des Montants des Factures</h1>
        <div id="piechart" style="width: 900px; height: 500px;"></div>
      </body>
    </html>
    '''

    return render_template_string(html_template, data_chart=data_chart)

# Route POST pour ajouter une nouvelle facture
@app.route('/factures', methods=['POST'])
def add_facture():
    data = request.get_json()

    # Récupérer les valeurs à partir de la requête JSON
    type_fac = data.get('type_fac')
    montant = data.get('montant')
    logement_id = data.get('logement_id')
    date_facture = data.get('date')

    # Vérifier si 'date' est fourni dans la requête, sinon utiliser la date actuelle
    if not date_facture:
        date_facture = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    # Connexion à la base de données
    conn = get_db_connection()
    conn.execute('''
        INSERT INTO Facture (type_fac, date, montant, logement_id)
        VALUES (?, ?, ?, ?)
    ''', (type_fac, date_facture, montant, logement_id))
    conn.commit()
    conn.close()

    return jsonify({'message': 'Facture ajoutée avec succès!'}), 201

# Route PUT pour mettre à jour une facture
@app.route('/factures/<int:facture_id>', methods=['PUT'])
def update_facture(facture_id):
    data = request.get_json()
    type_fac = data.get('type_fac')
    montant = data.get('montant')
    logement_id = data.get('logement_id')

    conn = get_db_connection()
    conn.execute('''
        UPDATE Facture
        SET type_fac = ?, montant = ?, logement_id = ?
        WHERE facture_id = ?
    ''', (type_fac, montant, logement_id, facture_id))
    conn.commit()
    conn.close()

    return jsonify({'message': 'Facture mise à jour avec succès!'}), 200

# Route DELETE pour supprimer toutes les factures
@app.route('/factures', methods=['DELETE'])
def delete_all_factures():
    conn = get_db_connection()
    conn.execute('DELETE FROM Facture')
    conn.commit()
    conn.close()

    return jsonify({'message': 'Toutes les factures ont été supprimées avec succès!'}), 200



# Route pour récupérer les prévisions météo à 5 jours et les afficher
@app.route('/previsions_meteo', methods=['GET'])
def get_previsions_meteo():
    # Utilisez votre clé API correcte
    api_key = 'e433b8f05f0e13c21887d09c227b23bd'
    city = 'paris'
    url = f'https://api.openweathermap.org/data/2.5/forecast?q={city}&appid={api_key}&units=metric'

    response = requests.get(url)
    if response.status_code == 200:
        meteo_data = response.json()

        # Vérifier si la clé 'list' est présente dans la réponse
        if 'list' not in meteo_data:
            return jsonify({'error': 'Impossible de récupérer les prévisions météo', 'response': meteo_data}), 500

        previsions = []

        # Récupérer les prévisions pour les 5 jours (8 entrées par jour, espacé de 3 heures)
        for i in range(0, len(meteo_data['list']), 8):
            date = meteo_data['list'][i]['dt_txt']
            temperature = meteo_data['list'][i]['main']['temp']
            description = meteo_data['list'][i]['weather'][0]['description']
            previsions.append({'date': date, 'temperature': temperature, 'description': description})

        # Modèle HTML pour afficher les prévisions
        html_template = '''
        <!DOCTYPE html>
        <html>
          <head>
            <title>Prévisions Météo à 5 jours pour {{ city }}</title>
          </head>
          <body>
            <h1>Prévisions Météo à 5 jours pour {{ city }}</h1>
            <ul>
              {% for prev in previsions %}
                <li>{{ prev.date }} - Température : {{ prev.temperature }}°C - {{ prev.description }}</li>
              {% endfor %}
            </ul>
          </body>
        </html>
        '''

        return render_template_string(html_template, previsions=previsions, city=city)
    else:
        # Retourner le code d'erreur et la réponse pour en savoir plus
        return jsonify({'error': 'Impossible de récupérer les prévisions météo', 'status_code': response.status_code, 'response': response.json()}), 500

# Route pour récupérer les données météo et les afficher
@app.route('/meteo', methods=['GET'])
def get_meteo():
    # Obtenir votre clé API OpenWeatherMap
    # Vous pouvez vous inscrire sur https://openweathermap.org/api et obtenir une clé API gratuite
    api_key = 'e433b8f05f0e13c21887d09c227b23bd'  # Remplacer 'YOUR_API_KEY' par la clé obtenue
    city = 'paris'
    url = f'http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric'

    response = requests.get(url)
    if response.status_code == 200:
        meteo_data = response.json()
        temperature = meteo_data['main']['temp']
        weather_description = meteo_data['weather'][0]['description']

        html_template = f'''
        <!DOCTYPE html>
        <html>
          <head>
            <title>Météo à {city}</title>
          </head>
          <body>
            <h1>Météo actuelle à {city}</h1>
            <p>Température : {temperature}°C</p>
            <p>Description : {weather_description}</p>
          </body>
        </html>
        '''
        return html_template
    else:
        return jsonify({'error': 'Impossible de récupérer les données météo'}), 500

# Route pour récupérer les économies réalisées et les afficher
@app.route('/economies', methods=['GET'])
def get_economies():
    conn = get_db_connection()

    # Fetch data from January to December grouped by type_fac
    economies = conn.execute('''
        SELECT strftime('%Y-%m', date) as month, type_fac, SUM(montant) as total_montant
        FROM Facture
        WHERE date BETWEEN '2024-01-01' AND '2024-12-31'
        GROUP BY month, type_fac
        ORDER BY month, type_fac
    ''').fetchall()
    conn.close()

    # Prepare response data for all months
    months = [f"2024-{str(i).zfill(2)}" for i in range(1, 13)]  # ["2024-01", "2024-02", ..., "2024-12"]
    data_by_type_fac = {}

    # Debug: Print raw data fetched from the database
    print("Raw economies data:", economies)

    for economie in economies:
        month = economie['month']
        type_fac = economie['type_fac']
        montant = economie['total_montant']

        # Initialize list for each type_fac
        if type_fac not in data_by_type_fac:
            data_by_type_fac[type_fac] = [0] * len(months)

        # Assign montant to the correct month
        if month in months:
            month_index = months.index(month)
            data_by_type_fac[type_fac][month_index] = montant

    # Format the response
    response = {
        "months": months,
        "data": [{"type_fac": type_fac, "montants": montants} for type_fac, montants in data_by_type_fac.items()]
    }

    return jsonify(response)

# In-memory storage for the state of sensors (for demonstration purposes)
capteur_states = {
    'capteur_temp': 'Actif',
    'capteur_humidite': 'Inactif',
    'capteur_lumiere': 'Inactif',
    'capteur_gaz': 'Inactif',
    'actionneur_lumiere': 'Inactif'
}

# Route to toggle the state of a sensor/actionneur
@app.route('/toggle_capteur', methods=['POST'])
def toggle_capteur():
    capteur_id = request.form.get('capteur_id')

    if capteur_id in capteur_states:
        # Toggle the state
        current_state = capteur_states[capteur_id]
        capteur_states[capteur_id] = 'Actif' if current_state == 'Inactif' else 'Inactif'
    
    return redirect(url_for('index'))
 

# API route to return filtered economies data in JSON format
@app.route('/api/filter_economies', methods=['GET'])
def filter_economies():
    type_fac = request.args.get('type_fac')

    conn = get_db_connection()
    if type_fac and type_fac != "Tous":
        economies = conn.execute('''
            SELECT strftime('%Y-%m', date) as month, SUM(montant) as total_montant
            FROM Facture
            WHERE type_fac = ?
            GROUP BY month
            ORDER BY month
        ''', (type_fac,)).fetchall()
    else:
        economies = conn.execute('''
            SELECT strftime('%Y-%m', date) as month, SUM(montant) as total_montant
            FROM Facture
            GROUP BY month
            ORDER BY month
        ''').fetchall()
    
    conn.close()

    # Format data for JavaScript consumption
    data = {
        "months": [economie['month'] for economie in economies],
        "montants": [economie['total_montant'] for economie in economies]
    }
    return jsonify(data)

# Gestion des Pièces
@app.route('/manage_rooms', methods=['POST', 'PUT', 'DELETE'])
def manage_rooms():
    # Get the action method, either from `_method` or request method directly
    room_action = request.form.get('_method', request.method)

    conn = get_db_connection()

    if room_action == 'POST':
        # Ajouter une nouvelle pièce
        room_name = request.form['roomName']
        coord_x = request.form['coord_x']
        coord_y = request.form['coord_y']
        coord_z = request.form.get('coord_z', 0)  # Par défaut, étage = 0 si non défini

        conn.execute('INSERT INTO Piece (nom, coord_x, coord_y, coord_z, logement_id) VALUES (?, ?, ?, ?, 1)', 
                     (room_name, coord_x, coord_y, coord_z))
        conn.commit()

    elif room_action == 'PUT':
        # Modifier une pièce existante
        room_id = request.form['existingRoomName']
        coord_x = request.form['coord_x']
        coord_y = request.form['coord_y']
        coord_z = request.form.get('coord_z', 0)

        print(f"Modification de la pièce ID: {room_id}, Coordonnées: ({coord_x}, {coord_y}, {coord_z})")

        conn.execute('UPDATE Piece SET coord_x = ?, coord_y = ?, coord_z = ? WHERE piece_id = ?', 
                     (coord_x, coord_y, coord_z, room_id))
        conn.commit()

    elif room_action == 'DELETE':
        # Supprimer une pièce
        room_id = request.form['existingRoomName']

        print(f"Suppression de la pièce ID: {room_id}")

        conn.execute('DELETE FROM Piece WHERE piece_id = ?', (room_id,))
        conn.commit()

    conn.close()
    return redirect(url_for('index'))



# Gestion des Capteurs/Actionneurs
@app.route('/manage_devices', methods=['POST'])
def manage_devices():
    device_type = request.form.get('deviceType')
    room_id = request.form.get('roomSelect')
    conn = get_db_connection()
    conn.execute('INSERT INTO Capteur (reference_commercial, type_id, piece_id) VALUES (?, ?, ?)',
                 (device_type, 1, room_id))  # `type_id` devrait être résolu en fonction du type de capteur.
    conn.commit()
    conn.close()
    return redirect(url_for('index'))

# Définir les Seuils de Consommation et Alertes
@app.route('/set_thresholds', methods=['POST'])
def set_thresholds():
    electricity_threshold = request.form.get('electricityThreshold')
    water_threshold = request.form.get('waterThreshold')
    # Stocker ces valeurs dans une base de données pour des vérifications futures
    return redirect(url_for('index'))

# Configurer les Notifications
@app.route('/update_notifications', methods=['POST'])
def update_notifications():
    notification_email = request.form.get('notificationEmail')
    # Logic to store email for notifications (e.g., store in a configuration table)
    return redirect(url_for('index'))

# Gérer la Sécurité des Pièces
@app.route('/manage_security', methods=['POST'])
def manage_security():
    room_id = request.form.get('securityRoomSelect')
    lock_state = request.form.get('lockState')
    # Logic to lock or unlock a specific room
    return redirect(url_for('index'))

@app.route('/delete_room', methods=['POST'])
def delete_room():
    room_id = request.form['room_id']

    conn = get_db_connection()
    conn.execute('DELETE FROM Piece WHERE piece_id = ?', (room_id,))
    conn.commit()
    conn.close()

    return redirect(url_for('index'))

@app.route('/add_room', methods=['POST'])
def add_room():
    room_name = request.form['room_name']
    coord_x = request.form.get('coord_x', type=int)
    coord_y = request.form.get('coord_y', type=int)
    coord_z = request.form.get('coord_z', type=int) if request.form.get('coord_z') else 0

    conn = get_db_connection()
    conn.execute('INSERT INTO Piece (nom, coord_x, coord_y, coord_z, logement_id) VALUES (?, ?, ?, ?, ?)',
                 (room_name, coord_x, coord_y, coord_z, 1))
    conn.commit()
    conn.close()

    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)