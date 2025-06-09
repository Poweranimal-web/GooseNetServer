#include <sys/socket.h>
#ifndef CLIENT
#define CLIENT
typedef struct address
{
    char* ip;
    int port;
} Address;
typedef struct client
{
    Address address;
    int fd;
} Client;
Client getClient(int fd){ // get address of client socket and put it in Client struct
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(fd, (struct sockaddr *)&addr, &addr_size);
    char ip[20];
    strcpy(ip, inet_ntoa(addr.sin_addr));
    Client Client;
    Client.fd = fd;
    Client.address.ip = ip;
    Client.address.port = ntohs(addr.sin_port);
    return Client;  
}
#endif
