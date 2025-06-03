#include <stdlib.h>
#include <string.h>
#include "request.h"
#ifndef ROUTER
#define ROUTER
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
    void (*handler)(Request request);
} Route;
typedef struct state
{
    int index; // current index of free cell in array
    int last; // last index  in array
} StateArray;
Route* ArrayRoutes; // storing routes that consisting of (path, method, handler) for webserver
StateArray stateRoute; // storing state of ArrayRoutes 
int Init = 0;
void MapGet(char* path, void (*handler)(Request request)); // append GET request handler to ArrayRoutes
int InitiliazeStateRoute(); // create properties for StateArray to start enumate ArrayRoutes
int InitiliazeStateRoute(){
    ArrayRoutes = (Route*)(malloc(sizeof(Route)*4));;
    stateRoute.index = 0;
    stateRoute.last = 4;
    return 0;
}
void MapGet(char* path, void (*handler)(Request request)){
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
void LinearSearchRoute(char* path, Request request){
    for (int i = 0; i < stateRoute.last; i++)
    {
        int resultSearch = ArrayRoutes[i].path != NULL ? strcmp(ArrayRoutes[i].path, path) : -1;
        if (resultSearch == 0){
            ArrayRoutes[i].handler(request);
            return;
        }
    }
    printf("Unfound path!\n");
    

}
#endif
