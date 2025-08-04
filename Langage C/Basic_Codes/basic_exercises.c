#include <stdio.h>
#include <stdbool.h>
void ex1_1(){
    int a; 
    int b; 
    printf("Saisir deux valeurs \n");
    scanf("%d %d",&a,&b);
    for(int i = a ; i <= b; i++){
        printf("%d \n",i);
    }
}

void ex1_2(){
    int a; 
    int b; 
    printf("Saisir deux valeurs \n");
    scanf("%d %d",&a,&b);
    for(int i = a ; i < b; i++){
        printf("%d \n",i);
    }
}

void ex1_3(){
    int a; 
    int b; 
    printf("Enter two Numbers \n");
    scanf("%d %d",&a,&b);
    for(int i = b ; i >= a; i--){
        printf("%d \n",i);
    }
}

void ex1_4(){
    int a; 
    int b; 
    printf("Enter two Numbers \n");
    scanf("%d %d",&a,&b);
    if(a > b){
        int rep; 
        rep = a; 
        a = b; 
        b = rep; 
        printf("Changed a and b \n");
    }
    for(int i = b ; i >= a; i--){
        printf("%d \n",i);
    }
}

void exo_2(){
    int num;
    int nb = 0; 
    printf("Enter a positive number\n");
    scanf("%d",&num);
    while(num != 0){
        nb = nb * 10;
        nb = nb + num%10;
        num = num/10;
    }
        printf("%d \n", nb);
}

void exo_3(){
    int num; 
    int res; 
    int nb_bits = 0;
    printf("Enter a number\n");
    scanf("%d",&num);
    while(num > 0){
        for(int i = 0; i < 31;i++){
            res = (1 << i) & num;
            if(res > 0){
                nb_bits++;
            }
            res = num;
        }
    printf("number of 1 bits %d \n",nb_bits);  
    nb_bits = 0;
    printf("Enter another number\n");
    scanf("%d",&num);
    }
}



void exo_4(){
    char c;
    printf("Enter a hexadecimal number\n");
    scanf("%c",&c);
    int res = 0;
    while((c >= 48 && c <= 57) || (c >= 65 && c <= 70)){
        res *= 16; 
        if(c >= 48  && c <= 57){
            res += (c-48);
        }
        if(c >= 65  && c <= 70){
            res += (c-55);
        }
        scanf("%c",&c);
    }
    printf("%d",res);
}

int main()
{
    /*ex1(); 
    ex1_2(); 
    ex1_4();
    exo_2();
    exo_3();*/
    exo_4();
    
    return 0;
}
