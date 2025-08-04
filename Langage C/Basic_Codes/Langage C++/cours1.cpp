#include <iostream>
#include <vector>

namespace var_ex {
    auto x = "Nicolas est nul"; 
}

using namespace std; 

int main(){
    /*
    string y; 
    cout << "Pourquoi Nicolas est nul? " << endl;
    cin >> y; 
    cout << "NON, YOU ARE WRONG ITS BECAUSE " << var_ex::x << endl;
    */

   int x,y,res; 

   /*
   cout << "Enter 2 numbers :" << endl; 
   cin >> x; 
   cin >> y; 
   res = x < y ? y : x ; 
   cout << "The maximum of " << x << " et " << y << " est " << res << endl; 
   cout << "NOW ADD 5 TO THE MAXIMUM"<< endl; 
   auto compteur = 0; 
   do{
    res += 1;
    compteur++;
   }while(compteur < 5);

   cout << "We added " << (compteur) << " to the maximum which is " << res << endl; 
   

   vector<int> valeurs = {'a','b','c','d','e'};
   for (char val: valeurs){
    cout << val << endl; 
   }

   
  
  cout << "Enter x = ?" << endl; 
   cin >> x; 
   int* p = &x; 

   cout << *p << endl;

   int taille; 
   cin >> taille; 
   int* tableaudynamique = new int[taille];
   for( int i = 0; i < taille; i++){
    tableaudynamique[i] = i;
    cout << tableaudynamique[i] << " ";
   }

   delete tableaudynamique;

  int** maille = new int*[taille];


*/
   return 0; 
}

//cout = printf, cin = scanf, dans libraire std, 
//cout << "CONTEXT" << variable << endl = \n
//cin >> x;

/*Namespace Espace de Noms 
    On peut utiliser pour variables ou pour fonctions
    On peut declarer par nous meme ou utiliser libraire defini avant 
    ex:  var_ex, std 
*/


/*La boucle « do-while » : similaire à « while », mais la condition est vérifiée après l’exécution du bloc, donc
le code s’exécute au moins une fois
*/




/*Tableau Dynamique 
malloc = "new type"

free = "delete"


dans STD: 
    Vector: Tableau Dynamique
    deque: File a double extremite 
    list: Liste chainee doublement liee
*/




