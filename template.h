#include <string.h>
#include <stdlib.h>
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
} Token;
typedef struct TokenArray
{
    Token* tokens;
    int length;
} TokenArray;


TokenArray tokenize(char* temp){
    Token* tokens = (Token*)malloc(sizeof(Token)*1000);
    char* content = (char*)malloc(sizeof(char)*10001);
    int length_content = 0;
    int id_token = 0;
    int in_tag = 0;
    TokenArray tokenEntity;
    Token* token = (Token*)malloc(sizeof(Token));
    token->Type = TEXT;
    char* temproryText = temp;
    while (*temproryText != '\0')
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
            temproryText++;
            temproryText++;
            continue;
        }
        else if (((*temproryText == 'f' && *(temproryText+1) == 'o' && *(temproryText+2) == 'r') || (*temproryText == ' ' && *(temproryText+1) == 'f' && *(temproryText+2) == 'o' && *(temproryText+3) == 'r' )) && in_tag == 1){ // check if it is loop start with 'for' operator with space or neither
            token->Type = LOOP;
        }
        else if (((*temproryText == 'i' && *(temproryText+1) == 'f') || (*temproryText == ' ' && *(temproryText+1) == 'i' && *(temproryText+2) == 'f' )) && in_tag == 1){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = COND;
        }
        else if (((*temproryText == 'e' && *(temproryText+1) == 'l' && *(temproryText+2) == 'i' && *(temproryText+3) == 'f') || (*temproryText == ' ' && *(temproryText+1) == 'e' && *(temproryText+2) == 'l' && *(temproryText+3) == 'i' && *(temproryText+4) == 'f' )) && in_tag == 1){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = COND;
        }
        else if (((*temproryText == 'e' && *(temproryText+1) == 'n' && *(temproryText+2) == 'd' && *(temproryText+3) == 'i' && *(temproryText+4) == 'f') || (*temproryText == ' ' && *(temproryText+1) == 'e' && *(temproryText+2) == 'n' && *(temproryText+3) == 'd' && *(temproryText+4) == 'i' && *(temproryText+5) == 'f')) && in_tag == 1){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = END_COND;
        }
        else if (((*temproryText == 'e' && *(temproryText+1) == 'n' && *(temproryText+2) == 'd' && *(temproryText+3) == 'f' && *(temproryText+4) == 'o' && *(temproryText+5) == 'r') || (*temproryText == ' ' && *(temproryText+1) == 'e' && *(temproryText+2) == 'n' && *(temproryText+3) == 'd' && *(temproryText+4) == 'f' && *(temproryText+5) == 'o' && *(temproryText+6) == 'r')) && in_tag == 1){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = END_LOOP;
        }
        else{
            if (in_tag != 1){
                token->Type = TEXT;
            }
        }
        content[length_content] = *temproryText;
        length_content += 1;
        temproryText++;
    }
    tokenEntity.tokens = tokens;
    tokenEntity.length = id_token;
    return tokenEntity;

}



