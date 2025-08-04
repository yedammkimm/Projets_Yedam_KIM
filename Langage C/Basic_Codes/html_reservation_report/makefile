main: main.o html_reservation_report.o
	gcc -o main main.o html_reservation_report.o

html_reservation_report.o: html_reservation_report.c 
	gcc -o html_reservation_report.o -c html_reservation_report.c -W -Wall -ansi -pedantic -std=c99 -std=gnu99 

main.o: main.c html_reservation_report.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic -std=c99 -std=gnu99 

clean:
	rm *.o
