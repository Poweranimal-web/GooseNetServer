#ifndef CLIENT
#define CLIENT
typedef struct address
{
    char* ip;
    char* port;
} Address;
typedef struct client
{
    Address address;
    int fd;
} Client;
#endif
