#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "request.h"
#include "router.h"
#include "client.h"
typedef struct server
{
   char* host;
   int port;
   short int isRunning;
   int descriptor;
} Server;
void startListing(Server object){
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
        int listen_status = listen(sock,10000);
        printf("Status bind: %d, Status listen: %d\n",bind_status, listen_status);
        printf("Server started listing on %s:%d \n", object.host, object.port);
        while (1)
        {
            int client = accept(sock, (struct sockaddr*)NULL, NULL);
            int length = recv(client, buffer, sizeof(buffer)-1, 0);
            buffer[length] = '\0';
            if (length > 0){ 
                Request request = parseRequest(buffer);
                Client Client = getClient(client);
                LinearSearchRoute(request.path, request, Client);
                freeRequest(&request);
            }
        }
    }
}


 