#ifndef TP4_H
#define TP4_H

#include <vector>
#include <string>


using namespace std;

template <typename T>

class Colonne{

private: 
    vector<T> valeurs; 
    string nom; 

public: 
    //Constructeur Par defaut
    Colonne();

    //Constructeur avec Parametres
    Colonne(const string& nom);

    //Constructeur par Copies 
    Colonne(const Colonne& autre);

    void add(T valeur);

    size_t taille() const;

    T& operator[](size_t i);

    const T& operator[](size_t i) const;

    template <typename S>
    friend void afficher(const Colonne<S>& col); 

    void vider();

    const string getNom() const;

    //Destructeur
    ~Colonne();
};


class DataFrame{
private: 
    vector<Colonne<int>> colonnesInt;
    vector<Colonne<double>> colonnesDouble;
    vector<Colonne<string>> colonnesString;
    template <typename T>
    bool colonneExiste(const std::vector<Colonne<T>>& colonnes, const std::string& nom) const;
    template <typename T>
    bool supprimerColonne(std::vector<Colonne<T>>& colonnes, const std::string& nom);

public: 

    DataFrame();
    void ajouterColonne(const std::string& nom, int);
    void ajouterColonne(const std::string& nom, double);
    void ajouterColonne(const std::string& nom, const std::string&);
    void supprimerColonne(const std::string& nom);
    size_t nombreColonnes() const;
    size_t nombreLignes() const;
    void afficher_data() const;
};




#endif //TP4_H