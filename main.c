#include "server.h"
#include "response.h"
void handler(Request request, Client client){
    renderHTML("./html/hello.html", client);
}
void handler2(Request request, Client client){
    renderHTML("./html/love.html", client);
}
void handler3(Request request, Client client){
    renderHTML("./html/page.html", client);
}
void handler4(Request request, Client client){
    printf("This password: %s\n", (char*)Get(request.parametrs,"password"));
    printf("This nickname: %s\n", (char*)Get(request.parametrs,"nickname"));
    printf("This id: %s\n", (char*)Get(request.parametrs,"id"));
    printf("This page: %s\n", (char*)Get(request.parametrs,"page"));
    renderHTML("./html/page.html", client);
}
// i need to fix data clearing in hashtable  
int main(){
    MapGet("/", handler);
    MapGet("/love", handler2);
    // MapGet("/admin", handler3);
    MapGet("/admin", handler4);
    Server server;
    server.host = "127.0.0.1";
    server.port = 1111;
    startListing(server);
    return 0;
}