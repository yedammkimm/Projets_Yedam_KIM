#include <stdio.h>

void ex01(){
    for(int i = 1; i < 101;i++){
        if((0 == i%3) && (0 == i%7)){
            printf("FizzBuzz ");
        }else if(0 == i%3){
            printf("Fizz ");
        }else if(0 == i%7){
            printf("Buzz ");
        }else{
            printf("%d ",i);
        }
    }
}

void decode(){
    /*We comapre two characters from the screen, and incremente 1 if there are same else change */
    int a; 
    char b;
    printf("Write a number and a character (with a space between) \n");
    scanf("%d %c",&a,&b);
    while(a != -1){
        if(a == 0){
            printf("\n");
        }else{
            for(int i = 1 ; i <= a; i++){
                printf("%c",b);
            }
        }
        printf("\nWrite another number and a character (with a space between)\n");
        scanf("%d %c",&a,&b);
    }
}

void decodefichier(){
    FILE* f = fopen("code1.txt", "r");//C:\Users\pltguest\Desktop\tp2
    if (!f){
        printf("Error Open file");
    }
    int a; 
    char b;
    fscanf(f,"%d %c",&a,&b);
    while(a != -1){
        if(a == 0){
            printf("\n");
        }else{
            for(int i = 1 ; i <= a; i++){
                printf("%c",b);
            }
        }
        fscanf(f,"%d %c",&a,&b);
    }
    fclose(f);
}


void rle(){
    char before;
    char after;
    char mid;
    int i = 1;
    printf("Write code RLE \n");
    scanf("%c%c",&before,&after);
    while(before != '#'){
        while(before == after){
        ++i;
        scanf("%c",&after);
        }
    if(after == '\n'){
        scanf("%c",&mid);
        printf("%d %c ",i,before);
        after = mid;
        printf("Ø quelconque ");
    }else{
        mid = after;
        scanf("%c",&after);
        printf("%d %c ",i,mid);
    }
    } 
    i = 1;
    printf("-1 FIN");
}



int main()
{
    /*ex01();
    rle();    
    decodefichier() */
    rle();
    return 0;
/*
a
b */
    
}