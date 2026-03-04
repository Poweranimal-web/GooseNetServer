#include <string.h>
#include <stdlib.h>
#include "../HashTable/hash.h"
#define START_STATEMENT '{'
#define LOOP_COND '%'
#define END_STATEMENT '}'
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
    Token* endToken;
} Token;
typedef struct TokenArray
{
    Token* tokens;
    int length;
    int contentLength;
} TokenArray;
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
            content[length_content+1] = '\0'; 
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
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
            content[length_content+1] = '\0';
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
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
            content[length_content+1] = '\0';
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
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
            content[length_content+1] = '\0';
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
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
    char* new_copy = malloc(sizeof(char)*strlen(name)-4);
    int length = 0;
    while (1)
    {
        if (*copy_name != START_STATEMENT && *copy_name != " "){
            new_copy[length] = *copy_name;
            length++; 
        }
        copy_name++;
    }
    return new_copy;
}
TokenArray analyzeTokens(TokenArray tokens){
    tokens.contentLength = 0;
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        if (token.Type == VAR){
            // char* name = getName(token.content);
            void* value = Get(storageVariables,token.content);
            if (value != ""){
                free(token.content); 
                token.content = value;
                tokens.contentLength += strlen(value);
            }
            else{
                tokens.contentLength += strlen(token.content);
            }

        }
    }
    return tokens;
}
char* returnUpdatedContent(TokenArray tokens){ // return handled string after templating
    char content[tokens.contentLength];
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        strcat(content,token.content);
    }
    return content;
}




