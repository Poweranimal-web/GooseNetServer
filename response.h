#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include "client.h"
#ifndef RESPONSE
#define RESPONSE
typedef struct headers{
    char* Access_Control_Allow_Origin;
    char* Accept_Charset;
    char* Accept_Encoding;
    char* Accept_Language;
    char* Connection;
    int Content_Length;
    char* Content_Type;
    char* User_Agent;
    char Date[30];
    char* Server;
    char* Set_Cookie;
    char* Last_Modified;
} HeaderResponse;
typedef struct response
{
    char* version;
    char* status;
    char* phrase;
    char* body;
    HeaderResponse headers;
} Response;
char weekDays[7][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"}; // array for date in response
char* returnPhrase(char* status){
    if (strcmp(status, "100") == 0){
        return "Continue";
    }
    else if(strcmp(status, "101") == 0){
        return "Switching Protocols";
    }
    else if(strcmp(status, "200") == 0){
        return "OK";
    }
    else if(strcmp(status, "201") == 0){
        return "Created";
    }
    else if(strcmp(status, "202") == 0){
        return "Accepted";
    }
    else if(strcmp(status, "203") == 0){
        return "Non-Authoritative Information";
    }
    else if(strcmp(status, "204") == 0){
        return "No Content";
    }
    else if(strcmp(status, "205") == 0){
        return "Reset Content";
    }
    else if(strcmp(status, "206") == 0){
        return "Reset Content";
    }
    else if(strcmp(status, "300") == 0){
        return "Multiple Choices";
    }
    else if(strcmp(status, "301") == 0){
        return "Moved Permanently";
    }
    else if(strcmp(status, "302") == 0){
        return "Found";
    }
    else if(strcmp(status, "303") == 0){
        return "See Other";
    }
    else if(strcmp(status, "304") == 0){
        return "Not Modified";
    }
    else if(strcmp(status, "304") == 0){
        return "Not Modified";
    }
    else if(strcmp(status, "305") == 0){
        return "Use Proxy";
    }
    else if(strcmp(status, "307") == 0){
        return "Temporary Redirect";
    }
    else if(strcmp(status, "400") == 0){
        return "Temporary Redirect";
    }
    else if(strcmp(status, "401") == 0){
        return "Unauthorized";
    }
    else if(strcmp(status, "402") == 0){
        return "Payment Required";
    }
    else if(strcmp(status, "403") == 0){
        return "Forbidden";
    }
    else if(strcmp(status, "404") == 0){
        return "Not Found";
    }
    else if(strcmp(status, "405") == 0){
        return "Method Not Allowed";
    }
    else if(strcmp(status, "406") == 0){
        return "Not Acceptable";
    }
    else if(strcmp(status, "407") == 0){
        return "Proxy Authentication Required";
    }
    else if(strcmp(status, "408") == 0){
        return "Request Time-out";
    }
    else if(strcmp(status, "409") == 0){
        return "Conflict";
    }
    else if(strcmp(status, "410") == 0){
        return "Gone";
    }
    else if(strcmp(status, "411") == 0){
        return "Length Required";
    }
    else if(strcmp(status, "412") == 0){
        return "Precondition Failed";
    }
    else if(strcmp(status, "413") == 0){
        return "Request Entity Too Large";
    }
    else if(strcmp(status, "414") == 0){
        return "Request-URI Too Large";
    }
    else if(strcmp(status, "414") == 0){
        return "Request-URI Too Large";
    }
    else if(strcmp(status, "415") == 0){
        return "Unsupported Media Type";
    }
    else if(strcmp(status, "416") == 0){
        return "Requested range not satisfiable";
    }
    else if(strcmp(status, "417") == 0){
        return "Expectation Failed";
    }
    else if(strcmp(status, "500") == 0){
        return "Internal Server Error";
    }
    else if(strcmp(status, "501") == 0){
        return "Not Implemented";
    }
    else if(strcmp(status, "502") == 0){
        return "Bad Gateway";
    }
    else if(strcmp(status, "503") == 0){
        return "Service Unavailable";
    }
    else if(strcmp(status, "504") == 0){
        return "Gateway Time-out";
    }
    else if(strcmp(status, "505") == 0){
        return "HTTP Version not supported";
    }
    else{
        return "Uknown";
    }
}

char* readHtml(char* path){
    FILE* file;
    int i = 0;
    file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* html = (char*)(malloc(sizeof(char)* size+1));
    int ch = fgetc(file);
    while (ch != EOF){
        html[i++]= (char)ch;
        ch = fgetc(file);
    }
    html[i]= '\0';
    fclose(file);
    return html;
}
char* readfile(char* path){ // read some static files
    FILE* file;
    int i = 0;
    char fullpath[50] = "./html";
    strcat(fullpath, path);
    file = fopen(fullpath, "r");
    if (file != NULL){
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* fileCustom = (char*)(malloc(sizeof(char)* size+1));
        int ch = fgetc(file);
        while (ch != EOF){
            fileCustom[i++]= (char)ch;
            ch = fgetc(file);
        }
        fileCustom[i]= '\0';
        fclose(file);
        return fileCustom;
    }
    else{
        return "e";
    }

}
Response createHTMLResponse(char* path, char* status){ // create instance of response
    Response response;
    response.status = status;
    response.phrase = returnPhrase(response.status);
    response.version = "HTTP/1.1";
    response.body = readHtml(path);
    response.headers.Content_Type = "text/html";
    response.headers.Content_Length = strlen(response.body);
    response.headers.Connection = "close";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(response.headers.Date,  "%s, %02d %02d %4d %02d:%02d:%02d GMT" ,
    weekDays[tm.tm_wday],tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return response;
}
Response createResponse(char* path,char* type,char* status){ // Create custom entity of response
    Response response;
    response.version = "HTTP/1.1";;
    char* bodyResult = readfile(path);
    if (strcmp(bodyResult, "e") == 0){
        response.status = "404";
        response.phrase = returnPhrase("404");
    }
    else{
        response.status = status;
        response.body = bodyResult; 
        response.phrase = returnPhrase(response.status);
    }
    response.headers.Content_Type = type;
    response.headers.Content_Length = strlen(response.body);
    response.headers.Connection = "close";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(response.headers.Date,  "%s, %02d %02d %4d %02d:%02d:%02d GMT" ,
    weekDays[tm.tm_wday],tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return response;
}

int renderHTML(char* path_html, Client client){
    Response response = createHTMLResponse(path_html, "200");
    char Response[4000];
    snprintf(Response, sizeof(Response),
        "%s %s %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Date: %s\r\n"
        "Connection: %s\r\n"
        "\r\n"
        "%s", response.version,response.status,response.phrase, 
        response.headers.Content_Type,response.headers.Content_Length,response.headers.Date,
        response.headers.Connection, response.body);
    send(client.fd, Response, strlen(Response), 0);
    free(response.body);
    close(client.fd);
    return 0;
}
int renderErrorHTML(Client client){
    Response response = createHTMLResponse("./html/error.html", "404");
    char Response[4000];
    snprintf(Response, sizeof(Response),
        "%s %s %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Date: %s\r\n"
        "Connection: %s\r\n"
        "\r\n"
        "%s", response.version,response.status,response.phrase, 
        response.headers.Content_Type,response.headers.Content_Length,response.headers.Date,
        response.headers.Connection, response.body);
    send(client.fd, Response, strlen(Response), 0);
    free(response.body);
    close(client.fd);
    return 0;
}
int renderJS(char* path, Client client){ // send js file
    Response response = createResponse(path,"text/javascript", "200");
    if (response.status == "404"){
        renderErrorHTML(client);
        return -1;
    }
    else{
        int body_len = strlen(response.body);
        char Response[4000];
        snprintf(Response, sizeof(Response),
            "%s %s %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"
            "Date: %s\r\n"
            "Connection: %s\r\n"
            "\r\n"
            "%s", response.version,response.status,response.phrase, 
            response.headers.Content_Type,body_len,response.headers.Date,
            response.headers.Connection, response.body);
        send(client.fd, Response, strlen(Response), 0);
        free(response.body);
        close(client.fd);
        return 0;

    }
}
int renderCSS(char* path, Client client){ // send css file
    Response response = createResponse(path,"text/css", "200");
    if (response.status == "404"){
        renderErrorHTML(client);
        return -1;
    }
    else{
        int body_len = strlen(response.body);
        char Response[512];
        snprintf(Response, sizeof(Response),
            "%s %s %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"
            "Date: %s\r\n"
            "Connection: %s\r\n"
            "\r\n"
            "%s", response.version,response.status,response.phrase, 
            response.headers.Content_Type,body_len,response.headers.Date,
            response.headers.Connection, response.body);
        send(client.fd, Response, strlen(Response), 0);
        free(response.body);
        close(client.fd);
        return 0;

    }
}
#endif