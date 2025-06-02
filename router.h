#include <stdlib.h>
#include <string.h>
#include "request.h"
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
enum Method{
    GET,
    POST,
    PUT,
    DELETE
};
Route* ArrayRoutes = (Route*)(malloc(sizeof(Route)*4)); // storing routes that consisting of (path, method, handler) for webserver
StateArray stateRoute; // storing state of ArrayRoutes 
void MapGet(char* path, void (*handler)(char* request)); // append GET request handler to ArrayRoutes
int InitiliazeState(StateArray* state); // create properties for StateArray to start enumate ArrayRoutes
int InitiliazeState(StateArray* state){
    state->index = 0;
    state->last = 4;
    return 0;
}
void MapGet(char* path, void (*handler)(Request request)){
    Route route;
    route.path = path;
    route.handler = handler;
    route.method = GET;
    ArrayRoutes[stateRoute.index++] = route;
}
void LinearSearchRoute(char* path, Request request){
    for (int i = 0; i < stateRoute.last; i++)
    {
        if (strcmp(ArrayRoutes[i].path, path) == 0){
            ArrayRoutes[i].handler(request);
        }
    }
    

}
