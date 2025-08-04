#include "Publication.h"
#include <iostream>

// Constructeur
Publication::Publication(const std::string& id, const std::string& idAuteur, const std::string& texte)
    : Contenu(id, idAuteur, texte) {}

// Implémentation des méthodes de IAimable
void Publication::aimer(const std::string& idUtilisateur) {
    for (const auto& user : utilisateursQuiAiment) {
        if (user == idUtilisateur) {
            return;
        }
    }
    utilisateursQuiAiment.push_back(idUtilisateur);
}

void Publication::retirerAime(const std::string& idUtilisateur) {
    for (size_t i = 0; i < utilisateursQuiAiment.size(); ++i) {
        if (utilisateursQuiAiment[i] == idUtilisateur) {
            utilisateursQuiAiment.erase(utilisateursQuiAiment.begin() + i);
            return;
        }
    }
}

int Publication::getNombreAimes() const {
    return utilisateursQuiAiment.size();
}

bool Publication::estAimePar(const std::string& idUtilisateur) const {
    for (const auto& user : utilisateursQuiAiment) {
        if (user == idUtilisateur) {
            return true;
        }
    }
    return false;
}

// Implémentation des méthodes abstraites de Contenu
void Publication::afficher() const {
    std::cout << "Publication (" << getId() << ") de " << getIdAuteur() << " : " << getTexte() << std::endl;
    std::cout << "Aimee par " << getNombreAimes() << " personnes." << std::endl;
    if (!commentaires.empty()) {
        std::cout << "Commentaires:" << std::endl;
        for (const auto& commentaire : commentaires) {
            std::cout << "- " << commentaire << std::endl;
        }
    }
}

std::string Publication::getType() const {
    return "Publication";
}

// Gestion des commentaires
void Publication::ajouterCommentaire(const std::string& commentaire) {
    commentaires.push_back(commentaire);
}

std::vector<std::string> Publication::getCommentaires() const {
    return commentaires;
}