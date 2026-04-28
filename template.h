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
    int length; // getting text content length
    enum type Type;
    int total_elements; // for loop shows how many elements
    int isTrueCond; // to identify is true condtion 
    struct token* nextToken;
    struct token* lastToken;
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
struct paramsLoop{
    char* nameList;
    char* nameItaretedVariable;
};
struct paramsCond{
    char* nameVariable;
    char* valueEql;
};
typedef struct element{
    void* value;
    char* type;
} ArrayElement;

typedef struct array_elements{
    ArrayElement* array;
    int length;
} TemplateElements;
Dictionary* storageVariables;
int Initiated = 0;
void initiateStorageVariables(){
    if (Initiated != 1){
        storageVariables = HashTable(90);
        Initiated = 1;
    }
}
int add_variable(char* name, void* value, char* type){
    if (Initiated == 0){
        initiateStorageVariables();
    }
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
    token->nextToken = NULL;
    token->lastToken = NULL;
    char* temproryText = temp;
    while (1)
    {
        if (*temproryText == START_STATEMENT && *(temproryText+1) == START_STATEMENT){ 
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content+1);
            copy_content[length_content] = '\0';
            token->length = length_content;
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            token->Type = VAR;
            token->nextToken = NULL;
            token->lastToken = NULL;
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
            token->length = length_content;
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            token->nextToken = NULL;
            token->lastToken = NULL;
            content = (char*)malloc(sizeof(char)*10001);
            length_content = 0;
            in_tag = 0;
            temproryText++;
            temproryText++;
            continue;
        }
        else if (*temproryText == START_STATEMENT && *(temproryText+1) == LOOP_COND){
            char* copy_content = (char*)realloc(content, sizeof(char)*length_content);
            copy_content[length_content] = '\0';
            token->length = length_content;
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            token->nextToken = NULL;
            token->lastToken = NULL;
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
            token->length = length_content;
            token->content = copy_content;
            tokens[id_token] = *token;
            id_token +=1;
            token = (Token*)malloc(sizeof(Token));
            token->nextToken = NULL;
            token->lastToken = NULL;
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
            token->isTrueCond = 0;
            detect_tag=1;
        }
        else if ((*temproryText == 'e' && *(temproryText+1) == 'l' && *(temproryText+2) == 'i' && *(temproryText+3) == 'f') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = COND;
            token->isTrueCond = 0;
            detect_tag=1;
        }
        else if ((*temproryText == 'e' && *(temproryText+1) == 'n' && *(temproryText+2) == 'd' && *(temproryText+3) == 'i' && *(temproryText+4) == 'f') && in_tag == 1 && detect_tag==0){ // check if it is condtion start with 'if' operator with space or neither
            token->Type = END_COND;
            token->isTrueCond = 0;
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
            token->length = length_content;
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
struct paramsLoop getParams(char* loopToken){ // method for getting name of variable where data is storing in loop
    char* copyToken = loopToken;
    struct paramsLoop params;
    int forBlock = 0;
    int inBlock = 0;
    char* buffer = (char*)malloc(sizeof(char)*1000);
    int bufferLength = 0;
    for (int i = 0; i < strlen(copyToken); i++)
    {
        if (copyToken[i] != START_STATEMENT && copyToken[i] != END_STATEMENT && copyToken[i] != LOOP_COND && copyToken[i] != ' '){
            buffer[bufferLength] = copyToken[i];
            bufferLength++;
        }
        else if (copyToken[i] == ' ' && bufferLength != 0){
            buffer[bufferLength] = '\0';
            if (strcmp(buffer,"for") == 0){
                forBlock = 1;
                free(buffer);
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                continue;
            }
            else if (strcmp(buffer,"in") == 0){
                inBlock = 1;
                free(buffer);
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                continue;
            }
            if (forBlock == 1){
                buffer = (char*)realloc(buffer, sizeof(char)*bufferLength+1);
                params.nameItaretedVariable = buffer;
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                forBlock = 0;
                continue;
            }
            if (inBlock == 1){
                buffer = realloc(buffer, sizeof(char)*bufferLength+1);
                params.nameList = buffer;
                inBlock = 0;
                bufferLength = 0;
                continue;
            }   
        }
    }
    if (inBlock != 0){
        buffer = realloc(buffer, sizeof(char)*bufferLength);
        params.nameList = buffer;
    }
    return params;
}
struct paramsCond extractCondition(char* content){
    char* copyToken = content;
    struct paramsCond params;
    int ifBlock = 0;
    int equelBlock = 0;
    char* buffer = (char*)malloc(sizeof(char)*1000);
    int bufferLength = 0;
    for (int i = 0; i < strlen(copyToken); i++)
    {
        if (copyToken[i] != START_STATEMENT && copyToken[i] != END_STATEMENT && copyToken[i] != LOOP_COND && copyToken[i] != ' '){
            buffer[bufferLength] = copyToken[i];
            bufferLength++;
        }
        else if (copyToken[i] == ' ' && bufferLength != 0){
            buffer[bufferLength] = '\0';
            if (strcmp(buffer,"if") == 0 || strcmp(buffer,"elif") == 0){
                ifBlock = 1;
                free(buffer);
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                continue;
            }
            else if (strcmp(buffer,"==") == 0){
                equelBlock = 1;
                free(buffer);
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                continue;
            }
            if (ifBlock == 1){
                buffer = (char*)realloc(buffer, sizeof(char)*bufferLength+1);
                params.nameVariable = buffer;
                buffer = (char*)malloc(sizeof(char)*1000);
                bufferLength = 0;
                ifBlock = 0;
                continue;
            }
            if (equelBlock == 1){
                buffer = realloc(buffer, sizeof(char)*bufferLength+1);
                params.valueEql = buffer;
                equelBlock = 0;
                bufferLength = 0;
                continue;
            }   
        }
    }
    if (equelBlock != 0){
        buffer = realloc(buffer, sizeof(char)*bufferLength);
        params.valueEql = buffer;
    }
    return params;
}
void handleLoop(Token loopToken,TokenArray* tokens, int* index){
    struct paramsLoop params = getParams(loopToken.content);
    TemplateElements* list = (TemplateElements*)Get(storageVariables,params.nameList);
    ArrayElement* list_values = list->array;
    int TotalLengthOfElement = 0;
    Token* token = &(tokens->tokens[*index]);
    Token* token_referenceLast = NULL;
    for (int i = 0; i < list->length;i++){
        add_variable(params.nameItaretedVariable, list_values[i].value, "string");
        for (int j = *index; j < tokens->length; j++)
        {   
            Token next_token = tokens->tokens[j];
            if (i == 0){
                TotalLengthOfElement++;
            }
            if (next_token.Type == VAR){
                struct token* new_instance = (struct token*)malloc(sizeof(struct token));
                char* name = getName(next_token.content); // getting name of variable
                char* value = (char*)Get(storageVariables, name);// getting value of variable
                if (strcmp(value, "") != 0){ // if value exists, It applies on the new object
                    new_instance->content = value;
                    tokens->contentLength += strlen(value);
                }
                else{  // if value doesn't exist, It applies an old content on the new object
                    new_instance->content = next_token.content;
                    tokens->contentLength += strlen(next_token.content);
                }
                /* put instance in linked list, bottom code: */
                new_instance->Type = VAR;
                new_instance->nextToken = NULL;
                if (token->nextToken == NULL){
                    token->nextToken = new_instance;
                    token_referenceLast = new_instance;
                }
                else{
                    token_referenceLast->nextToken = new_instance;
                    token_referenceLast = new_instance;
                }
            }
            else if (next_token.Type == TEXT){
                struct token* new_instance = (struct token*)malloc(sizeof(struct token));
                new_instance->content = next_token.content;
                new_instance->Type = TEXT;
                new_instance->nextToken = NULL;
                if (token->nextToken == NULL){
                    token->nextToken = new_instance;
                    token_referenceLast = new_instance;
                }
                else{
                    token_referenceLast->nextToken = new_instance;
                    token_referenceLast = new_instance;

                }
                tokens->contentLength += next_token.length;
            }
            else if (next_token.Type == END_LOOP){
                break;
            }
        }
    }
    token->total_elements = TotalLengthOfElement;
    *index = (*index) + TotalLengthOfElement;
}
void handleVar(Token* tokenVar, TokenArray* tokens, int* index){
    char* name = getName(tokenVar->content);
    char* value = (char*)Get(storageVariables, name);
    if (*index == 0){
        if (strcmp(value, "") != 0){
            replace(tokenVar->content, value);
            tokenVar->content[strlen(value)] = '\0';
            tokens->contentLength += strlen(tokenVar->content);
        }
        else{
            tokens->contentLength += strlen(tokenVar->content);
        }
    }
    else{

    }
}
void handleCondion(Token loopToken,TokenArray* tokens, int* index){
    Token* token = &(tokens->tokens[*index]); // for storing orginal version of condtional token
    int endCond = 0;
    while (endCond != 1)
    {
        if (token->Type == COND){
                struct paramsCond params = extractCondition(token->content);
                char* value = (char*)Get(storageVariables, params.nameVariable);
                if (strcmp(value, params.valueEql) == 0){
                    token->isTrueCond = 1;
                    Token* token_referenceLast = NULL;
                    int counter = 0;
                    int j = (*index)+1;
                    for (j; j < tokens->length; j++)
                    {
                        Token* new_element = &tokens->tokens[j];
                        counter++;
                        if (new_element->Type == TEXT){
                            if (token->nextToken == NULL){
                                token->nextToken = new_element;
                                token_referenceLast = new_element;
                            }
                            else{
                                token_referenceLast->nextToken = new_element;
                                token_referenceLast = new_element;
                            }
                            tokens->contentLength += new_element->length;
                        }
                        else if (new_element->Type == VAR){
                            char* name = getName(new_element->content); // getting name of variable
                            char* value = (char*)Get(storageVariables, name);// getting value of variable
                            if (strcmp(value, "") != 0){ // if value exists, It applies on the new object
                                new_element->content = value;
                                tokens->contentLength += strlen(value);
                            }
                            else{  // if value doesn't exist, It applies an old content on the new object
                                tokens->contentLength += strlen(new_element->content);
                            }
                            if (token->nextToken == NULL){
                                token->nextToken = new_element;
                                token_referenceLast = new_element;
                            }
                            else{
                                token_referenceLast->nextToken = new_element;
                                token_referenceLast = new_element;
                            }

                        }
                        else if (new_element->Type == COND){
                            token->total_elements = counter;
                            *index = j-1;
                            break;
                        }
                        else if (new_element->Type == END_COND){
                            token->total_elements = counter;
                            *index = j;
                            endCond = 1;
                            break;
                        }
                    }
            }
            else{
                token->isTrueCond = 0;
                int counter = 0;
                int j = (*index)+1;
                for (j; j < tokens->length; j++)
                    {
                        Token* new_element = &tokens->tokens[j];
                        counter++;
                        if (new_element->Type == COND){
                            token->total_elements = counter;
                            *index = j-1;
                            break;
                        }
                        else if (new_element->Type == END_COND){
                            token->total_elements = counter;
                            *index = j;
                            endCond = 1;
                            break;
                        }
                    }

            }
            
        }        
        (*index)++;
        token = &(tokens->tokens[*index]);
                
    }
}
void printTokens(TokenArray tokens){
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        printf("Token %d, Content: %s\n",i,token.content);
    } 
}
TokenArray analyzeTokens(TokenArray tokens){
    tokens.contentLength = 0;
    TokenArray copy_tokens = tokens;
    for (int i = 0; i < copy_tokens.length; i++)
    {
        Token token = copy_tokens.tokens[i];
        if (token.Type == VAR){ 
            char* name = getName(token.content);
            char* value = (char*)Get(storageVariables, name);
            if (strcmp(value, "") != 0){
                token.content = (char*)realloc(token.content,sizeof(char)*(strlen(value)+1));
                replace(token.content, value);
                token.content[strlen(value)] = '\0';
                copy_tokens.contentLength += strlen(token.content);
            }
            else{
                copy_tokens.contentLength += strlen(token.content);
            }
        }
        else if (token.Type == LOOP)
        {
            handleLoop(token,&copy_tokens,&i);
        }
        else if (token.Type == COND){
            handleCondion(token,&copy_tokens,&i);
        }
        else{
            copy_tokens.contentLength += token.length;
            
        }
    }
    printTokens(copy_tokens);
    return copy_tokens;
}
int freeTokens(TokenArray tokens){
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        if ((&token)->content != NULL){
            free((&token)->content);
        }
    }
    if (tokens.tokens != NULL){
        free(tokens.tokens);
    }
    return 0;

}
char* returnUpdatedContent(TokenArray tokens){ // return handled string after templating
    char* content = (char*)malloc(sizeof(char)*(tokens.contentLength+100));
    for (int i = 0; i < tokens.length; i++)
    {
        Token token = tokens.tokens[i];
        if (i == 0){
            strcpy(content,token.content);
        }
        else{
            if (token.Type == LOOP){
                Token* current_element = token.nextToken;
                while (current_element != NULL)
                {
                    strcat(content,current_element->content);
                    current_element = current_element->nextToken;  
                }
                i = i + token.total_elements;
                continue;
            }
            else if (token.Type == COND && token.isTrueCond == 1){
                Token* current_element = token.nextToken;
                while (current_element != NULL)
                {
                    strcat(content,current_element->content);
                    current_element = current_element->nextToken;  
                }
                i = i + (token.total_elements-1);
                continue;
            }
            else if (token.Type == COND && token.isTrueCond == 0){
                i = i + (token.total_elements-1);
                continue;
            }
            else if (token.Type != END_LOOP && token.Type != END_COND){
                if (token.content != NULL){
                    strcat(content,token.content);
                }
                
            }
        }
    }
    content[tokens.contentLength] = '\0';
    freeTokens(tokens);
    return content;
}
#endif




