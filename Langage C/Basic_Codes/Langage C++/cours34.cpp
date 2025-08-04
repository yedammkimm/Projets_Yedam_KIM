#include <iostream>
#include <string> 

using namespace std; 


//Pour Definir une classe, on a besoins une point vergule a la fin
class Compteur{
private:
    int value;

public: 
    /*Constucteur avec parametres 
    Compteur(int a) : value(a){
        cout << "Constructeur appele pour" << value << endl; 
    }
    */

    Compteur(int a){
        this->value = a;
        cout << "Constructeur avec this pour" << value << endl; 
    }
   
    //Constructeur par defaut
    Compteur(): value(0) {
        cout << "Constructeur par defaut appele pour" << value << endl; 
    }

    Compteur(Compteur&& autre) noexcept : value(autre.value){
        autre.value = 0; 
    }

    Compteur& incrementer(){
        ++this->value; 
        return *this;
    }
    /*
    ++this->value; → c.value = 1
    Returns *this (reference to c).
    */

    // Les fonctions sont appeeles surcharges, car meme nom, meme methodes mais il depend de parametres, nombres de arguments ou soit le type 

    //Constructeur de Copie, que on definir par construire un type de classe par faire copie des choses qui existe deja
    // On peut creer une fonction qui fait le copie en utilisant les surcharges 
    // ou type nametype = type1 

    // Constructeur de Deplacement: On construire par faire transferer les donnees de type qui existe deja
    // On utilise le mot clef noexcept, pour garantir que il ny a pas de exception 
    // par ex: std::move 
    // La fonction std::move convertit l’objet buf1 en une rvalue pour déclencher l’appel du constructeur de déplacement
    // Après déplacement, buf1 reste valide mais ne contient plus de donnée et nécessite une réinitialisation de ses ressources


    //Destructeur, On a besoins de liberer la memoire
    //Dynamique: Delete 
    //local(Sur Pile): la sortie de la fonction 


    //this = Self in python

    //Il est courant de retourner *this pour permettre le chaînage des appels de méthodes. Cela rend le code plus
    //fluide et permet d’appeler plusieurs méthodes sur le même objet en une seule instruction




    // Allocation Statique, 

    /*
     Une méthode statique est une fonction membre d’une classe qui appartient à la classe elle-même plutôt qu’à
    une instance spécifique. Cela signifie qu’elle peut être appelée sans avoir besoin de créer un objet de la
    classe.
     Une méthode statique est déclarée avec le mot-clé stati c dans la classe. Elle ne peut pas accéder aux attributs
    non statiques de la classe, car elle n’est pas liée à une instance spécifique.


    Page 9 Cours 4



    Accesseurs et Mutateurs

        Acceusseurs: Il renvoie les valeurs des attributs privees, Cest une fonction qui renvoie 

            Les accesseurs doivent généralement être marqués comme const, car ils ne modifient pas l’objetLorsque des
            attributs sont volumineux (exemple: std::string), il est préférable de retourner une référence constante (const
            &) pour éviter des copies inutiles

        Mutateurs: Il modifie les valeurs des attributs privees , Cest une fonction qui prend un argument et modifie le

    Friend 
        Une méthode friend (amie) est une fonction qui n’appartient pas à une classe mais qui peut accéder à ses
        membres privés et protégés

        Page 17 Cours 4

        • afficherNom() n’est pas une méthode de Personne
        mais peut accéder à nom grâce à friend
        • friend est déclaré dans la classe mais la définition est
        externe
        • Permet un accès direct aux membres privés sans passer
        par un accesseur


        On peut utiliser deux classes et que definir une fonction d'une classe et l'autre classe declare la fonction comme friend, donc on peut acceder aux membres privees     


        Les méthodes friend sont souvent utilisées pour la surcharge des opérateurs car elles permettent d’accéder aux
        membres privés de la classe sans pour autant faire partie de la classe


        Patrons de Fonction 
        Les patrons de fonction (ou templates de fonction) en C++ permettent d’écrire du code générique, c’est-à-dire du
        code indépendant du type de données manipulé. Cela évite de dupliquer du code pour différents type et améliore
        la réutilisabilité et la maintenabilité. == Template 
            Si on souhaite créer une fonction qui cherche le maximum de 2 entiers ou de 2 floats il sera nécessaire de créer
            deux fonctions, une pour le maximum de deux ints et une seconde pour le maximum de deux floats

        template <typename T>
        T addition (T a, T b){
        return a+b;
        }



        Patrons de Classe 
        Comme pour les templates de fonction, il existe la même notion pour les classes. Les patrons de classe (ou
        templates de classe) permettent de généraliser la définition des classes afin de fonctionner avec plusieurs types de
        données sans dupliquer le code.


        template <typename T>
        class NOMDELACLASSE{
        private: 
            T attribut; 
            
        Public: 
            NOMDELCLASSE(T valeur);
            void methode();
        };


        ON peut aussi accepter plusieurs generiques: 

        template <typename T1, typename T2>
    */   

};


int main(void){
    Compteur c; //renovoie 0 
    c.incrementer().incrementer().incrementer();

    /*
    c.incrementer() returns c → Next .incrementer() is applied on the same object.
    ++this->value; → c.value = 2
    Returns *this.
    
    */
}