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
typedef struct record
{
    char* key;
    void* value;
} Record;

typedef struct dictionary
{
    int length; // length of hash table
    int count; // counter for get length of items in hash table
    float load_factor;
    HashEntry* hashtable;
} Dictionary;
void Insert(Dictionary* dictionary,char* key,void* value,char* type_value);
Record* Items();
void Remove(Dictionary* dictionary,char* key);
Dictionary* HashTable(int length);

Dictionary* HashTable(int length){
    Dictionary* dictionary = (Dictionary*)malloc(sizeof(Dictionary));
    dictionary->length = length;
    dictionary->count = 0;
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
void rehashing(Dictionary* dictionary){
    int new_length = dictionary->length * 2;
    HashEntry* hashtable = (HashEntry*)malloc(sizeof(HashEntry)*new_length);
    for (int i = 0; i < new_length; i++)
    {
        hashtable[i].key = "";
        hashtable[i].value = "";
        hashtable[i].type_value = "";
        hashtable[i].next_entry = (HashEntry*)malloc(sizeof(HashEntry));
        hashtable[i].next_entry->key = "";
    }
    for (int i = 0; i < dictionary->length; i++)
    {   
        HashEntry* imediated_record = &dictionary->hashtable[i];
        while(1)
        {
            if (strcmp(imediated_record->key,"") != 0){
                    unsigned long hash_key = hash((unsigned char*)imediated_record->key);
                    unsigned long index = hash_key % new_length;
                    if (strcmp(hashtable[index].key, "") != 0){
                        HashEntry* imediated_record2 = &hashtable[index]; // imidiate variable for linked list if colliason still exists
                        while (1)
                        {
                            if (strcmp(imediated_record2->key, "") == 0){
                                imediated_record2->key = strdup(imediated_record->key);
                                imediated_record2->value  = strdup(imediated_record->value);
                                imediated_record2->type_value = strdup(imediated_record->type_value);
                                imediated_record2->next_entry = (HashEntry*)malloc(sizeof(HashEntry));
                                imediated_record2->next_entry->key = "";
                                break;
                            }
                            imediated_record2 = imediated_record2->next_entry;
                        }
                        
                    }
                    else
                    {
                        hashtable[index].key = strdup(imediated_record->key);
                        hashtable[index].value = strdup(imediated_record->value);
                        hashtable[index].type_value = strdup(imediated_record->type_value);
                        hashtable[index].next_entry = (HashEntry*)malloc(sizeof(HashEntry));
                        hashtable[index].next_entry->key = "";
                    }
                                    
            }
            else{
                break;
            }
            imediated_record = imediated_record->next_entry;

        }
    }
    free(dictionary->hashtable);
    dictionary->hashtable = hashtable;
    dictionary->length = new_length;
}
Record* Items(Dictionary* dictionary){ // method for getting all records from hash table
    Dictionary* dict = dictionary;
    Record* items = (Record*)malloc(sizeof(Record)*dict->count);
    int counter = 0;
    for (int i = 0; i < dict->length; i++)
    {
        if (strcmp(dict->hashtable[i].key, "") != 0){
            HashEntry current_element = dict->hashtable[i];
            Record element;
            element.key = current_element.key;
            element.value = current_element.value;
            items[counter] = element;
            counter++;
            while (1)
            {
                current_element = *(current_element.next_entry);
                if (strcmp(current_element.key, "") != 0){
                    Record element;
                    element.key = current_element.key;
                    element.value = current_element.value;
                    items[counter] = element;
                    counter++;
                }
                else{
                    break;
                }
            }
            

        }

    }
    return items;
}
void Insert(Dictionary* dictionary,char* key,void* value,char* type_value){ 
    unsigned long hash_key = hash((unsigned char*)key);
    unsigned long index = hash_key % dictionary->length;
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
    dictionary->count++;
    dictionary->load_factor = ((float)dictionary->count)/((float)dictionary->length);
    if (dictionary->load_factor > 1.0){
        printf("Need rehashing\n");
        rehashing(dictionary);
    }
}
void* Get(Dictionary* dictionary, char* key){
    unsigned long hash_key = hash((unsigned char*)key);
    int index = hash_key % dictionary->length;
    if (COMPARE(dictionary->hashtable[index].key,key) == 0){
        return dictionary->hashtable[index].value;
    }
    else{
        if (dictionary->hashtable[index].key != ""){
            HashEntry* imediated_record = dictionary->hashtable[index].next_entry;
            while (COMPARE(imediated_record->key,"") != 0){
                if (COMPARE(imediated_record->key,key) == 0){
                    return imediated_record->value;
                }
                imediated_record = imediated_record->next_entry;
            }
        }
        return "";
    }
}
void Remove(Dictionary* dictionary,char* key){
    unsigned long hash_key = hash((unsigned char*)key);
    int index = hash_key % dictionary->length;
    if (COMPARE(dictionary->hashtable[index].key,key) != 0){
        HashEntry* current_record = &dictionary->hashtable[index];
        HashEntry* next_record = current_record->next_entry;
        int remove = 0; // state for indicating element which it will being removed
        while (1)
        {
            if (strcmp(current_record->key,"") == 0){ // if nothing is got
                break;
            }
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
    dictionary->count--;
}
char* StringInHashTable(Dictionary* dictionary){
    Record* hashItems = Items(dictionary);
    char* json = (char*)malloc(sizeof(char));
    for (int i = 0; i < dictionary->count; i++)
    {
        if (i == 0){
            char record[strlen(hashItems[i].key)+strlen(hashItems[i].value)+8];
            snprintf(record, sizeof(record), "{'%s':'%s',",hashItems[i].key,hashItems[i].value);
            strcat(json, record);
        }
        else if (i == dictionary->count-1){
            char record[strlen(hashItems[i].key)+strlen(hashItems[i].value)+7];
            snprintf(record, sizeof(record), "'%s':'%s'}",hashItems[i].key,hashItems[i].value);
            strcat(json, record);
        }
        else{
            char record[strlen(hashItems[i].key)+strlen(hashItems[i].value)+7];
            snprintf(record, sizeof(record), "'%s':'%s',",hashItems[i].key,hashItems[i].value);
            strcat(json, record);
        }
    }
    return json;

}
#endif


