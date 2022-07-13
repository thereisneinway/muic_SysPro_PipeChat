#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

void *func(void *arg) {
  int index = *((int *)arg);
  if(index == 1){
    printf("You are primary\n");
    int p2;
    char * s2p = "/tmp/s2p";
    mkfifo(s2p, 0666);
    p2 = open(s2p, O_RDONLY);
    char arg2[50];
    while (1){
      read(p2, arg2, 50);
      printf("Secondary: %s", arg2);
      if(strncmp(arg2, "end chat", 1) == 0) exit(EXIT_FAILURE);
    }
  }
  else if(index == 2){
    printf("You are secondary\n");
    int p1;
    char * p2s = "/tmp/p2s";
    mkfifo(p2s, 0666);
    p1 = open(p2s, O_RDONLY);
    char arg1[50];
    while (1){
      read(p1, arg1, 50);
      printf("Primary: %s", arg1);
      if(strncmp(arg1, "end chat", 1) == 0) exit(EXIT_FAILURE);
    }
  }


  else if(index == 3){
    int p1;
    char * p2s = "/tmp/p2s";
    mkfifo(p2s, 0666);
    p1 = open(p2s, O_WRONLY);
    char arg1[50];
    while (1){
      fgets(arg1, 50, stdin);
      write(p1, arg1, strlen(arg1)+1);
      if(strncmp(arg1, "end chat", 1) == 0) exit(EXIT_FAILURE);
    }
  }
  else if(index == 4){
    int p2;
    char * s2p = "/tmp/s2p";
    mkfifo(s2p, 0666);
    char arg2[50];
    p2 = open(s2p, O_WRONLY);
    while (1){
      fgets(arg2, 50, stdin);
      write(p2, arg2, strlen(arg2)+1);
      if(strncmp(arg2, "end chat", 1) == 0) exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]){
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <[1,2]>\n", *argv);
    exit(EXIT_FAILURE);
  }
  pthread_t read,write;
  argv++;
  if(strncmp(*argv, "1", 1) == 0){
    int index1 = 1;
    int index2 = 3;
    pthread_create(&read, NULL, &func, &index1);
    pthread_create(&write, NULL, &func, &index2);
    pthread_join(read, NULL);
    pthread_join(write, NULL);
  }
  else if(strncmp(*argv, "2", 1) == 0){
    int index1 = 2;
    int index2 = 4;
    pthread_create(&read, NULL, &func, &index1);
    pthread_create(&write, NULL, &func, &index2);
    pthread_join(read, NULL);
    pthread_join(write, NULL);
  }
  

  printf("terminated incorrectly\n");
}