#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include "client.h"
#include "template.h"
#include "../HashTable/hash.h"
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
    char Date[37];
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
// features for controling cookies below
char* get_cookie_string(Request* request,char* key){
    return (char*)Get(request->cookies, key);
}
void remove_cookie(Request* request,char* key){
    Insert(request->cookies,key, "; Max-Age=0; Path=/","string");
}
void add_cookie_string(Request* request,char* key, char* value){
    Insert(request->cookies,key, value,"string");
}
// method for getting params if it exists
char* get_param_string(Request* request,char* key){
    return (char*)Get(request->parametrs,key);
}
char* serialize_cookie_hash(Dictionary* table) {
    if (!table) return NULL;
    size_t capacity = 1024;
    char* result = (char*)malloc(sizeof(char)*capacity);
    if (!result) return NULL;
    result[0] = '\0';
    for (size_t i = 0; i < table->length; ++i) {
        HashEntry* entry = &table->hashtable[i];
        while (strcmp(entry->key, "") != 0) {
            if (entry->key && entry->value) {
                // Calculate needed space for one Set-Cookie line + newline
                size_t needed = strlen("Set-Cookie: ") + strlen(entry->key) + 1 + strlen(entry->value) + strlen("\r\n") + 1;
                size_t current_len = strlen(result);
                if (current_len + needed >= capacity) {
                    capacity *= 2;
                    result = realloc(result, capacity);
                    if (!result) return NULL;
                }

                strcat(result, "Set-Cookie: ");
                strcat(result, entry->key);
                strcat(result, "=");
                strcat(result, entry->value);
                strcat(result, "\r\n");
            }
            entry = entry->next_entry;
        }
    }

    return result;
}

char* readHtml(char* path){
    FILE* file;
    int i = 0;
    file = fopen(path, "r");
    if (file == NULL){
        printf("Path of file: %s", path);
        printf("Error, %s\n",strerror(errno)); 
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* html = (char*)(malloc(sizeof(char)* 1000+1));
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
    char* original_content = readHtml(path);
    // if (strcmp(status, "200") == 0){
    //      printf("Body: %s\n", original_content);
    //     printf("Length: %d\n",strlen(original_content));   
    // }
    TokenArray tokens = tokenize(original_content);
    TokenArray new_tokens = analyzeTokens(tokens);
    char* content = returnUpdatedContent(new_tokens);
    response.body = content;
    response.headers.Content_Type = "text/html";
    response.headers.Content_Length = strlen(response.body);
    response.headers.Connection = "close";
    time_t t = time(NULL);
    struct tm tm;
    if (t == -1){
        printf("Error, %s",strerror(errno));
    }
    if (localtime_r(&t, &tm) == NULL) {
        printf("Error, %s",strerror(errno));
    }
    sprintf(response.headers.Date,  "%s, %02d %02d %4d %02d:%02d:%02d GMT" ,
    weekDays[tm.tm_wday],tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return response;
}
Response createJsonResponse(char* data, char* status){ // create instance of response
    Response response;
    response.status = status;
    response.phrase = returnPhrase(response.status);
    response.version = "HTTP/1.1";
    response.body = data;
    response.headers.Content_Type = "application/json";
    response.headers.Content_Length = strlen(data);
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

int renderHTML(Request request,char* path_html, Client client){
    Response response = createHTMLResponse(path_html, "200");
    char* cookies = serialize_cookie_hash(request.cookies);
    char Response[4000];
    snprintf(Response, sizeof(Response),
        "%s %s %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Date: %s\r\n"
        "Connection: %s\r\n"
        "%s"
        "\r\n"
        "%s", response.version,response.status,response.phrase, 
        response.headers.Content_Type,response.headers.Content_Length,response.headers.Date,
        response.headers.Connection,cookies, response.body);
    send(client.fd, Response, strlen(Response), 0);
    free(response.body);
    free(cookies);
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
    if (strcmp(response.status,"404") == 0){
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
int returnJson(Request request,char* data, char* status, Client client){
    Response response = createJsonResponse(data, status);    
    char* cookies = serialize_cookie_hash(request.cookies);
    char Response[4000+strlen(data)];
    snprintf(Response, sizeof(Response),
        "%s %s %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Date: %s\r\n"
        "Connection: %s\r\n"
        "%s"
        "\r\n"
        "%s", response.version,response.status,response.phrase, 
        response.headers.Content_Type,response.headers.Content_Length,response.headers.Date,
        response.headers.Connection,cookies, response.body);
    send(client.fd, Response, strlen(Response), 0);
    free(cookies);
    close(client.fd);
    return 0;
}
#endif