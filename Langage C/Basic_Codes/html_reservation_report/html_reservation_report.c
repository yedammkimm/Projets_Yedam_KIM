#include "html_reservation_report.h"


//We compare firstly year, and month and day 
int compare(reservation reser1,reservation reser2){
    return (!(reser1.arrival.year < reser2.arrival.year)||((reser1.arrival.year == reser2.arrival.year)&&(reser1.arrival.month < reser2.arrival.month))||((reser1.arrival.year == reser2.arrival.year)&&(reser1.arrival.month == reser2.arrival.month)&&(reser1.arrival.day < reser2.arrival.day)));
}

// classic bubble sort 
void bubble_sort(reservation* reser){
    bool tri = false;
    while (!tri){
        tri = true; 
        for(int i = 0; i < 4; ++i){
            if((compare(reser[i],reser[i+1]))){
            reservation t = reser[i];
            reser[i] = reser[i+1];
            reser[i+1] = t;
            tri = false;
            }
        }
    }
}

int calcul_duration(reservation reser){
    return ((reser.departure.year - reser.arrival.year)*365 + (reser.departure.month - reser.arrival.month)*30 + (reser.departure.day - reser.arrival.day));

}

//We suppose that type_reservation has name only "Suite" or "Classic Room"
int price_2_pay(reservation reser){
    if(*reser.reservation_type == 'S'){
        return ((calcul_duration(reser)-1)*400);
    }else{
        return ((calcul_duration(reser)-1)*60);
    }
}

void html_pour_une_reservation(reservation reser){
    printf("Client information about reservation\nName of reservation: ");
    for(int i = 0; reser.name[i] != '\0'; i++){
        printf("%c",reser.name[i]);
    }
    printf("\nArrival Date: %d/%d/%d\n",reser.arrival.year,reser.arrival.month,reser.arrival.day);
    printf("Depart Date: %d/%d/%d\n",reser.departure.year,reser.departure.month,reser.departure.day);
    if(*reser.reservation_type == 'S'){
        printf("Reservation Type: Suite\n");
    }else{
        printf("Reservation Type: Classic Room\n");       
    }
}
void html_tableau_reservations(reservation* reser){
    bubble_sort(reser);
    printf("Clients information about reservation \n");
    for(int i = 0; i < 5; i++){
        printf("Name of reservation:"); 
    for(int j = 0; reser[i].name[j] != '\0'; j++){
        printf("%c",reser[i].name[j]);
    }
    printf("\nArrival Date: %d/%d/%d\n",reser[i].arrival.year,reser[i].arrival.month,reser[i].arrival.day);
    printf("Depart Date: %d/%d/%d\n",reser[i].departure.year,reser[i].departure.month,reser[i].departure.day);
    if(*reser[i].reservation_type == 'S'){
        printf("Reservation Type: Suite\n");
    }else{
        printf("Reservation Type: Classic Room\n");       
    }
    }
}

void html_file(reservation reser){
    FILE* f = fopen("devis.html","w");
    fprintf(f,"Client information about reservation<br>Name of reservation: ");
    for(int i = 0; reser.name[i] != '\0'; i++){
        fprintf(f,"%c",reser.name[i]);
    }
    fprintf(f,"<br>Arrival Date: %d/%d/%d<br>",reser.arrival.year,reser.arrival.month,reser.arrival.day);
    fprintf(f,"Depart Date: %d/%d/%d<br>",reser.departure.year,reser.departure.month,reser.departure.day);
    if(*reser.reservation_type == 'S'){
        fprintf(f,"Reservation Type: Suite<br>");
    }else{
        fprintf(f,"Reservation Type: Classic Room<br>");       
    }
    fclose(f);
}

void comptes_html_file(reservation* reser){
    bubble_sort(reser);
    FILE* f = fopen("comptes.html","w");
    fputs("<html>",f);
    fputs("<title>",f);
    fprintf(f,"Clients information about reservation");
    fputs("</title>",f);
    fputs("<head>",f);
    fprintf(f,"Clients information about reservation:<br>");
    fputs("</head>",f);
    for(int i = 0; i < 5; i++){
        fputs("<body>",f);
        fprintf(f,"<br>Name of reservation:"); 
    for(int j = 0; reser[i].name[j] != '\0'; j++){
        fprintf(f,"%c",reser[i].name[j]);
    }
    fprintf(f,"<br>Arrival Date: %d/%d/%d<br>",reser[i].arrival.year,reser[i].arrival.month,reser[i].arrival.day);
    fprintf(f,"Depart Date: %d/%d/%d<br>",reser[i].departure.year,reser[i].departure.month,reser[i].departure.day);
    if(*reser[i].reservation_type == 'S'){
        fprintf(f,"Reservation Type: Suite<br> </body>");
    }else{
        fprintf(f,"Reservation Type: Classic Room<br> </body>");       
    }
        fputs("</body>",f);
    }
    fputs("</html>",f);
    fclose(f);
}

