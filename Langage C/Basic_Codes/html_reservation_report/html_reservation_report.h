#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//date has 3 parts: Year, Month, and day
typedef struct date
{
    int year; 
    int month; 
    int day;
}date;



typedef struct reservation{
    char* name; 
    date  arrival; 
    date departure;
    char* reservation_type;   
}reservation; 

int compare(reservation reser1,reservation reser2);
void bubble_sort(reservation* reser);
int calcul_duration(reservation reser);
int price_2_pay(reservation reser);
void html_pour_une_reservation(reservation reser);
void html_tableau_reservations(reservation* reser);
void html_file(reservation reser);
void comptes_html_file(reservation* reser);