#include <stdio.h>
#include <string.h>
#include "../HashTable/hash.h"
#ifndef REQUEST
#define REQUEST
#define MAX_HEADER_NAME_SIZE 64
#define MAX_HEADER_VALUE_SIZE 4000
typedef struct response_headers{
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
    char* Sec_Fetch_Dest;
} Header;
typedef struct request
{
  char method[10];
  char path[4096];
  char version[15];
  char* body;
  char paramsString[10000];
  Dictionary* parametrs;
  Dictionary* cookies;
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
    SEC_FETCH_DEST,
    UNKNOWN
};
Request parseRequest(char* request);
void parseField(char* header, char* value, Request* request);
enum HeaderType parseHeader(char* header);
void freeRequest(Request* req);
void parseRequestLine(char* ch,int done, Request* request);
void extractParams(Request* request); // extract parametrs from path like ?name="Nikita"
void extractCookies(Request* request); // It's moving data from cookie to hash store
Request parseRequest(char* request){ // parser of income requestes
    Request req = {0};
    req.parametrs = HashTable();
    req.cookies = HashTable();
    req.headers.Cookie = "";
    char bufferHeader[MAX_HEADER_NAME_SIZE];
    char bufferValue[MAX_HEADER_VALUE_SIZE];
    int indexValue = 0;
    int indexHeader = 0;
    int value = 0;
    int first = 1;
    int body = 0;
    while (1)
    {   
        if (*request == '\0'){
            if (indexValue > 0){ // check if body exist, then copy it.
                bufferValue[indexValue] = '\0';
                req.body = strdup(bufferValue);
            }
            break;
        }
        else if (*request == ':' && value == 0)  // check if it cross :
        {
            value = 1;
            request+=2;
        }
        if (*(request) == '\r' && *(request+1) == '\n'){
            value = 1;
            request += 2;
            continue;
        }
        if (value == 0 && first != 1){
            bufferHeader[indexHeader++] = *request;
        }
        else if(value == 1){
            bufferValue[indexValue++] = *request;
        }
        if (first == 1){
            parseRequestLine(request,0,&req);
        }
        if (*(request+1) == '\r' && *(request+2) == '\n'){
            bufferHeader[indexHeader] = '\0';
            bufferValue[indexValue] = '\0';
            parseField(bufferHeader,  bufferValue, &req);
            indexHeader = 0;
            indexValue = 0;
            value = 0;
            if (first == 1){
                parseRequestLine(request,1,&req);
                extractParams(&req);
                first = 0;
            }
            request+=2;
        }
        request++;

    }
    if (strlen(req.headers.Cookie) > 0){
        extractCookies(&req);
    }
    return req;
    
}
void extractCookies(Request* request){
    char* cookies = request->headers.Cookie;
    // count length of value and key below:
    int lengthKey = 0; 
    int lengthValue = 0;
    int indexEnum = 0;
    // variable that indicate state which part of pair is it: key or value; For instance if value is currnect then i will get key=0 and value=1:
    int key = 1; 
    int value = 0;
    // buffer for storing value
    char* keyBuffer = (char*)(malloc(sizeof(char)*1000));
    char* valueBuffer = (char*)(malloc(sizeof(char)*1000));
    while (1)
    {
        if (*cookies == ';'){
            cookies += 2;
            value = 0;
            key = 1;
            if (lengthKey < 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue);
            }
            keyBuffer[lengthKey] = '\0';
            valueBuffer[lengthValue] = '\0';
            indexEnum = 0;
            lengthKey = 0;
            lengthValue = 0;
            Insert(request->cookies,keyBuffer, valueBuffer, "string");
            keyBuffer = (char*)(malloc(sizeof(char)*1000)); // dedicate new part of memmory for comming params
            valueBuffer = (char*)(malloc(sizeof(char)*1000));
            continue;
        }
        else if (*cookies == '\0'){
            if (lengthKey < 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue);
            }
            keyBuffer[lengthKey] = '\0';
            valueBuffer[lengthValue] = '\0';
            Insert(request->cookies,keyBuffer, valueBuffer, "string");
            free(keyBuffer); // free memory in the end of function
            free(valueBuffer);
            break;
        }
        else if (*cookies == '='){
            value = 1;
            key = 0;
            indexEnum = 0;
            cookies++;
            continue;
        }
        if (key == 1){
            lengthKey++;
            keyBuffer[indexEnum++] = *cookies;
            if (lengthKey > 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey+1);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue+1);
            }

        }
        if (value == 1){
            lengthValue++;
            valueBuffer[indexEnum++] = *cookies;
            if (lengthKey > 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey+1);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue+1);
            }
        }
        cookies++;
    }
    
}
void extractParams(Request* request){ /* method for exracting params in url path. 
    For instance: http://localhost:3000/api/search?location=wherever&duration=123, then param is location=wherever */
    char* path = &request->paramsString[0];
    if (strlen(path) <= 0){
        return;
    }
    // variable that indicate state which part of pair is it: key or value; For instance if value is currnect then i will get key=0 and value=1:
    int key = 0; 
    int value = 0;
    // count length of value and key below:
    int lengthKey = 0; 
    int lengthValue = 0;
    int indexEnum = 0;
    // buffer for storing value
    char* keyBuffer = (char*)(malloc(sizeof(char)*1000));
    char* valueBuffer = (char*)(malloc(sizeof(char)*1000));
    while (1)
    {
        if (*path == '\0'){ // if end of parametrs then clean memmory and complete action
            if (lengthKey < 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue);
            }
            keyBuffer[lengthKey] = '\0';
            valueBuffer[lengthValue] = '\0';
            // printf("Values: %s:%s\n",keyBuffer,valueBuffer);
            Insert(request->parametrs,keyBuffer,valueBuffer, "string");
            free(keyBuffer);
            free(valueBuffer);
            break;
        }
        if (*path == '?' && key != 1){
            key = 1;
            path++;
            continue;                        
        }
        else if (*path == '='  && value != 1){
            key = 0;
            value = 1;
            path++;
            indexEnum = 0;
            continue; 
        }
        else if (*path == '&'){ // add key&value pair in hash, and resize length of value pair, if length less than 1000 bytes
            key = 1;
            value = 0;
            indexEnum = 0;
            if (lengthKey < 1000){ // if size less 1000, than i will change size 
                keyBuffer = (char*)realloc(keyBuffer, lengthKey);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue);
            }
            // printf("Lenght: %d\n",lengthKey);
            keyBuffer[lengthKey] = '\0';
            valueBuffer[lengthValue] = '\0';
            Insert(request->parametrs,keyBuffer,valueBuffer, "string");
            keyBuffer = (char*)(malloc(sizeof(char)*1000)); // dedicate new part of memmory for comming params
            valueBuffer = (char*)(malloc(sizeof(char)*1000));
            path++;
            lengthKey = 0;
            lengthValue = 0;
            continue;
        }
        if (key == 1){
            lengthKey++;
            keyBuffer[indexEnum++] = *path;
            if (lengthKey > 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey+1);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue+1);
            }

        }
        if(value == 1){
            lengthValue++;
            valueBuffer[indexEnum++] = *path;
            if (lengthKey > 1000){
                keyBuffer = (char*)realloc(keyBuffer, lengthKey+1);
                valueBuffer = (char*)realloc(valueBuffer, lengthValue+1);
            }
        }
        path++;
    }
    
}
void parseRequestLine(char* ch,int done, Request* request){
    static int index = 0;
    static int i = 0;
    static int params = 0; // check if path consisted a parameters
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
        if (*(ch+1) == ' '){
            request->method[i] = '\0';
        }
    }
    else if (index == 1){
        request->path[i++] = *ch;
        if (*(ch+1) == ' '){
            request->path[i] = '\0';
        }
        else if(*(ch+1) == '?'){
            request->path[i] = '\0';
            index += 1;
            i = 0;
            params = 1;
            return;
        }
    }
    else if (index == 2 && params == 1){
        request->paramsString[i++] = *ch;
        if (*(ch+1) == ' '){
            request->paramsString[i] = '\0';
            params = 0;
        }
    }
    else{
        request->version[i++] = *ch;
        if (*(ch+1) == ' '){
            request->version[i] = '\0';
        }
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
    else if(strcmp(header, "Sec-Fetch-Dest") == 0){
        return SEC_FETCH_DEST;
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
        case SEC_FETCH_DEST:
            request->headers.Sec_Fetch_Dest = strdup(value);
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
    if (req->headers.Sec_Fetch_Dest != NULL){
        free(req->headers.Sec_Fetch_Dest);
    }
    if (req->body != NULL){
        free(req->body);
    }
    if (req->parametrs != NULL){
        free(req->parametrs->hashtable);
        free(req->parametrs);
    }
    if (req->cookies != NULL){
        free(req->cookies->hashtable);
        free(req->cookies);
    }
}
#endif

