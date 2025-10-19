#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/timerfd.h>
#include "../HashTable/hash.h"
#define KEYLENGTH 31
#ifndef VER
#define VER
Dictionary* keyStorage;
int epoll_fd;
char tokenSymbols[] = {
   // Uppercase letters
  'A','B','C','D','E','F','G','H','I','J','K','L','M',
  'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',

  // Lowercase letters
  'a','b','c','d','e','f','g','h','i','j','k','l','m',
  'n','o','p','q','r','s','t','u','v','w','x','y','z',

  // Digits
  '0','1','2','3','4','5','6','7','8','9',

  // Special characters
  '!','@','#','$','%','^','&','*','(',')',
  '-','_','=','+','[',']','{','}','|',';',
  ':',',','.','<','>','?','/','`','~'
};
char* GenererateTokenBasedAuth(int timeout);
int VerifyTokenBasedAuth(char* id);
void* start_loop();
int add_timer(int epollfd,int fd){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD,fd,&ev) == -1){
        printf("Here is error\n");
        perror("epoll_ctl: adding failed");
        return -1;
    }
    else{
        return 0;
    }
}
void* start_loop(){
    keyStorage = HashTable();
    epoll_fd = epoll_create1(0);
    printf("Loop stared\n");
    struct epoll_event events[10000];
    while(1){
        int length = epoll_wait(epoll_fd,events,10000,-1);
        if (length == -1){
            perror("epoll_wait");
            printf("Error, %s\n",strerror(errno)); 
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < length; i++)
        {
            printf("Time is over for fd: %d\n", events->data.fd);
            char* key;
            sprintf(key,"%d",events->data.fd);
            Remove(keyStorage,key);
            close(events->data.fd);

        }
    }
}
char* GenererateTokenBasedAuth(int timeout){
    char* token = (char*)malloc(sizeof(char)*30);
    for (int i = 0; i < KEYLENGTH; i++)
    {
        if (i == KEYLENGTH-1){
            token[i] = '\0';
            break;
        }
        int index = rand() % (91 + 1);
        token[i] = tokenSymbols[index];
    }
    struct itimerspec settings;
    settings.it_value.tv_sec = timeout;
    int fd_timer = timerfd_create(CLOCK_REALTIME,TFD_NONBLOCK);
    timerfd_settime(fd_timer,CLOCK_REALTIME,&settings,NULL);
    add_timer(epoll_fd,fd_timer);
    char* key = (char*)malloc(sizeof(char)*4);
    sprintf(key,"%d\0",fd_timer);
    Insert(keyStorage, key,token,"string");
    return key;
}
int VerifyTokenBasedAuth(char* id){
    if (id != NULL){
        char* result = (char*)Get(keyStorage,id);
        if (result != NULL){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}
#endif


