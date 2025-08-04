#include "Utilisateur.h"
#include <iostream>

// Constructeur
Utilisateur::Utilisateur(const std::string& id, const std::string& nom)
    : id(id), nom(nom) {}

// Destructeur
Utilisateur::~Utilisateur() {
    for (Contenu* contenu : contenusCrees) {
        delete contenu;
    }
}

// Getters
std::string Utilisateur::getId() const {
    return id;
}

std::string Utilisateur::getNom() const {
    return nom;
}

// Gestion des abonnements
void Utilisateur::abonner(const std::string& idUtilisateur) {
    for (const auto& abonne : abonnements) {
        if (abonne == idUtilisateur) {
            return;
        }
    }
    abonnements.push_back(idUtilisateur);
}

void Utilisateur::desabonner(const std::string& idUtilisateur) {
    for (size_t i = 0; i < abonnements.size(); ++i) {
        if (abonnements[i] == idUtilisateur) {
            abonnements.erase(abonnements.begin() + i);
            return;
        }
    }
}

bool Utilisateur::estAbonneA(const std::string& idUtilisateur) const {
    for (const auto& abonne : abonnements) {
        if (abonne == idUtilisateur) {
            return true;
        }
    }
    return false;
}

// Création de contenu
void Utilisateur::creerPublication(const std::string& texte) {
    contenusCrees.push_back(new Publication("contenu" + std::to_string(contenusCrees.size() + 1), id, texte));
}

void Utilisateur::creerMedia(const std::string& texte, const std::string& url, const std::string& typeMedia) {
    contenusCrees.push_back(new Media("contenu" + std::to_string(contenusCrees.size() + 1), id, texte, url, typeMedia));
}

// Récupération des contenus créés
std::vector<Contenu*> Utilisateur::getContenus() const {
    return contenusCrees;
}

// Affichage du profil
void Utilisateur::afficherProfil() const {
    std::cout << "Utilisateur: " << nom << " (" << id << ")" << std::endl;
    std::cout << "Abonnements: " << abonnements.size() << std::endl;
    std::cout << "Contenus créés: " << contenusCrees.size() << std::endl;
}