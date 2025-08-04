
#include <iostream>


using namespace std; 

// Exo1 
int calcul_romain(int mois){
    switch (mois){
    case 1: return 11; break; 
    case 2: return 12; break; 
    case 3: return 1; break; 
    case 4: return 2; break; 
    case 5: return 3; break; 
    case 6: return 4; break; 
    case 7: return 5; break; 
    case 8: return 6; break; 
    case 9: return 7; break; 
    case 10: return 8; break; 
    case 11: return 9; break; 
    case 12: return 10; break; 
    default: return 0; cout << "Month Not between 1 and 12 = Error"<< endl; 
    }
}

void Zeller(int y_annee, int m_mois, int t_jour){
    int m_romain = calcul_romain(m_mois);
    if(m_romain == 11 || m_romain == 12){
        y_annee -= 1; 
    }
    int h = (int)(y_annee/100);
    int j = y_annee - (h*100);
    int w = (t_jour + (int)(2.6*(m_romain - 0.2)) + j + (int)(j/4) + (int)(h/4) - 2*h) % 7;
    cout << t_jour << "/" << m_mois << "/" << y_annee << ": ";  
    switch(w){
        case 0: cout << "Dimanche" << endl; break; 
        case 1: cout << "Lundi" << endl; break; 
        case 2: cout << "Mardi"<< endl; break; 
        case 3: cout << "Mercredi"<< endl; break; 
        case 4: cout << "Jeudi" << endl; break; 
        case 5: cout << "Vendredi"<< endl; break; 
        case 6: cout << "Samedi" << endl; break; 
    }
}

// Exo2 
long ** pytha(int n){
    //initialisation
    long** pyth = new long*[n+1];
    for(int i = 0; i <= n; i++ ){
        pyth[i] = new long[n+1];   
    }
    //Enter Values
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= n; j++){
            pyth[i][j] = i*j;
        }
    }
    return pyth;
}

void affiche_pytha(long** tab, int n){
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= n; j++){
            cout << tab[i][j] << "\t"; // /t pour Affichage de Tabulation
        }
        cout << endl;
    }
}

void delete_tab(long** tab,int n){
    for(int i = 0; i <= n; i++){
        delete[] tab[i];
    }
    delete[] tab;
}

long ** pytha_amelior(int n, int m){
    //initialisation
    long** pyth = new long*[m+1];
    for(int i = 0; i <= m; i++ ){
        pyth[i] = new long[n+1];   
    }
    //Enter Values
    for(int i = 0; i <= m; i++){
        for(int j = 0; j <= n; j++){
            pyth[i][j] = i*j;
        }
    }
    return pyth;
}

void affiche_pytha_amelior(long** tab, int n, int m){
    for(int i = 0; i <= m; i++){
        for(int j = 0; j <= n; j++){
            cout << tab[i][j] << "\t"; // /t pour Affichage de Tabulation
        }
        cout << endl;
    }
    cout << "Le plus grand nombre dans ce tableau vaut " << tab[m][n] << endl; 
}



int main(void){
    int year,month,day; 
    cout << "Enter Day:" << endl;
    cin >> day; 
    cout << "Enter Month:" << endl;
    cin >> month;
    cout << "Enter Year:" << endl;
    cin >> year;  
    Zeller(year,month,day);

    /*
    int n; 
    cout << "Enter A NUMBER" << endl; 
    cin >> n; 
    long** tab = pytha(n);
    affiche_pytha(tab,n);
    delete_tab(tab,n);
   

   int n,m; 
   cout << "Enter first NUMBER" << endl; 
   cin >> n; 
   cout << "Enter second NUMBER" << endl; 
   cin >> m; 
   
   //So we always have n >= m; 
   if(m > n){
    int number = m; 
    m = n; 
    n = number; 
   }

   long** tab = pytha_amelior(n,m);
   affiche_pytha_amelior(tab,n,m);
   delete_tab(tab,m);

    */
}