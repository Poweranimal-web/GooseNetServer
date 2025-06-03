#include <stdio.h>
#ifndef REQUEST
#define REQUEST
#define MAX_HEADER_NAME_SIZE 64
#define MAX_HEADER_VALUE_SIZE 4000
typedef struct headers{
    char* Host;
    char* Accept;
    char* Accept_Charset;
    char* Accept_Encoding;
    char* Accept_Language;
    char* Expect;
    char* Connection;
    char* Content_Length;
    char* Content_Type;
    char* User_Agent;
    char* Authorization;
    char* Cookie;
} Header;
typedef struct request
{
  char method[10];
  char path[4096];
  char version[15];
  int fd;
  Header headers;  
} Request;
enum HeaderType {
    HOST,
    ACCEPT,
    ACCEPT_CHARSET,
    ACCEPT_ENCODING,
    ACCEPT_LANGUAGE,
    EXPECT,
    CONNECTION,
    CONTENT_LENGTH,
    CONTENT_TYPE,
    USER_AGENT,
    AUTHORIZATION,
    COOKIE,
    UNKNOWN
};
Request parseRequest(char* request);
void parseField(char* header, char* value, Request* request);
enum HeaderType parseHeader(char* header);
void freeRequest(Request* req);
void parseRequestLine(char* ch,int done, Request* request);

Request parseRequest(char* request){
    Request req = {0};
    char bufferHeader[MAX_HEADER_NAME_SIZE];
    char bufferValue[MAX_HEADER_VALUE_SIZE];
    int i = 0;
    int indexValue = 0;
    int indexHeader = 0;
    int value = 0;
    int first = 1;
    while (1)
    {   
        if (*request == '\0'){
            break;
        }
        else if (*request == ':' && value == 0)
        {
            value = 1;
            request+=2;
        }
        if (value == 0 && first != 1){ // check if it cross :
            bufferHeader[indexHeader++] = *request;
        }
        else if(value == 1){
            bufferValue[indexValue++] = *request;
        }

        if (first == 1){
            parseRequestLine(request,0,&req);
        }
        if (*(request+i) == '\r' && *(request+i+1) == '\n'){
            bufferHeader[indexHeader] = '\0';
            bufferValue[indexValue] = '\0';
            parseField(bufferHeader,  bufferValue, &req);
            indexHeader = 0;
            indexValue = 0;
            value = 0;
            i=0;
            if (first == 1){
                parseRequestLine(request,1,&req);
                first = 0;
            }
            request++;
        }
        request++;

    }
    return req;
    
}
void parseRequestLine(char* ch,int done, Request* request){
    static int index = 0;
    static int i = 0;
    if (done == 1){
        index = 0;
        i = 0;
        return;
    }
    if (*ch == ' '){
        index += 1;
        i = 0;
        return;
    }
    if (index == 0){
        request->method[i++] = *ch; 
    }
    else if (index == 1){
        request->path[i++] = *ch;
    }
    else{
        request->version[i++] = *ch;
    }
}
enum HeaderType parseHeader(char* header){
    if (strcmp(header, "Host") == 0){
        return HOST;
    }
    else if (strcmp(header, "Accept") == 0){
        return ACCEPT;
    }
    else if(strcmp(header, "Accept-Charset") == 0)
    {
        return ACCEPT_CHARSET;
        
    }
    else if(strcmp(header, "Accept-Encoding") == 0){
        return ACCEPT_ENCODING;
    }
    else if (strcmp(header, "Accept-Language") == 0)
    {
        return ACCEPT_LANGUAGE;
    }
    else if (strcmp(header, "Expect") == 0)
    {
        return EXPECT;
    }
    else if (strcmp(header, "Connection") == 0){
        return CONNECTION;
    }
    else if(strcmp(header, "Content-Length") == 0){

        return CONTENT_LENGTH;
    }
    else if(strcmp(header, "Content-Type") == 0){

        return CONTENT_TYPE;
    }
    else if(strcmp(header, "User-Agent") == 0){

        return USER_AGENT;
    }
    else if(strcmp(header, "Authorization") == 0){
        return AUTHORIZATION;
    }
    else if(strcmp(header, "Cookie") == 0){
        return COOKIE;
    }
    else{
        return UNKNOWN;
    }
    
    
    
}
void parseField(char* header, char* value, Request* request){
    enum HeaderType Header = parseHeader(header);
    switch (Header)
    {
        case HOST:
            request->headers.Host = strdup(value); // strdup create indepedent copy of value param
            break;
        case ACCEPT:
            request->headers.Accept = strdup(value);
            break;
        case ACCEPT_CHARSET:
            request->headers.Accept_Charset = strdup(value);
            break;
        case ACCEPT_ENCODING:
            request->headers.Accept_Encoding = strdup(value);
            break;
        case ACCEPT_LANGUAGE:
            request->headers.Accept_Language = strdup(value);
            break;
        case EXPECT:
            request->headers.Expect = strdup(value);
            break;
        case CONNECTION:
            request->headers.Connection = strdup(value);
            break;
        case CONTENT_LENGTH:
            request->headers.Content_Length = strdup(value);
            break;
        case CONTENT_TYPE:
            request->headers.Content_Type = strdup(value);
            break;
        case USER_AGENT:
            request->headers.User_Agent = strdup(value);
            break;
        case AUTHORIZATION:
            request->headers.Authorization = strdup(value);
            break;
        case COOKIE:
            request->headers.Cookie = strdup(value);
            break;
        default:
            break;
    }
}
void freeRequest(Request* req){
    if (req->headers.Host != NULL){
        free(req->headers.Host);
    }
    if (req->headers.User_Agent != NULL){
        free(req->headers.User_Agent);
    }
    if (req->headers.Accept != NULL){
        free(req->headers.Accept);
    }
    if (req->headers.Accept_Charset != NULL){
        free(req->headers.Accept_Charset);
    }
    if (req->headers.Accept_Encoding != NULL){
        free(req->headers.Accept_Encoding);
    }
    if (req->headers.Accept_Language != NULL){
        free(req->headers.Accept_Language);
    }
    if (req->headers.Expect != NULL){
        free(req->headers.Expect);
    }
    if (req->headers.Connection != NULL){
        free(req->headers.Connection);
    }
    if (req->headers.Content_Length != NULL){
        free(req->headers.Content_Length);
    }
    if (req->headers.Authorization != NULL){
        free(req->headers.Authorization);
    }
    if (req->headers.Cookie != NULL){
        free(req->headers.Cookie);
    }
}
#endif

