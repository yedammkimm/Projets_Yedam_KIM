#include "html_reservation_report.h"

int main(){
    reservation* res = malloc(sizeof(reservation) * 5); 
    res[0].name = "Aymane"; res[1].name = "Victor"; res[2].name = "Quentin";res[3].name = "Nicolas";res[4].name = "Yedam";
    res[0].arrival.year = 2020;res[1].arrival.year = 2019;res[2].arrival.year = 2018;res[3].arrival.year = 2021;res[4].arrival.year = 2022;
    res[0].arrival.month = 11;res[1].arrival.month  = 10;res[2].arrival.month  = 1;res[3].arrival.month  = 2;res[4].arrival.month  = 6;
    res[0].departure.month = 6;res[1].departure.month = 1;res[2].departure.month  = 7;res[3].departure.month = 9;res[4].departure.month = 10;
    res[0].arrival.day = 26 ;res[1].arrival.day   = 20;res[2].arrival.day  = 19;res[3].arrival.day   = 5;res[4].arrival.day  = 7;
    res[0].departure.day  = 16;res[1].departure.day  = 15;res[2].departure.day   = 14;res[3].departure.day  = 1;res[4].departure.day  = 14;
    res[0].departure.year = 2021;res[1].departure.year = 2020;res[2].departure.year = 2019;res[3].departure.year = 2022;res[4].departure.year = 2023;
    res[0].reservation_type = "Classic Room";res[1].reservation_type = "Suite";res[2].reservation_type = "Classic Room";res[3].reservation_type = "Classic Room";res[4].reservation_type = "Suite";
    /*printf("%d",compare(res[0],res[3]));
    bubble_sort(res);
    for(int i = 0; i < 5;i++){
        for(int j = 0;res[i].name[j] != '\0'; j++){
            printf("%c",res[i].name[j]);
        }
        printf(" ");
    }

   printf("%d \n",calcul_duration(res[1]));
   printf("%d \n",price_2_pay(res[1]));  

   html(res[1]);
    html_file(res[1]);
    */
    comptes_html_file(res);

   return 0;
}

