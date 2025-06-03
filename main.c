#include "server.h"
void handler(Request request){
    const char* body = "<html><body><h1>Hello World</h1></body></html>";
    int body_len = strlen(body);  // Don't hardcode!
    char response[512];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s", body_len, body);
    send(request.fd, response, strlen(response), 0);
}
void handler2(Request request){
    const char* body = "<html><body><h1>I love you!</h1></body></html>";
    int body_len = strlen(body);  // Don't hardcode!
    char response[512];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s", body_len, body);
    send(request.fd, response, strlen(response), 0);
}
int main(){
    MapGet("/", handler);
    MapGet("/love", handler2);
    Server server;
    server.host = "127.0.0.1";
    server.port = 1111;
    startListing(server);
    return 0;
}