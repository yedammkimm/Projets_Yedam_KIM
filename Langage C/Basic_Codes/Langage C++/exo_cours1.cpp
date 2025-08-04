/*1. Ecrire une fonction qui calcule le produit de 2 entiers. Les seules opérations qui peuvent
etre utiisées sont la somme et la difference. */

#include <iostream>


using namespace std; 

int exo1(int x, int y){
    int res = 0; 
    for(int i = 0; i < y ; i++){
        res += x;
    }
    return res; 
}

//exo2

/*
int main(int argc, char * argv[]){
    if (argc < 3){
        cout << "PUT MORE NUMEBERS" << endl; 
    }else{
        int sum = 0; 
        for(int i = 0; i < argc; i++){
            sum += atoi(argv[i]);
        }
        cout << sum << endl; 
    }
}



int main (int argc, char* argv[]){
    const int con = 10; 
    int tab[con]; 
    int min, max; 
    cout << "PUT 10 numbers" << endl;
    cin >> tab[0];
    max = tab[0];
    min = tab[0];

    for(int i = 1; i < con; i++){
        cin >> tab[i];
        if(tab[i]< min){
            min = tab[i];
        }
        if(tab[i] > max){
            max = tab[i];
        }
    }

    cout << "max est " << max<< endl; 
    cout << "min est " << min <<endl;

}


//avec pointeur
int main (int argc, char* argv[]){
    const int con = 10; 
    int tab[con]; 
    int* min;
    int* max; 
    cout << "PUT 10 numbers" << endl;
    cin >> tab[0];
    max = tab;
    min = tab;

    for(int i = 1; i < con; i++){
        cin >> tab[i];
        if(tab[i]< *min){
            min = &tab[i];
        }
        if(tab[i] > *max){
            max = &tab[i];
        }
    }

    cout << "max est " << *max<< endl; 
    cout << "min est " << *min <<endl;

}
*/

//exo4 

void copy_tab(int* tab1, int* tab2){
    for(int i = 0; i < 10; i++){
        if(tab1[i] < 0){
            tab2[i] = 0;
        }else{
            tab2[i] = tab1[i];
        }
    }
}

//Exo5 

//cest quoi cMessage


//Exo 6 

int somme(int* tab, int n){
    int sum = 0; 
    for(int i = 0; i < n; i++){
        sum += tab[i];
    }
    return sum;
}

//Exo7 

void min_max_reference(int* tab, int n){
    if(tab == NULL){
        cout << "Non Existant Tableau";
    }else{
        int &min = tab[0];
        int &max = tab[0];
        for(int i = 0; i < n; i++){
            if(tab[i] < min){
                min = tab[i];
            }
            if(tab[i] > max){
                max = tab[i];
            }
        }
        cout << "Max: " << max << endl; 
        cout << "Min: " << min << endl; 
    }
}


void min_max_pointeur(int* tab, int n){
    if(tab == NULL){
        cout << "Non Existant Tableau";
    }else{
        int* min = tab;
        int* max = tab;
        for(int i = 0; i < n; i++){
            if(tab[i] < *min){
                min = &tab[i];
            }
            if(tab[i] > *max){
                max = &tab[i];
            }
        }
        cout << "Max: " << *max << endl; 
        cout << "Min: " << *min << endl; 
    }
}

//Exo 8 

int* double_tab(int* tab,int n){
    int* tab_2 = new int[n];
    for(int i = 0; i < n; i++){
        tab_2[i] = tab[i]*2;
    }
    return tab_2;
}

void affiche_tab(int* tab, int n){
    for(int i = 0; i < n; i++){
        cout << tab[i] << "\t";
    }
}


int main(void){
    int n; 
    cout << "Enter the dimension of the table" << endl; 
    cin >> n; 

    int* your_tab = new int[n];

    cout << "Now, Enter the Values" << endl;
    for(int i = 0; i < n; i++){
        cin >> your_tab[i];
    }
    cout << "Table Completed";

    int* my_tab = double_tab(your_tab,n);

    cout << "Your Table"; 

    affiche_tab(your_tab,n);

    cout << "My Table";

    affiche_tab(my_tab,n);

    delete[] your_tab;
    delete[] my_tab;
}







