#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef HASHTABLE
#define HASHTABLE
int COMPARE(char* input_key,char* key){
    return input_key != NULL ? strcmp(input_key,key) : -1;
}
typedef struct hash_entry
{
    char* key;
    void* value;
    char* type_value;
    struct hash_entry* next_entry;
} HashEntry;
typedef struct dictionary
{
    int length;
    HashEntry* hashtable;
} Dictionary;
void Insert(Dictionary* dictionary,char* key,void* value,char* type_value);
void Remove(Dictionary* dictionary,char* key);
Dictionary* HashTable();

Dictionary* HashTable(){
    Dictionary* dictionary = (Dictionary*)malloc(sizeof(Dictionary));
    dictionary->length = 8;
    HashEntry* hashtable = (HashEntry*)malloc(sizeof(HashEntry)*8);
    for (int i = 0; i < dictionary->length ; i++)
    {
        hashtable[i].key = "";
        hashtable[i].value = "";
        hashtable[i].type_value = "";
        hashtable[i].next_entry = (HashEntry*)malloc(sizeof(HashEntry));
        hashtable[i].next_entry->key = "";
    }
    dictionary->hashtable = hashtable;
    return dictionary;
}
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
void Insert(Dictionary* dictionary,char* key,void* value,char* type_value){ 
    unsigned long hash_key = hash((unsigned char*)key);
    unsigned long index = hash_key % dictionary->length;
    // printf("Record: %s:%d\n", key, index);
    if (strcmp(dictionary->hashtable[index].key, "") != 0){
        HashEntry* imediated_record = &dictionary->hashtable[index];
        while (1){
            if (strcmp(imediated_record->key, "") == 0){
                imediated_record->key = strdup(key);
                imediated_record->value  = strdup(value);
                imediated_record->type_value = strdup(type_value);
                imediated_record->next_entry = (HashEntry*)malloc(sizeof(HashEntry));
                imediated_record->next_entry->key = "";
                break;
            }
            else if(strcmp(imediated_record->key, key) == 0){
                imediated_record->value = strdup(value);
                imediated_record->type_value = strdup(type_value);
            }
            imediated_record = imediated_record->next_entry;
        }
    }
    else{
        // printf("Work1\n");
        // printf("%s:%s\n",key,(char*)value);
        dictionary->hashtable[index].key = strdup(key);
        dictionary->hashtable[index].value = strdup(value);
        dictionary->hashtable[index].type_value = strdup(type_value);
        dictionary->hashtable[index].next_entry = (HashEntry*)malloc(sizeof(HashEntry));
        dictionary->hashtable[index].next_entry->key = "";
    }
}
void* Get(Dictionary* dictionary, char* key){
    unsigned long hash_key = hash((unsigned char*)key);
    int index = hash_key % dictionary->length;
    if (COMPARE(dictionary->hashtable[index].key,key) == 0){
        return dictionary->hashtable[index].value;
    }
    else{
        HashEntry* imediated_record = dictionary->hashtable[index].next_entry;
        while (dictionary->hashtable[index].key != NULL && COMPARE(imediated_record->key,"") != 0)
        {
            if (COMPARE(imediated_record->key,key) == 0){
                return imediated_record->value;
            }
            imediated_record = imediated_record->next_entry;
            
        }
        return NULL;
    }
}
void Remove(Dictionary* dictionary,char* key){
    unsigned long hash_key = hash((unsigned char*)key);
    int index = hash_key % dictionary->length;
    if (COMPARE(dictionary->hashtable[index].key,key) != 0){
        HashEntry* current_record = &dictionary->hashtable[index];
        HashEntry* next_record = current_record->next_entry;
        int remove = 0;
        while (1)
        {
            if (COMPARE(current_record->key,key) == 0 && remove == 0){
                remove = 1;
                continue;
            }
            else if(COMPARE(current_record->key,key) != 0 && remove == 0){
                current_record = next_record;
                next_record = current_record->next_entry;
                continue;
            }
            if (strcmp(next_record->key,"") == 0 && remove == 1){
                current_record->key = "";
                free(next_record);
                break;
            }
            if (strcmp(next_record->key,"") != 0 && remove == 1){
                // printf("Replace %s on this %s\n", current_record->key, next_record->key);
                current_record->key = next_record->key;
                current_record->value = next_record->value;
                current_record->type_value = next_record->type_value;
                current_record = next_record;
                next_record = current_record->next_entry;
                continue;
            }
        }
        
    }
    else{
        HashEntry* current_record = &dictionary->hashtable[index];
        HashEntry* next_record = dictionary->hashtable[index].next_entry;
        while (1)
        {
            if (strcmp(next_record->key,"") == 0){
                current_record->key = "";
                free(next_record);
                break;
            }
            // Replace previous one to next record instead.
            current_record->key = next_record->key;
            current_record->value = next_record->value;
            current_record->type_value = next_record->type_value;
            current_record = next_record;
            next_record = next_record->next_entry;

        }
        
    }
}
#endif


