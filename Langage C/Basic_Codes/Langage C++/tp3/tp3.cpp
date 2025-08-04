#include "etudiant.h"
#include <iostream>


Etudiant::Etudiant(): nom("INCONNU"), id(-1){
    cout << "Constructeur Par Defaut" << endl; 
}


Etudiant::Etudiant(int id_ex, string nom_ex): nom(nom_ex), id(id_ex){
    cout << "Constructeur avec Parametres" << endl; 
}

Etudiant::Etudiant(const Etudiant& autre): nom(autre.nom), id(autre.id), notes(autre.notes){
    cout << "Constructeur par Copies" << endl; 
}


Etudiant::Etudiant(Etudiant& autre) noexcept : id(autre.id), nom(autre.nom),notes(move(autre.notes)){
    cout << "Constructeur par Deplacement" << endl; 
}

Etudiant::~Etudiant(){
    cout << "Destructeur" << endl; 
}


//Accesseurs
int Etudiant::getid() const{
    return id;
}
string Etudiant::getnom() const{
    return nom;
}


void Etudiant::afficher() const{
    cout << "Le nom de l'Etudiant est " << nom << "avec ID: " << id << endl;
}

void Etudiant::ajouterNote(const string& matiere, double note){
    if(notes.find(matiere) == notes.end()){  //if matiere doesnt exist in the map
        notes.insert(make_pair(matiere,note));
    }else{                                     //if matiere does exist in the map
        cout << "Erreur, Matiere deja existe"<< endl; 
    }
}

void Etudiant::modifierNom(const string nom_ex){
    nom = nom_ex;
}


void Etudiant::modifierNote(const string& matiere, double note){
    auto it = notes.find(matiere);
    if(it == notes.end()){         //if matiere doesnt exist in the map
        cout << "Erreur, Matiere n'existe pas"<< endl; 
    }else{
        it->second = note;
    }
}

double Etudiant::moyenne(){
    double moyen = 0; 
    for(auto it = notes.begin(); it != notes.end(); ++it){
        moyen += it->second;
    }
    return moyen;
}


ClasseEtudiants::ClasseEtudiants(){

}

void afficheMenu(){
    cout << endl << "Menu:" << endl;
    cout << "1. Ajouter un Etudiant"<< endl;
    cout << "2. Supprimer un Etudiant" << endl;
    cout << "3. Modifier les informations d'un Etudiant"<< endl;
    cout << "4. Saisir ou Modifier une note" << endl;
    cout << "5. Afficher la liste des Etudiants" << endl;
    cout << "6. Afficher les informations d'un Etudiants" << endl;
    cout << "7. Quitter" << endl;
    cout << "Choisissez une option: ";
}


void ClasseEtudiants::ajouteretudiant(int id, string nom){
    for(const auto& etudiant: Classe){
        if(etudiant.getid() == id){ 
            cout << "ID n'est pas Unique" << endl;
            return;
            }
        }
    Classe.push_back(Etudiant(id,nom));
}


void ClasseEtudiants::supprimeretudiant(int id) {
    for (auto it = Classe.begin(); it != Classe.end(); ++it) { 
        if (it->getid() == id) {  
            Classe.erase(it);  
            return;   
        }
    }
    cout << "Etudiant not found" << std::endl;
}

void ClasseEtudiants::modifieretudiant(int id, string nom){
    for(auto& etudiant: Classe){
        if(etudiant.getid() == id){ 
            etudiant.modifierNom(nom);
            return;
            }
        }
    cout << "Etudiant not found" << endl;
}


void ClasseEtudiants::modifiernote(int id, double note, const std::string& matiere) {
    for (auto& etudiant : Classe) {  
        if (etudiant.getid() == id) {  
            etudiant.modifierNote(matiere, note);
            etudiant.ajouterNote(matiere, note);
            std::cout << "Note ajoutée ou mise à jour !" << std::endl;
            return;
        }
    }
    std::cout << "Erreur: Étudiant non trouvé !" << std::endl;
}

void ClasseEtudiants::afficherListeEtudiants() const {
    if (Classe.empty()) {
        cout << "Aucun étudiant" << endl;
        return;
    }
    cout << "Liste des étudiants :" << endl;
    for (const auto& etudiant : Classe) {
        etudiant.afficher();  
    }
}

void ClasseEtudiants::afficherEtudiant(int id) const {
    for (const auto& etudiant : Classe) {
        if (etudiant.getid() == id) {
            etudiant.afficher();
            return;
        }
    }
}




int main(void){
    ClasseEtudiants classe;
    int option;
    int id; 
    string nom; 
    string matiere; 
    double note; 

    do {
        afficheMenu();
        cout << "Choisissez une option: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1: // Ajouter un Etudiant
                cout << "ID de l'Etudiant : ";
                cin >> id; 
                cout << "Nom de l'Etudiant : ";
                cin.ignore();
                getline(cin, nom);
                classe.ajouteretudiant(id, nom);
                break;
            
            case 2: // Supprimer un Etudiant
                cout << "ID de l'Etudiant à supprimer : ";
                cin >> id; 
                classe.supprimeretudiant(id);
                break;
            
            case 3: // Modifier le nom d'un Etudiant
                cout << "ID de l'Etudiant à modifier : ";
                cin >> id; 
                cout << "Son Nouveau Nom : ";
                cin.ignore();
                getline(cin, nom);
                classe.modifieretudiant(id, nom);
                break;
            
            case 4: // Modifier/Ajouter une note
                cout << "ID de l'Etudiant : ";
                cin >> id; 
                cout << "Matière : ";
                cin.ignore();
                getline(cin, matiere);
                cout << "Note : ";
                cin >> note; 
                classe.modifiernote(id, note, matiere);
                break;
            
            case 5: // Afficher tous les étudiants
                classe.afficherListeEtudiants();
                break;
            
            case 6: // Afficher un étudiant spécifique
                cout << "ID de l'Etudiant à afficher : ";
                cin >> id;
                classe.afficherEtudiant(id);
                break;
            
            case 7: // Quitter
                cout << "Fermeture du programme" << endl;
                break;
            
            default:
                cout << "Option invalid" << endl;
                break;
        }
    } while (option != 7);

    return 0;
}
