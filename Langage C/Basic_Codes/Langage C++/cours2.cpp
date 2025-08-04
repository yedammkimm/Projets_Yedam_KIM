#include <iostream>
#include <vector>

using namespace std;


int main(void){
    
}

/*
Arguments Muets 
    On peut ignorer une argument par 
    (void)argument dans la fonction
        Utilisable dans if 

    On peut initialiser argument en METTANT A LA FIN 
        void test(int y, int x = 10)


Overloading of fonctions 
    Par rapport aux types des arguments, c++ utilise la fonction correspondant 

    ex:
        int somme(int x , int y)

        double somme(int x, int y) 

Fonction Lambda 
    int x = 10; 
    int y = 5;

    auto addition = [&x](){
        return x + 5; 
    };

    auto addition = [=](){
        return x + y; 
    };

     auto addition = [&x](int g){
        return x + g; 
    };

    auto addition = [=](int g){
        (void) y;
        return x + g; 
    };


Reference 

On peut manipuler reference, 
    Il est comme le pointeur, mais pas exactement 
    En mettant &, on peut directement acceder a la variable et AUSSI MODIFIER comme pointeur 
        Mais JE pense on peut pas utiliser pour les tableaux et memoire dynamique 
    
    int x = 10; 
    int &ref = x; 
    cout << "la valeur de x et ref sont" << x << ref << endl; 
    ref = 20; 
    cout << "Jai change le ref a 20 donc x doit etre 20 qui vant " << x << endl; 

    DECLARATION ET INITIALISATION EN MEME TEMPS 

    ❑ Une référence est un alias d’une variable existante. 
    ❑ Une référence est généralement stockée comme un
    pointeur constant en interne. Pas de mémoire
    supplémentaire utilisée.
    ❑ Dans le cas d’un tableau, une référence ne peut pas
    être utilisée pour stocker plusieurs adresses.


    ❑ Un pointeur est une variable qui stocke une adresse
    mémoire. Il est peut être null, réassigné et utilisé
    pour manipuler la mémoire dynamiquement.
    ❑ Un pointeur utilise un espace mémoire
    supplémentaire pour stocker l’adresse.
    ❑ Un pointeur peut être utilisé pour itérer sur un
    tableau.

    Une fonction peut retourner une référence, cela permet d’accéder et de modifier directement un élément interne d’un objet (par exemple un élément d’un vecteur)

    Si on a demande le pointeur, 
        g++ dectecte automatiquement que cest une addresse qui est besoins pour pointeur 
        sinon, il dectecte comme une reference 
    
    

vector 

    name_vector.at(index) = Process access "at" the index number 
    name_vector.size()

    vector<int> v = {10,20,30};
    obtenirElement(v,1) = 1919; 

    for(int &val: v){
        cout << val << endl; 
    }

    for (int &n: v){
        n *= 2;
    }

    for(int &val: v){
        cout << val << endl; 
    }

    cout << v.at(0) << endl; 

    return 0; 

    vector.push_back();
        We add at the end of the vector

R - Value: 

les références && permettent d’optimiser la gestion de la mémoire en évitant des copies inutiles.

void afficher(string &&message){
    cout << "Message: " << message << endl; 
 }

int main(void){

    afficher("R-Value");

}


La fonction de la bibliothèque standard std::move() permet la conversion d’un objet en r-value, permettant ainsi son déplacement
apres move, la variable orignale est efface 

ex: 
    vector<int> v1 = {1,2,3};

    vector<int> v2 = move(v1);

    cout << v1.size() << endl; 
    cout << v2.size() << endl;


Il est intéressant d’utiliser move() lorsque:
❑ Un objet temporaire est utilisé (std::move(obj))
❑ On veut éviter une copie inutile (vector, string, map…)
❑ On fait un retour d’objet dans une fonction


Ne pas utiliser move() dans les cas suivants:
❑ Sur une variable encore utilisée
❑ Si on souhaite accéder à un objet après move()



Structure
    Meme comme C
    typedef ou using 
        using Voiture = struct Personne;
        typedef struct Personne Voiture;
    

On peut utiliser une constante référence pour passer une structure en argument d’une fonction, cela évite une copie et améliore la performance

void afficher(const Voiture &p){
    cout << "NOM: " << p.nom << ", Age : " << p.age << endl; 
}
int main(void){
    Personne p{'c',99};
    afficher(p);
}


🚀 Conclusion
for (auto livre : bibl)	    Copies each element	            Slower	            Unsafe (returns pointer to temporary)
for (auto& livre : bibl)	Uses references (no copy)	    Faster	            Safe
for (const auto& livre : bibl)	     No copy read-only	    Fast	            Safe (if return is needed)


👉 Always use auto& in loops when modifying elements or returning pointers to them.
👉 Use const auto& for read-only access to avoid unnecessary copies. 🚀



Revoir TP2
    Creer Tableau
    Trier
    Supprimer vector
*/