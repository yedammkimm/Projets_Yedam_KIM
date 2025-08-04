DROP TABLE IF EXISTS Logement; 
DROP TABLE IF EXISTS Piece; 
DROP TABLE IF EXISTS Capteur; 
DROP TABLE IF EXISTS TypeCapteur; 
DROP TABLE IF EXISTS Mesure; 
DROP TABLE IF EXISTS Facture; 


CREATE TABLE Logement(
    logement_id INTEGER PRIMARY KEY AUTOINCREMENT,
    adresse TEXT NOT NULL, 
    adresse_ip TEXT, 
    numero_telephone TEXT, 
    date_insertion TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Piece ( 
    piece_id INTEGER PRIMARY KEY AUTOINCREMENT, 
    nom TEXT NOT NULL,
    coord_x INTEGER, 
    coord_y INTEGER,
    coord_z INTEGER, 
    logement_id INTEGER,
    FOREIGN KEY (logement_id) REFERENCES Logement(logement_id)
);

CREATE TABLE TypeCapteur(
    type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL, 
    unite_mesure TEXT, 
    Plage_precision TEXT
);

CREATE TABLE Capteur ( 
    capteur_id INTEGER PRIMARY KEY AUTOINCREMENT, 
    reference_commercial TEXT,
    type_id INTEGER,
    piece_id INTEGER,
    port_communication TEXT, 
    date_insertion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(piece_id) REFERENCES Piece(piece_id),
    FOREIGN KEY(type_id) REFERENCES TypeCapteur(type_id)
);

CREATE TABLE Mesure( 
    mesure_id INTEGER PRIMARY KEY AUTOINCREMENT,
    capteur_id INTEGER,
    valeur REAL,
    date_insertion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(capteur_id) REFERENCES Capteur(capteur_id)
);

CREATE TABLE Facture (
    facture_id INTEGER PRIMARY KEY AUTOINCREMENT,
    type_fac TEXT NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    montant REAL,
    valeur_consomme REAL,
    logement_id INTEGER,
    FOREIGN KEY (logement_id) REFERENCES Logement(logement_id)
);

-- Q4
INSERT INTO Logement(adresse,adresse_ip,numero_telephone,date_insertion) VALUES ('123 Avenue de Paris', '127.3.2.1','0123456789',CURRENT_TIMESTAMP);
INSERT INTO Piece (nom, coord_x, coord_y, coord_z,logement_id) VALUES ('Salon', 0, 0, 0, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));
INSERT INTO Piece (nom, coord_x, coord_y, coord_z,logement_id) VALUES ('Cuisine', 1, 0, 0, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));
INSERT INTO Piece (nom, coord_x, coord_y, coord_z,logement_id) VALUES ('Chambre', 0, 1, 0, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));
INSERT INTO Piece (nom, coord_x, coord_y, coord_z,logement_id) VALUES ('Salle de bain', 0, 0, 1, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));


-- Q5
INSERT INTO TypeCapteur (nom, unite_mesure, Plage_precision) VALUES ('Température', '°C', '0-100');
INSERT INTO TypeCapteur (nom, unite_mesure, Plage_precision) VALUES ('Humidité', '%', '0-100');
INSERT INTO TypeCapteur (nom, unite_mesure, Plage_precision) VALUES ('Lumière', 'Lux', '0-10000');
INSERT INTO TypeCapteur (nom, unite_mesure, Plage_precision) VALUES ('Consommation Électrique', 'kWh', '0-1000');


--Q6 

INSERT INTO Capteur (reference_commercial, type_id, piece_id, port_communication, date_insertion)
VALUES ('Capteur_Temp_Salon', (SELECT type_id FROM TypeCapteur WHERE nom = 'Température'), 
        (SELECT piece_id FROM Piece WHERE nom = 'Salon'), 'Port1', CURRENT_TIMESTAMP);

INSERT INTO Capteur (reference_commercial, type_id, piece_id, port_communication, date_insertion)
VALUES ('Capteur_Humid_Cuisine', (SELECT type_id FROM TypeCapteur WHERE nom = 'Humidité'), 
        (SELECT piece_id FROM Piece WHERE nom = 'Cuisine'), 'Port2', CURRENT_TIMESTAMP);


-- Q7
INSERT INTO Mesure (capteur_id, valeur, date_insertion)
VALUES ((SELECT capteur_id FROM Capteur WHERE reference_commercial = 'Capteur_Temp_Salon'), 22.5, CURRENT_TIMESTAMP);

INSERT INTO Mesure (capteur_id, valeur, date_insertion)
VALUES ((SELECT capteur_id FROM Capteur WHERE reference_commercial = 'Capteur_Temp_Salon'), 23.1, CURRENT_TIMESTAMP);

-- Mesures pour le capteur d'humidité dans la Cuisine
INSERT INTO Mesure (capteur_id, valeur, date_insertion)
VALUES ((SELECT capteur_id FROM Capteur WHERE reference_commercial = 'Capteur_Humid_Cuisine'), 45.0, CURRENT_TIMESTAMP);

INSERT INTO Mesure (capteur_id, valeur, date_insertion)
VALUES ((SELECT capteur_id FROM Capteur WHERE reference_commercial = 'Capteur_Humid_Cuisine'), 46.5, CURRENT_TIMESTAMP);


-- Q8
INSERT INTO Facture (type_fac, date, montant, valeur_consomme, logement_id)
VALUES ('Eau', CURRENT_TIMESTAMP, 30.5, 120, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));

INSERT INTO Facture (type_fac, date, montant, valeur_consomme, logement_id)
VALUES ('Électricité', CURRENT_TIMESTAMP, 60.0, 250, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));

INSERT INTO Facture (type_fac, date, montant, valeur_consomme, logement_id)
VALUES ('Déchets', CURRENT_TIMESTAMP, 15.0, 60, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));

INSERT INTO Facture (type_fac, date, montant, valeur_consomme, logement_id)
VALUES ('Gaz', CURRENT_TIMESTAMP, 40.5, 150, (SELECT logement_id FROM Logement WHERE adresse = '123 Avenue de Paris'));
