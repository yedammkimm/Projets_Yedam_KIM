import sqlite3
import random
from datetime import datetime, timedelta

# Ouverture/initialisation de la base de données
conn = sqlite3.connect('logement.db')
conn.row_factory = sqlite3.Row
c = conn.cursor()

# Ajout de plusieurs mesures pour différents capteurs
def ajouter_mesures(capteur_id, nombre_mesures=5):
    for _ in range(nombre_mesures):
        valeur = round(random.uniform(20.0, 30.0), 2)  # Valeurs aléatoires de mesure (ex: température)
        date_insertion = datetime.now() - timedelta(days=random.randint(0, 30))  # Date aléatoire dans les 30 derniers jours
        c.execute('''
            INSERT INTO Mesure (capteur_id, valeur, date_insertion)
            VALUES (?, ?, ?)
        ''', (capteur_id, valeur, date_insertion))

# Ajout de plusieurs factures pour un logement spécifique
def ajouter_factures(logement_id, nombre_factures=4):
    types_facture = ['Eau', 'Électricité', 'Gaz', 'Déchets']
    for type_fac in types_facture:
        montant = round(random.uniform(20.0, 100.0), 2)  # Montant aléatoire
        valeur_consomme = random.randint(50, 500)  # Consommation aléatoire
        date_facture = datetime.now() - timedelta(days=random.randint(0, 365))  # Date aléatoire dans l'année
        c.execute('''
            INSERT INTO Facture (type_fac, date, montant, valeur_consomme, logement_id)
            VALUES (?, ?, ?, ?, ?)
        ''', (type_fac, date_facture, montant, valeur_consomme, logement_id))

# Sélectionner les capteurs et ajouter des mesures pour chacun
c.execute('SELECT capteur_id FROM Capteur')
capteurs = c.fetchall()
for capteur in capteurs:
    ajouter_mesures(capteur['capteur_id'], nombre_mesures=5)  # Ajouter 5 mesures pour chaque capteur

# Sélectionner le logement et ajouter des factures
c.execute('SELECT logement_id FROM Logement')
logements = c.fetchall()
for logement in logements:
    ajouter_factures(logement['logement_id'], nombre_factures=4)  # Ajouter 4 factures pour chaque logement

# Fermeture de la base de données
conn.commit()
conn.close()
