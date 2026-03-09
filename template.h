#include <string.h>
#include <stdlib.h>
#include "../HashTable/hash.h"
#define START_STATEMENT '{'
#define LOOP_COND '%'
#define END_STATEMENT '}'
#ifndef TEMPLATE
#define TEMPLATE
enum type{
    TEXT,
    VAR,
    COND,
    END_COND,
    LOOP,
    END_LOOP
};
typedef struct token
{
    char* content;
    enum type Type;
    struct token* endToken;
} Token;
typedef struct TokenArray
{
    Token* tokens;
    int length;
    int contentLength;
} TokenArray;
struct TransformedVariable{
    char* name;
    int length;
};
Dictionary* storageVariables;
void initiateStorageVariables(){
    storageVariables = HashTable(90);
}
int add_variable(char* name, void* value, char* type){
    Insert(storageVariables,name,value,type);
    return 0;
}
TokenArray tokenize(char* temp){
    Token* tokens = (Token*)malloc(sizeof(Token)*1000);
    char* content = (char*)malloc(sizeof(char)*10001);
    int length_content = 0;
    int id_token = 0; // current element in structure
    int in_tag = 0; // check if we are in tag
    int detect_tag = 0; // Purporse of this variable is identifying was defined type of tag in case if it is condtion or loop.
    TokenArray tokenEntity;
    Token* token = (Token*)malloc(sizeof(Token));
    token->Type = TEXT;
    char* temproryText = temp;
    while (1)
    {
        if (*temproryText == START_STATEMENT && *(temproryText+1) == START_STATEMENT){ 
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
            copy_content[length_content] = '\0';
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            token->Type = VAR;
            content = (char*)malloc(sizeof(char)*10001);
            length_content = 0;
            in_tag = 1;
        }
        else if (*temproryText == END_STATEMENT && *(temproryText+1) == END_STATEMENT){
            content[length_content] = *temproryText; /*  adding the last two charecter       */
            length_content += 1;
            content[length_content] = *(temproryText+1);
            length_content += 1;
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
            copy_content[length_content] = '\0';
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            content = (char*)malloc(sizeof(char)*10001);
            length_content = 0;
            in_tag = 0;
            temproryText++;
            temproryText++;
            continue;
        }
        else if (*temproryText == START_STATEMENT && *(temproryText+1) == LOOP_COND){
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content);
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            content = (char*)malloc(sizeof(char)*10001);
            length_content = 0;
            in_tag = 1;
        }
        else if (*temproryText == LOOP_COND && *(temproryText+1) == END_STATEMENT){
            content[length_content] = *temproryText; /*  adding the last two charecter       */
            length_content += 1;
            content[length_content] = *(temproryText+1);
            length_content += 1;
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
            copy_content[length_content] = '\0';
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            content = (char*)malloc(sizeof(char)*10001);
            length_content = 0;
            in_tag = 0;
            detect_tag=0;
            temproryText++;
            temproryText++;
            continue;
        }
        else if ((*temproryText == 'f' && *(temproryText+1) == 'o' && *(temproryText+2) == 'r') && in_tag == 1 && detect_tag==0){ // check if it is loop start with 'for' operator with space or neither
            token->Type = LOOP;
            detect_tag=1;
        }
        else if ((*temproryText == 'i' && *(temproryText+1) == 'f') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = COND;
            detect_tag=1;
        }
        else if ((*temproryText == 'e' && *(temproryText+1) == 'l' && *(temproryText+2) == 'i' && *(temproryText+3) == 'f') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = COND;
            detect_tag=1;
        }
        else if ((*temproryText == 'e' && *(temproryText+1) == 'n' && *(temproryText+2) == 'd' && *(temproryText+3) == 'i' && *(temproryText+4) == 'f') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = END_COND;
            detect_tag=1;
        }
        else if ((*temproryText == 'e' && *(temproryText+1) == 'n' && *(temproryText+2) == 'd' && *(temproryText+3) == 'f' && *(temproryText+4) == 'o' && *(temproryText+5) == 'r') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = END_LOOP;
            detect_tag=1;
        }
        else{
            if (in_tag != 1){
                token->Type = TEXT;
            }
        }
        if (*temproryText == '\0'){
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
            copy_content[length_content] = '\0';
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            break;
        }
        content[length_content] = *temproryText;
        length_content += 1;
        temproryText++;
    }
    tokenEntity.tokens = tokens;
    tokenEntity.length = id_token;
    return tokenEntity;
}
char* getName(char* name){
    char* copy_name = name;
    char* new_copy = (char*)malloc(sizeof(char)*(strlen(name)-4+1));
    int length = 0;
    while (1)
    {
        if (*copy_name == '\0'){
            break;
        }
        else if (*copy_name != START_STATEMENT && *copy_name != END_STATEMENT && *copy_name != ' '){
            new_copy[length] = *copy_name;
            length++;
        }
        copy_name++;
    }
    new_copy[length] = '\0';
    return new_copy;
}
void replace(char* target, char* new_value){
    int i=0;
    for (i; i < strlen(new_value); i++)
    {
        target[i] = new_value[i];
    }
}
TokenArray analyzeTokens(TokenArray tokens){
    tokens.contentLength = 0;
    TokenArray copy_tokens = tokens;
    for (int i = 0; i < copy_tokens.length; i++)
    {
        Token token = copy_tokens.tokens[i];      
        // printf("Text: %s, Length: %d, \n", token.content, strlen(token.content));
        if (token.Type == VAR){ 
            char* name = getName(token.content);
            char* value = (char*)Get(storageVariables, name);
            if (strcmp(value, "") != 0){
                replace(token.content, value);
                token.content[strlen(value)] = '\0';
                copy_tokens.contentLength += strlen(token.content);
            }
            else{
                copy_tokens.contentLength += strlen(token.content);
            }
        }
        // else if (token.Type == LOOP)
        // {
            
        // }
        
        else{
            // printf("Text: %s, Length: %d, \n", token.content, strlen(token.content));
            copy_tokens.contentLength += strlen(token.content);
        }
    }
    return copy_tokens;
}
int freeTokens(TokenArray tokens){
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        free((&token)->content);
    }
    free(tokens.tokens);
    return 0;

}
char* returnUpdatedContent(TokenArray tokens){ // return handled string after templating
    char* content = (char*)malloc(sizeof(char)*tokens.contentLength+1);
    int length = 0;
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        if (i == 0){
            strcpy(content,token.content);
            length += strlen(token.content);
        }
        else{
            strcat(content,token.content);
            length += strlen(token.content);
        }
    }
    content[tokens.contentLength] = '\0';
    freeTokens(tokens);
    return content;
}
#endif




