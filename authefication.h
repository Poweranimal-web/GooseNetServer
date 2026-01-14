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
typedef struct api_creaditianals{ // entity for parsing auth creaditionals in API authorization.

    char* key;
    char* value;
} ApiAuth;
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
  '-','_','=','+','[',']','{','}',';',
  ',','.','<','>','?','/','~'
};
char* GenererateTokenBasedAuth(int timeout);
int VerifyTokenBasedAuth(char* key);
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
    keyStorage = HashTable(90);
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
        int index = rand() % (88 + 1);
        token[i] = tokenSymbols[index];
    }
    struct itimerspec settings;
    settings.it_value.tv_sec = timeout;
    int fd_timer = timerfd_create(CLOCK_REALTIME,TFD_NONBLOCK);
    timerfd_settime(fd_timer,CLOCK_REALTIME,&settings,NULL);
    add_timer(epoll_fd,fd_timer);
    char* key = (char*)malloc(sizeof(char)*100);
    char* result = (char*)malloc(sizeof(char)*140);
    sprintf(key,"%d\0",fd_timer);
    Insert(keyStorage, key,token,"string");
    sprintf(result,"%s:%s\0",key, token);
    return result;
}
ApiAuth parseAPIkey(char* value){
    int key = 1;
    ApiAuth data = {0};
    printf("%s\n",value);
    int length_data = strlen(value);
    int current_length = 0;
    data.key = (char*)malloc(sizeof(char)*100);
    data.value = (char*)malloc(sizeof(char)*KEYLENGTH);
    char* key_ptr = data.key;
    char* value_ptr = data.value;
    while (current_length < length_data)
    {
        if (*value == ':'){
            key = 0;
            key_ptr = '\0';
            current_length++;
            value++;
            continue;
        }
        if (key == 1){
            *key_ptr = *value;
            key_ptr++;
        }
        else{
            *value_ptr = *value;
            value_ptr++;
        }
        current_length++;
        value++;
    }
    return data;
    
}
int VerifyTokenBasedAuth(char* key){
    if (key != NULL){
        ApiAuth cread = parseAPIkey(key);
        if (strlen(cread.key) > 0){
            char* result = (char*)Get(keyStorage,cread.key);
            if (result != NULL){
                if (strcmp(result,cread.value) == 0){
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
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
    
}
#endif


