#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static int pipefd1[2], pipefd2[2] ;

void INIT(void){
   if(pipe(pipefd1) < 0 || pipe(pipefd2) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
   }
}
void WRITE_TO_PARENT(char child_massage[100]){
    close(pipefd2[0]);
    write(pipefd2[1],child_massage,strlen(child_massage)+1);
    close(pipefd2[1]);
    printf("Child send message to parent!\n");
}
void READ_FROM_PARENT(void){
    char temp[100];
    close(pipefd1[1]);
    read(pipefd1[0],temp,100);
    close(pipefd1[0]);
    printf("Child receive message from parent!\n");
    printf("Receive message: %s\n",temp);
}
void WRITE_TO_CHILD(char parent_massage[100]){
    close(pipefd1[0]);
    write(pipefd1[1],parent_massage,strlen(parent_massage)+1);
    close(pipefd1[1]);
    printf("Parent send message to child!\n");
}
void READ_FROM_CHILD(void){
    char read_message[100];
    wait(NULL);
    close(pipefd2[1]);
    read(pipefd2[0],read_message,100);
    close(pipefd2[0]);
    printf("Parent receive message from child!\n");
    printf("Receive messae: %s\n",read_message);
}
int main(int argc,char* argv[]){
    INIT();
    char parent_massage[100] = "Hello Child";
    char child_massage[100] = "Hello Parent";
    pid_t pid;
    pid = fork();
    alarm (10);
    if(pid == 0){
        while(1){
        sleep(rand()%2+1);
        WRITE_TO_CHILD(parent_massage);
        READ_FROM_CHILD();
        }
    }else{
        while(1){
        sleep(rand()%2+1);
        READ_FROM_PARENT();
        WRITE_TO_PARENT(child_massage);
        }
    }
    return 0;
}
