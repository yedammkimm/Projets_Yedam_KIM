#include "Contenu.h"
#include <iostream>

// Constructeur
Contenu::Contenu(const std::string& id, const std::string& idAuteur, const std::string& texte)
    : id(id), idAuteur(idAuteur), texte(texte) {}

// Getters
std::string Contenu::getId() const {
    return id;
}

std::string Contenu::getIdAuteur() const {
    return idAuteur;
}

std::string Contenu::getTexte() const {
    return texte;
}

// Setter
void Contenu::setTexte(const std::string& nouveauTexte) {
    texte = nouveauTexte;
}

// Ajouter un mot-clé
void Contenu::ajouterMotCle(const std::string& motCle) {
    motsCles.push_back(motCle);
}

// Destructeur virtuel
Contenu::~Contenu() {}
