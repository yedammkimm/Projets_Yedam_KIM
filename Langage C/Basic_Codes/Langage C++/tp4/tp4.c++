#include "tp4.h"
#include <iostream>

template <typename T>

Colonne<T>::Colonne(): nom(""){
    cout << "Constructeur Par Defaut" << endl; 
}

template <typename T>
Colonne<T>::Colonne(const string& nom): nom(nom) {
    cout << "Constructeur avec Parametres" << endl; 
}

template <typename T>
Colonne<T>::Colonne(const Colonne& autre): nom(autre.nom), valeurs(autre.valeurs){
    cout << "Constructeur par Copie" << endl;
}

template <typename T>
void Colonne<T>::add(T valeur){
    valeurs.push_back(valeur);
}

template <typename T>
size_t Colonne<T>::taille() const{
    return valeurs.size();
}


template <typename T>
T& Colonne<T>::operator[](size_t i){
    return valeurs[i];
}

template <typename T>
const T& Colonne<T>::operator[](size_t i) const{
    return valeurs[i];
}

template <typename T>
void afficher(const Colonne<T>& col){
    for (T val: col.valeurs){
        cout << val << endl; 
    }
}

template <typename T>
void Colonne<T>::vider(){
    valeurs.clear();
}

template <typename T>
const string Colonne<T>::getNom() const{
    return nom;
}

template <typename T>
Colonne<T>::~Colonne(){
    cout << "Destructeur" << endl; 
}


DataFrame::DataFrame() {
    std::cout << "DataFrame créé avec succès !" << std::endl;
}

template <typename T>
bool DataFrame::colonneExiste(const std::vector<Colonne<T>>& colonnes, const std::string& nom) const {
    for (const auto& col : colonnes) {  // Added `const auto&`
        if (col.getNom() == nom) {  
            return true;
        }
    }
    return false;
}


template <typename T>
bool DataFrame::supprimerColonne(std::vector<Colonne<T>>& colonnes, const std::string& nom) {
    for (auto it = colonnes.begin(); it != colonnes.end(); ++it) {
        if (it->getNom() == nom) {  
            colonnes.erase(it);
            return true;  
        }
    }
    return false; 
}



void DataFrame::ajouterColonne(const std::string& nom, int) {
    if (colonneExiste(colonnesInt, nom)) {
        std::cerr << "Erreur : Une colonne avec le nom \"" << nom << "\" existe déjà dans les entiers." << std::endl;
        return;
    }
    colonnesInt.emplace_back(nom);
}


void DataFrame::ajouterColonne(const std::string& nom, double) {
    if (colonneExiste(colonnesDouble, nom)) {
        std::cerr << "Erreur : Une colonne avec le nom \"" << nom << "\" existe déjà dans les réels." << std::endl;
        return;
    }
    colonnesDouble.emplace_back(nom);
}

void DataFrame::ajouterColonne(const std::string& nom, const std::string&) {
    if (colonneExiste(colonnesString, nom)) {
        std::cerr << "Erreur : Une colonne avec le nom \"" << nom << "\" existe déjà dans les chaînes." << std::endl;
        return;
    }
    colonnesString.emplace_back(nom);
}


void DataFrame::supprimerColonne(const std::string& nom) {
    if (supprimerColonne(colonnesInt, nom) ||
        supprimerColonne(colonnesDouble, nom) ||
        supprimerColonne(colonnesString, nom)) {
        std::cout << "Colonne \"" << nom << "\" supprimée avec succès." << std::endl;
    } else {
        std::cerr << "Erreur : Aucune colonne nommée \"" << nom << "\" trouvée." << std::endl;
    }
}


void DataFrame::afficher_data() const {
    std::cout << "\n=== Affichage du DataFrame ===\n";

    std::cout << "Colonnes Int :" << std::endl;
    for (const auto& col : colonnesInt) {
        afficher(col);
    }

    std::cout << "Colonnes Double :" << std::endl;
    for (const auto& col : colonnesDouble) {
        afficher(col);
    }

    std::cout << "Colonnes String :" << std::endl;
    for (const auto& col : colonnesString) {
        afficher(col);
    }
} 



size_t DataFrame::nombreColonnes() const {
    return colonnesInt.size() + colonnesDouble.size() + colonnesString.size();
}


size_t DataFrame::nombreLignes() const {
    size_t maxLignes = 0;

    for (const auto& col : colonnesInt) {
        if (col.taille() > maxLignes) maxLignes = col.taille();
    }

    for (const auto& col : colonnesDouble) {
        if (col.taille() > maxLignes) maxLignes = col.taille();
    }

    for (const auto& col : colonnesString) {
        if (col.taille() > maxLignes) maxLignes = col.taille();
    }

    return maxLignes;
}


int main() {
    Colonne<int> col1("Entiers");

    col1.add(10);
    col1.add(20);
    col1.add(30);

    std::cout << "Colonne initiale : " << std::endl;
    afficher(col1);

    std::cout << "Taille de la colonne : " << col1.taille() << std::endl;

    std::cout << "Valeur à l'index 1 avant modification : " << col1[1] << std::endl;
    col1[1] = 50;  // Modification d'un élément
    std::cout << "Valeur à l'index 1 après modification : " << col1[1] << std::endl;

    Colonne<int> col2(col1);
    std::cout << "Colonne copiée : " << std::endl;
    afficher(col2);

    col1.vider();
    std::cout << "Colonne après vidage : " << std::endl;
    afficher(col1);

    DataFrame df;

    // Ajouter des colonnes de différents types
    df.ajouterColonne("Entiers", 0);
    df.ajouterColonne("Réels", 0.0);
    df.ajouterColonne("Noms", std::string(""));

    // Afficher le nombre de colonnes et de lignes
    std::cout << "Nombre de colonnes : " << df.nombreColonnes() << std::endl;
    std::cout << "Nombre de lignes (max) : " << df.nombreLignes() << std::endl;

    return 0;
}