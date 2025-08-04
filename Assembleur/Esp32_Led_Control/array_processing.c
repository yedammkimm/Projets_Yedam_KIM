#include <stdio.h>
#include <stdlib.h>




void main(int N, int* tab);


void app_main(void)
{
    int N = 10; 
    int tab[] = {12,36,8,5,59,798,65,48,12,58};
    main(N,tab);
    for(int i = 0; i < N; i++){
        printf("%d",tab[i]);
    }
}