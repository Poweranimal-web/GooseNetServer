typedef struct client
{
    Address address;
    int fd;
} Client;
typedef struct address
{
    char* ip;
    char* port;
} Address;
