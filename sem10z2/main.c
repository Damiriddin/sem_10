#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_HASH  10000  // задаем размер хэш таблицы
// структура хэш элемента 
typedef struct HashItem {
    char *key;
    int value;
    struct HashItem *next;
} HashItem;

// хэш функция
unsigned int hashCode(const char *key) {
    unsigned int sum = 0;

    for (int i = 0; key[i]; i++) {
        sum += (unsigned char)key[i] * (i + 1);
    }
    return sum % MAX_HASH;
}
// очистка памяти
void freeItem(HashItem *item) {
    free(item->key);
    free(item);
}

HashItem * getItem(HashItem *table[], const char *key) {
    HashItem **link = &table[hashCode(key)];

    while (*link) {
        HashItem *tmp = *link;
        if (strcmp(tmp->key, key) == 0) {
            return tmp;
        } else {
            link = &(*link)->next;
        }
    }
    return NULL;
}
// удаление города 
void deleteItem(HashItem *table[], const char *key) {
    HashItem **link = &table[hashCode(key)];

    while (*link) {
        HashItem *tmp = *link;
        if (strcmp(tmp->key, key) == 0) {
            *link = tmp->next;  
            freeItem(tmp);
            break;
        } else {
            link = &(*link)->next;
        }
    }
}
// функция добавления города
void insertItem(HashItem *table[], const char *key, const int value) {
    HashItem *item = getItem(table, key);

    if (item == NULL) {
        unsigned int code = hashCode(key);
        HashItem *item = malloc(sizeof(*item));

        if (item != NULL) {
            item->key = strdup(key);
            item->value = value;
            item->next = table[code];
            table[code] = item;
        }
    } else {
        item->value = value;
    }
}
// печать хэш таблицы
void displayHashTable(HashItem *table[]) {
    for (int i = 0; i < MAX_HASH; i++) {
        for (HashItem *tmp = table[i]; tmp; tmp = tmp->next) {
            printf("%s %d\n", tmp->key, tmp->value);
        }
    }
    puts("");
}



int main(void) {
    HashItem *table[MAX_HASH] = { 0 };

    char command, town[51];
    int population;

    while (1){
         puts("\n--- Menu ---\n\
        1 - To Add.\n\
        2 - To Remove.\n\
        3 - The List of Cities.\n\
        4 - Exit.\n");
        printf("Enter your answer: ");
        scanf("%d",&command);
        switch (command) {
            case 1:
                printf("\nEnter the name of city: ");
                scanf("%s", town);
                printf("\nEnter the population of city: ");
                scanf("%i", &population);
                insertItem(table, town, population);
                break;


            case 2:
                printf("Enter the name of deleting city: ");
                scanf("%s", town);
                deleteItem(table, town);
                break;
            case 3: {
                printf("\nThe list cities:\n");
                displayHashTable(table);
                break;
            }
            case 4:
                puts("GoodBye!");
                sleep(1.5);
                exit(0);
            default:
                puts("Unknown command. Try again.");
                break;
        }
    }
    return 0;
}
