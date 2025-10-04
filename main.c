#define ASYNC
#include "server.h"
#include "response.h"
void handler(Request request, Client client){
    renderHTML(request,"./html/hello.html", client);
}
void handler2(Request request, Client client){
    renderHTML(request,"./html/love.html", client);
}
void handler3(Request request, Client client){
    renderHTML(request,"./html/page.html", client);
}
void handler4(Request request, Client client){
    printf("This password: %s\n", get_param_string(&request,"password"));
    printf("This nickname: %s\n", get_param_string(&request,"nickname"));
    printf("This id: %s\n",  get_param_string(&request,"id"));
    printf("This page: %s\n",  get_param_string(&request,"page"));
    renderHTML(request,"./html/page.html", client);
}
// i need to fix data clearing in hashtable  
int main(){
    MapGet("/", handler);
    MapGet("/love", handler2);
    // MapGet("/admin", handler3);
    MapGet("/admin", handler3);
    Server server;
    server.host = "127.0.0.1";
    server.port = 1111;
    startListing(server);
    return 0;
}