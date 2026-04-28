#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "request.h"
#include "router.h"
#include "client.h"
#include "../HashTable/queue.h"
#include "authefication.h"
typedef struct server
{
   char* host;
   int port;
   short int isRunning;
   int descriptor;
} Server;
#ifdef ASYNC
typedef struct events{
    struct epoll_event* array;
    int length;
} Events;
struct params{
    Queue* queue;
    Queue* garbageQueue;
    pthread_t* garbageThread;
    int* state;
};
int addEvent(int epollFd,int newFd){
    struct epoll_event ev;
    ev.data.fd = newFd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, newFd,&ev);
    return 0;
}
int read_data_socket(int fd, char* buffer){
    int length = 0;
    while (1)
    {
        int state = read(fd,buffer,4048);
        if (state == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;  // All data read
            } else {
                perror("read");
                break;
            }
        } else if (state == 0) {
            // Peer closed connection
            close(fd);
            break;
        } else {
            length+= state;
            continue;
        }
    }
    return length;
    
}
void accept_new_connection(int sockfd,int epollfd, Events* events){
    int new_connection = -1;
    int length = 0;
    while (1)
    {
        new_connection = accept(sockfd,(struct sockaddr*)NULL, NULL);
        if (new_connection == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more connections
                break;
            } else {
                perror("accept");
                break;
            }
        }
        fcntl(new_connection, F_SETFL, O_NONBLOCK);
        addEvent(epollfd,new_connection);
    }
}
void* handleRequests(void *param){ // handling request is here
    struct params *Param = (struct params *)param;
    Element* element = Pop(Param->queue); // extract request from queue
    while (element != NULL)
    {
        
        Request request = parseRequest(element->data); 
        Client Client = getClient(element->fd);
        LinearSearchRoute(request.path, request, Client);
        PushElement(Param->garbageQueue,element); // add request to garbage, because we were done it
        freeRequest(&request);
        element = Pop(Param->queue);
    }
    *(Param->state) = 0; // set state to create new thread.
    return NULL;
}
void startListing(Server object){
    initiateStorageVariables();
    pthread_t thread_token;
    pthread_create(&thread_token, NULL,start_loop,NULL);
    Events events;
    pthread_t thread;
    Element* garbageElement;
    events.length = 20000;
    events.array = (struct epoll_event*)malloc(events.length * sizeof(struct epoll_event));// create array for storing fd that has data in it.
    int epollfd = epoll_create(20000); // create instance of epoll 
    Queue* requestQueue = InithiateQueue(); // queue for storing income data from multiply fd to handle it in another thread.
    Queue* garbageQueue = InithiateQueue(); // queue for storing data which already handled to clear.
    int state_of_queue = 0;
    struct params* param = (struct params*)malloc(sizeof(struct params));
    param->queue = requestQueue;
    param->garbageQueue = garbageQueue;
    param->state = &state_of_queue;
    if (epollfd == -1) {
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return;
	}
    int sock = socket(AF_INET,SOCK_STREAM,0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    int true = 1;
    int sockset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    struct sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(object.port); // convert port to decent sequence of bytes for IPV4 network
    socketInfo.sin_addr.s_addr = socketInfo.sin_addr.s_addr;
    int result = inet_aton(object.host, &socketInfo.sin_addr); // convert address to decent sequence of bytes and store it in a structure
    int bind_status = bind(sock, (struct sockaddr*)&socketInfo, sizeof(socketInfo));
    if (bind_status == -1){
        printf("Status bind: %d\n",bind_status);
        printf("Error, %s",strerror(errno)); 
    }
    int listen_status = listen(sock,20000);
    addEvent(epollfd,sock);
    printf("Server started listing on %s:%d \n", object.host, object.port);
    while (1)
    {
        int lenght_ready_events = epoll_wait(epollfd,events.array,events.length,-1);// here is waiting for new data in fd
        // printf("Length: %d\n",lenght_ready_events);
        if (lenght_ready_events == -1){
            perror("epoll_wait");
            printf("Error, %s\n",strerror(errno)); 
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < lenght_ready_events; i++) // here is handling data
        {
            if (events.array[i].data.fd == sock){
                accept_new_connection(events.array[i].data.fd,epollfd,&events);
            }else{
                char* buffer = (char*)malloc(sizeof(char)*4048); // buffer for new data
                int length = read_data_socket(events.array[i].data.fd, buffer); // read data from socket
                buffer[length] = '\0';
                if (length > 0){
                    Push(requestQueue,buffer,events.array[i].data.fd); // add data to queue
                    if (state_of_queue == 0){ // creating thread for handling data if it doesn't exist
                        state_of_queue = 1;
                        pthread_create(&thread, NULL,handleRequests,param);
                    }
                }
            }

        }
        if (state_of_queue == 0){
            while ((garbageElement = Pop(garbageQueue)) != NULL) { // clear all data in garbage queue after compliting all request 
                free(garbageElement->data);
                free(garbageElement);
            }
        }
    }

    

}
#else
void startListing(Server object){
    initiateStorageVariables();
    pthread_t thread_token;
    pthread_create(&thread_token, NULL,start_loop,NULL);
    int sock = socket(AF_INET,SOCK_STREAM,0);
    int true = 1;
    int sockset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    char buffer[4048];
    struct sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(object.port); // convert port to decent sequence of bytes for IPV4 network
    int result = inet_aton(object.host, &socketInfo.sin_addr); // convert address to decent sequence of bytes and store it in a structure
    if (result == 1){
        socketInfo.sin_addr.s_addr = socketInfo.sin_addr.s_addr;
        int bind_status = bind(sock, (struct sockaddr*)&socketInfo, sizeof(socketInfo));
        printf("Error, %s",strerror(errno)); 
        int listen_status = listen(sock,20000);
        printf("Status bind: %d, Status listen: %d\n",bind_status, listen_status);
        printf("Server started listing on %s:%d \n", object.host, object.port);
        while (1)
        {
            int client = accept(sock, (struct sockaddr*)NULL, NULL);
            int length = recv(client, buffer, sizeof(buffer)-1, 0);
            buffer[length] = '\0';
            // printf("%s\n", buffer);
            if (length > 0){ 
                Request request = parseRequest(buffer);
                Client Client = getClient(client);
                LinearSearchRoute(request.path, request, Client);
                freeRequest(&request);
            }
        }
    }
}
#endif


 