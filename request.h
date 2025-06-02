#include <stdio.h>
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
  char* method;
  char* path;
  char* version;
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

Request parseRequest(char* request){
    Request req;
    char bufferHeader[MAX_HEADER_NAME_SIZE];
    char bufferValue[MAX_HEADER_VALUE_SIZE];
    int i = 0;
    int indexValue = 0;
    int indexHeader = 0;
    int value = 0;
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
        if (value == 0){ // check if it cross :
            bufferHeader[indexHeader++] = *request;
        }
        else if(value == 1){
            bufferValue[indexValue++] = *request;
        }

        if (*(request+i) == '\r' && *(request+i+1) == '\n'){
            bufferHeader[indexHeader] = '\0';
            bufferValue[indexValue] = '\0';
            parseField(bufferHeader,  bufferValue, &req);
            indexHeader = 0;
            indexValue = 0;
            value = 0;
            i=0;
            request++;
        }
        request++;

    }
    return req;
    
}
enum HeaderType parseHeader(char* header){
    // printf("Result: %s, %d\n",header, strcmp(header, "\nHost"));
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

