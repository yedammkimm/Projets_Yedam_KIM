#include <stdio.h>

/*
int main(void){
    unsigned long numero_etudiant; 
    char nom[50]; 
    char prenom[50];
    char scan[100];
    fgets(scan, sizeof(scan), stdin);
    sscanf(scan,"%s %s %lu",nom,prenom,&numero_etudiant);
    printf("\n%lu %s %s \n",numero_etudiant,prenom,nom);
}
*/


/*Parametre de fork 
 pid_t fork(void);
 la valeur de retour de fork contient la valeur de PID pour le process ID 
    Ils ont deux valeurs de retours 
        le process actuel est dans le process child on a la valeur de PID de child process 
        si -1, no child process est cree 
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    pid_t p = fork();        // we create the child process of this process and p takes the PID of child process 
    if(p<0){                // if the child process is not created = -1, we send error
        perror("fork fail");
        exit(1);
    }
    printf("Hello world!, process_id(pid) = %d \n",p); //if it is created, we send its ID 
    return 0;
}


