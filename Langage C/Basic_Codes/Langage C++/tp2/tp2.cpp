#include <iostream>
#include <vector>
#include <string>

using namespace std; 

int factorielle(int n){
    if(n == 1){
        return 1;
    }else{
        return n*(factorielle(n-1));
    }
}

int puissance(int x,int n){
    if (n <= 1){
        return x; 
    }else if (n%2){ // n%2 == 1, si n est impair 
        return x* puissance(x*x,(n-1)/2);
    }else if (!(n%2)){ // si n est pair
        return puissance(x*x,n/2);
    }else{
        cout << "Error";
        return 0;
    }
}


void creerTableau(int taille, int* &tab, int valeurDefaut = 0){ //Si on a pas la valeur de valeurDefaut, on prend 0 sinon la valeurDefaut
    delete[] tab; 
    tab = new int[taille];
    for(int i = 0; i < taille; i++){
        tab[i] = valeurDefaut;   
    }
}

void remplir(int taille, int* &tab){
    cout << "Enter " << taille << " Valeurs: " << endl; 
    for(int i = 0; i < taille; i++){
        cin >> tab[i]; 
    }
}

void sort(int* &tab, int taille){
    for(int i = 1; i < taille; ++i){
        int valeur = move(tab[i]);
        int j = i-1; 
        while(j >= 0 && tab[j] > valeur){
            tab[j+1] = move(tab[j]);
            --j;
        }
        tab[j+1] = move(valeur);
    }
}

void affichage_tab(int taille, int* &tab){
    for (int i = 0; i < taille; ++i) {
    cout << tab[i] << " ";
    }
    cout << endl;
}


typedef struct{
    string titre; 
    string auteur; 
    int annee; 
}Livre; 


void ajouterLivre(vector<Livre>& bibl, const string titre, const string auteur, int annee) {
    bibl.push_back({titre, auteur, annee});
}

void afficherBibliotheque(vector<Livre>& bibl) {
    for (const auto& livre : bibl) {
        cout << "Titre: " << livre.titre << ", Auteur: " << livre.auteur << ", Année: " << livre.annee << endl;
    }
}

void rechercherLivre(const vector<Livre>& bibl, const string& titre) {
    bool trouve = false;
    for (const auto& livre : bibl) {
        if (livre.titre == titre) {
            cout << "Livre trouvé:\n";
            cout << "Titre: " << livre.titre << "\n";
            cout << "Auteur: " << livre.auteur << "\n";
            cout << "Année: " << livre.annee << "\n";
            trouve = true;
            break;
        }
    }
    if (!trouve) {
        cout << "Livre non trouvé.\n";
    }
}

void trierBibliotheque(vector<Livre>& bibl) {
    for (size_t i = 0; i < bibl.size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < bibl.size(); ++j) {
            if (bibl[j].titre < bibl[minIndex].titre) {
                minIndex = j;
            }
        }
        swap(bibl[i], bibl[minIndex]);
    }
}

void supprimerLivre(vector<Livre>& bibl, const string& titre) {
    for (auto it = bibl.begin(); it != bibl.end(); ++it) {
        if (it->titre == titre) {
            bibl.erase(it);
            cout << "Livre supprimé\n";
            return;
        }
    }
    cout << "Livre non trouvé.\n";
}

void afficherMenu() {
    cout << "\nMenu:\n";
    cout << "1. Ajouter un livre\n";
    cout << "2. Afficher la bibliothèque\n";
    cout << "3. Rechercher un livre\n";
    cout << "4. Trier les livres\n";
    cout << "5. Supprimer un livre\n";
    cout << "6. Quitter\n";
}

/* TEST
int main (void){

    int x,n;
    cout << "Enter a number" << endl; 
    cin >> x; 
    cout << "Factiorielle of " << x << " is " << factorielle(x) << endl; 
    cout << "Enter 2 numbers x and n" << endl;
    cin >> x; 
    cin >> n; 
    cout << x << " puissance " << n << " is " << puissance(x,n)<< endl;
    
    int* monTableau;
    int taille = 5;
    
    creerTableau(taille, monTableau,1);
    
    affichage_tab(taille,monTableau);  
    
    remplir(taille,monTableau);

    affichage_tab(taille,monTableau);

    delete[] monTableau;
   
}
*/

int main() {
    vector<Livre> bibliotheque;
    int choix;
    string titre, auteur;
    int annee;
    do {
        afficherMenu();
        cout << "Choisissez une option: ";
        cin >> choix;
        cin.ignore();
        
        switch (choix) {
            case 1:
                cout << "Titre: ";
                getline(cin, titre);
                cout << "Auteur: ";
                getline(cin, auteur);
                cout << "Année: ";
                cin >> annee;
                cin.ignore();
                ajouterLivre(bibliotheque, titre.c_str(), auteur.c_str(), annee);
                break;
            case 2:
                afficherBibliotheque(bibliotheque);
                break;
            case 3:
                cout << "Titre du livre à rechercher: ";
                getline(cin, titre);
                rechercherLivre(bibliotheque,titre);
                break;
            case 4:
                trierBibliotheque(bibliotheque);
                cout << "Bibliothèque triée.\n";
                break;
            case 5:
                cout << "Titre du livre à supprimer: ";
                getline(cin, titre);
                supprimerLivre(bibliotheque, titre);
                break;
            case 6:
                cout << "Quitter le programme.\n";
                break;
            default:
                cout << "Option invalide, veuillez réessayer.\n";
        }
    } while (choix != 6);

    return 0;
}


