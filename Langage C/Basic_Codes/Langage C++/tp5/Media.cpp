#include "Media.h"
#include <iostream>

// Constructeur
Media::Media(const std::string& id, const std::string& idAuteur, const std::string& texte, 
             const std::string& url, const std::string& typeMedia)
    : Contenu(id, idAuteur, texte), url(url), typeMedia(typeMedia) {}

// Implémentation des méthodes de IAimable
void Media::aimer(const std::string& idUtilisateur) {
    for (const auto& user : utilisateursQuiAiment) {
        if (user == idUtilisateur) {
            return;
        }
    }
    utilisateursQuiAiment.push_back(idUtilisateur);
}

void Media::retirerAime(const std::string& idUtilisateur) {
    for (size_t i = 0; i < utilisateursQuiAiment.size(); ++i) {
        if (utilisateursQuiAiment[i] == idUtilisateur) {
            utilisateursQuiAiment.erase(utilisateursQuiAiment.begin() + i);
            return;
        }
    }
}

int Media::getNombreAimes() const {
    return utilisateursQuiAiment.size();
}

bool Media::estAimePar(const std::string& idUtilisateur) const {
    for (const auto& user : utilisateursQuiAiment) {
        if (user == idUtilisateur) {
            return true;
        }
    }
    return false;
}

// Implémentation des méthodes de IPartageable
void Media::partager(const std::string& idUtilisateur) {
    for (const auto& user : utilisateursQuiPartagent) {
        if (user == idUtilisateur) {
            return;
        }
    }
    utilisateursQuiPartagent.push_back(idUtilisateur);
}

int Media::getNombrePartages() const {
    return utilisateursQuiPartagent.size();
}

// Implémentation des méthodes abstraites de Contenu
void Media::afficher() const {
    std::cout << "Media (" << getId() << ") de " << getIdAuteur() << " : " << getTexte() << std::endl;
    std::cout << "Type: " << getTypeMedia() << " | URL: " << getUrl() << std::endl;
    std::cout << "Aimee par " << getNombreAimes() << " personnes. Partagee " << getNombrePartages() << " fois." << std::endl;
}

std::string Media::getType() const {
    return "Media";
}

// Getters
std::string Media::getUrl() const {
    return url;
}

std::string Media::getTypeMedia() const {
    return typeMedia;
}