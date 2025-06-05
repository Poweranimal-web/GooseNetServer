#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "client.h"
#include "response.h"
#ifndef ROUTER
#define ROUTER
#define RESULTPATH(client_path, founded_path) \
founded_path != NULL ? strcmp(founded_path, client_path) : -1 // method  for comparing address was requested by client and founded path, if founded_path is null return -1
int RESULT_TYPE_MESSAGE(char* typeRequest, char* type) {
    // printf("%s\n", typeRequest);
    // printf("%d\n", typeRequest != NULL ? strcmp(typeRequest, type) : -1);
    return typeRequest != NULL ? strcmp(typeRequest, type) : -1;
}
enum Method{
    GET,
    POST,
    PUT,
    DELETE
};
typedef struct route
{
    char* path;
    enum Method method;
    void (*handler)(Request request, Client client);
} Route;
typedef struct state
{
    int index; // current index of free cell in array
    int last; // last index  in array
} StateArray;
Route* ArrayRoutes; // storing routes that consisting of (path, method, handler) for webserver
StateArray stateRoute; // storing state of ArrayRoutes 
int Init = 0;
void MapGet(char* path, void (*handler)(Request request, Client client)); // append GET request handler to ArrayRoutes
int InitiliazeStateRoute(); // create properties for StateArray to start enumate ArrayRoutes
int InitiliazeStateRoute(){
    ArrayRoutes = (Route*)(malloc(sizeof(Route)*4));;
    stateRoute.index = 0;
    stateRoute.last = 4;
    return 0;
}
void MapGet(char* path, void (*handler)(Request request, Client client)){
    if (Init == 0){
        InitiliazeStateRoute();
        Init = 1;
    }
    if (stateRoute.index >= stateRoute.last){
        stateRoute.last = stateRoute.index + 1;
        ArrayRoutes = (Route*)realloc(ArrayRoutes, sizeof(Route)*(stateRoute.last));
        if (ArrayRoutes == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
        }
    }
    Route route;
    route.path = path;
    route.handler = handler;
    route.method = GET;
    ArrayRoutes[stateRoute.index++] = route;
}
void LinearSearchRoute(char* path, Request request, Client client){ 
    if (RESULT_TYPE_MESSAGE(request.headers.Sec_Fetch_Dest, "script") == 0){
        renderJS(path,client);
    }
    else if (RESULT_TYPE_MESSAGE(request.headers.Sec_Fetch_Dest, "style") == 0){
        renderCSS(path, client);
    }
    else {
        for (int i = 0; i < stateRoute.last; i++)
        {
            int resultSearch = RESULTPATH(path, ArrayRoutes[i].path);
            if (resultSearch == 0){
                ArrayRoutes[i].handler(request,client);
                return;
            }
        }
        renderErrorHTML(client);
    }
}
#endif
