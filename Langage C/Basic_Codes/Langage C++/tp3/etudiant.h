#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <map>
#include <string>
#include <vector>


using namespace std; 

class Etudiant{
private:
    int id;
    string nom; 
    map<string,double> notes; 


public:

    //Constructeur Par defaut
    Etudiant();

    //Constructeur avec Parametres
    Etudiant(int id, string nom);

    //Constructeur par Copies 
    Etudiant(const Etudiant& autre);

    //Constructeur par Deplacement
    Etudiant(Etudiant& autre) noexcept;

    //Destructeur
    ~Etudiant();



    //Accesseurs
    int getid() const; 
    string getnom() const;


    //Fonctions
    void afficher() const;

    void ajouterNote(const string &matiere, double note);

    void modifierNote(const string &matiere, double note);

    void modifierNom(const string nom);

    double moyenne();
};




class ClasseEtudiants{
private: 
    vector<Etudiant> Classe; 



public: 
    ClasseEtudiants();
    void ajouteretudiant(int id, string nom);
    void supprimeretudiant(int id);
    void modifieretudiant(int id, string nom);
    void modifiernote(int id, double note, const std::string& matiere);
    void afficherListeEtudiants() const;
    void afficherEtudiant(int id) const;

};


void afficheMenu();
#endif //ETUDIANT_H
