#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <string>
#include <vector>
#include "Contenu.h"
#include "Publication.h"
#include "Media.h"

class Utilisateur {
private:
    std::string id;
    std::string nom;
    std::vector<std::string> abonnements;
    std::vector<Contenu*> contenusCrees;

public:
    // Constructeur
    Utilisateur(const std::string& id, const std::string& nom);

    // Destructeur
    ~Utilisateur();

    // Getters
    std::string getId() const;
    std::string getNom() const;

    // Gestion des abonnements
    void abonner(const std::string& idUtilisateur);
    void desabonner(const std::string& idUtilisateur);
    bool estAbonneA(const std::string& idUtilisateur) const;

    // Création de contenu
    void creerPublication(const std::string& texte);
    void creerMedia(const std::string& texte, const std::string& url, const std::string& typeMedia);

    // Récupération des contenus créés
    std::vector<Contenu*> getContenus() const;

    // Affichage du profil
    void afficherProfil() const;
};

#endif // UTILISATEUR_H