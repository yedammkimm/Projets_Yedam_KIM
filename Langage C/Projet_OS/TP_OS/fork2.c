#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    pid_t p = fork();
    if(p<0){
        perror("fork fail");
        exit(1);
    }
    else if(p==0){
        printf("Hello world! from child, process_id(pid) = %d\n",getpid());
        exit(0);
    }
    else{
        sleep(4);
        printf("Hello world! from parent, process_id(pid) = %d \n",getpid());
    }
    return 0; 
}