#define ASYNC
#include <string.h>
#include "server.h"
#include "response.h"
#include "authefication.h"
#include "../HashTable/hash.h"
Dictionary* storage;
void handler(Request request, Client client){
    if (VerifyTokenBasedAuth(request.headers.Authorization)==1){
        char* key = get_param_string(&request, "key");
        Remove(storage, key);
        returnJson(request,"{'Status':'deleted'}", "200", client);
    }
    else{
        renderErrorHTML(client);
    }
}
void handler2(Request request, Client client){
    if (VerifyTokenBasedAuth(request.headers.Authorization)==1){
        char* key = get_param_string(&request, "key");
        char* value = get_param_string(&request, "value");
        Insert(storage, key,value,"string");
        returnJson(request,"{'Status':'added'}", "200", client);
    }
    else{
        printf("Error: Invalid token\n");
        renderErrorHTML(client);
    }
}
void handler3(Request request, Client client){
    if (VerifyTokenBasedAuth(request.headers.Authorization)==1)
    {
        char* string = StringInHashTable(storage);
        returnJson(request, string, "200", client);
    }
    else
    {
        renderErrorHTML(client);
    }
}
void handler5(Request request, Client client){
    char* token = GenererateTokenBasedAuth(120);
    char Response[13+strlen(token)];
    snprintf(Response, sizeof(Response), "{'token':'%s'}",token);
    returnJson(request,Response,"200", client);
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
    storage = HashTable(90);
    MapGet("/delete", handler);
    MapGet("/add", handler2);
    MapGet("/login", handler5);
    MapGet("/all", handler3);
    Server server;
    server.host = "127.0.0.1";
    server.port = 1111;
    startListing(server);
    return 0;
}