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
int main(){
    MapGet("/", handler);
    MapGet("/love", handler2);
    MapGet("/admin", handler3);
    Server server;
    server.host = "127.0.0.1";
    server.port = 1111;
    startListing(server);
    return 0;
}