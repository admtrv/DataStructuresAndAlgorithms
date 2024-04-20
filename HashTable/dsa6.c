#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 30
#define MAX_SIZE 15000
#define LOAD_FACTOR 0.75
#define LENGTH 16
#define RESIZE_UP 2
#define RESIZE_DOWN 0.5

typedef struct Person {
    char firstName[LENGTH];
    char lastName[LENGTH];
    char birth[LENGTH];
    int euros;
    char cents[2];
    struct Person *next;
} Person;

typedef struct HashTable {
    Person **table;
    int size;
    int count;
} HashTable;

void copyString(char *destination, char *string)
{
    while (*string)
    {
        *destination= *string;
        destination+=1;
        string+=1;
    }
    *destination = '\0';
}

void addString(char *destination, char *string)
{
    while (*destination)
        destination+=1;

    while (*string)
    {
        *destination= *string;
        destination+=1;
        string+=1;
    }

    *destination = '\0';
}

int compareStrings(char *string1, char *string2)
{
    while (*string1 && (*string1 == *string2))
    {
        string1+=1;
        string2+=1;
    }
    return *string1 - *string2;
}

void copyStringN(char *destination, char *string, int n)
{
    if (n == 0) return;

    while (*string && n > 0)
    {
        *destination= *string;
        destination+=1;
        string+=1;
        n-=1;
    }

    *destination = '\0';
}

void copyBlock(char *destination, char *string, int n)
{
    while (n > 0)
    {
        *destination= *string;
        destination+=1;
        string+=1;
        n-=1;
    }
}

unsigned long hashFunctionDjb2(char *string, int tableSize)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *string))
    {
        hash = ((hash << 5) + hash) + c;
        string+=1;
    }
        
    return hash % tableSize;
}

HashTable *createHashTable(int size)
{
    HashTable *hashTable = malloc(sizeof(HashTable));
    hashTable->size = size;
    hashTable->count = 0;
    hashTable->table = malloc(sizeof(Person*) * size);
    for (int i = 0; i < size; i++)
        hashTable->table[i] = NULL;

    return hashTable;
}

void freeList(Person *person)
{
    if (person != NULL) 
    {
        freeList(person->next);
        free(person);
    }
}

void freeHashTable(HashTable *hashTable)
{
    for (int i = 0; i < hashTable->size; i++)
        freeList(hashTable->table[i]);

    free(hashTable->table);
    free(hashTable);
}

Person *createPerson(char *firstName, char *lastName, char *birth, int euros, char cents[2])
{
    Person *newPerson = malloc(sizeof(Person));
    copyStringN(newPerson->firstName, firstName, LENGTH);
    copyStringN(newPerson->lastName, lastName, LENGTH);
    copyStringN(newPerson->birth, birth, LENGTH);
    newPerson->euros = euros;
    copyBlock(newPerson->cents, cents, 2);
    newPerson->next = NULL;
    return newPerson;
}

void createKey(char *key, char *firstName, char *lastName, char *birth)
{
    copyString(key, firstName);
    addString(key, lastName);
    addString(key, birth);
}

void resize(HashTable *hashTable, int newSize) {
    HashTable *newTable = createHashTable(newSize);

    for (int i = 0; i < hashTable->size; i++) 
    {
        Person *person = hashTable->table[i];
        while (person != NULL) 
        {
            Person *nextPerson = person->next;

            char key[LENGTH * 3];
            createKey(key, person->firstName, person->lastName, person->birth);
            unsigned long newIndex = hashFunctionDjb2(key, newSize);

            person->next = newTable->table[newIndex];
            newTable->table[newIndex] = person;

            person = nextPerson;
        }
    }

    free(hashTable->table);
    hashTable->table = newTable->table;
    hashTable->size = newSize;

    newTable->table = NULL;
    free(newTable);
}



void insert(HashTable *hashTable, char *firstName, char *lastName, char *birth, int euros, char cents[2], int *isFirstPrint)
{
    float loadFactor = hashTable->count / (float)hashTable->size;
    if (loadFactor > LOAD_FACTOR && (int)(hashTable->size * RESIZE_UP) < MAX_SIZE) 
        resize(hashTable, (int)(hashTable->size * RESIZE_UP));
    else if (loadFactor < (float)(LOAD_FACTOR / 4) && (int)(hashTable->size* RESIZE_DOWN) > INITIAL_SIZE) 
        resize(hashTable, (int)(hashTable->size * RESIZE_DOWN));

    char key[LENGTH * 3];
    createKey(key, firstName, lastName, birth);

    unsigned long index = hashFunctionDjb2(key, hashTable->size);

    Person *current = hashTable->table[index];
    while (current != NULL)
    {
        if (compareStrings(current->firstName, firstName) == 0 &&
            compareStrings(current->lastName, lastName) == 0 &&
            compareStrings(current->birth, birth) == 0)
        {
            if (*isFirstPrint)
                *isFirstPrint = 0;
            else
                printf("\n");

            printf("insert failed");

            return;
        }
        current = current->next;
    }

    Person *newPerson = createPerson(firstName, lastName, birth, euros, cents);
    newPerson->next = hashTable->table[index];
    hashTable->table[index] = newPerson;
    hashTable->count++;
}

void search(HashTable *hashTable, char *firstName, char *lastName, char *birth, int *isFirstPrint)
{
    char key[LENGTH * 3];
    createKey(key, firstName, lastName, birth);
    unsigned long index = hashFunctionDjb2(key, hashTable->size);

    Person *current = hashTable->table[index];
    while (current != NULL)
    {
        if (compareStrings(current->firstName, firstName) == 0 &&
            compareStrings(current->lastName, lastName) == 0 &&
            compareStrings(current->birth, birth) == 0)
        {
            if (*isFirstPrint)
                *isFirstPrint = 0;
            else
                printf("\n");
            printf("%d,%c%c", current->euros, current->cents[0], current->cents[1]);

            return;
        }
        current = current->next;
    }
    if (*isFirstPrint)
        *isFirstPrint = 0;
    else
        printf("\n");

    printf("search failed");
}

void update(HashTable *hashTable, char *firstName, char *lastName, char *birth, int changeEuros, char changeCents[2], int *isFirstPrint)
{
    char key[LENGTH * 3];
    createKey(key, firstName, lastName, birth);
    unsigned long index = hashFunctionDjb2(key, hashTable->size);

    Person *current = hashTable->table[index];
    while (current)
    {
        if (compareStrings(current->firstName, firstName) == 0 &&
            compareStrings(current->lastName, lastName) == 0 &&
            compareStrings(current->birth, birth) == 0)
        {

            int currentTotalCents = current->euros * 100 + (current->cents[0] - '0') * 10 + (current->cents[1] - '0');
            int changeTotalCents = 0;
            if(changeEuros >= 0)
                changeTotalCents = changeEuros * 100 + (changeCents[0] - '0') * 10 + (changeCents[1] - '0');
            else
                changeTotalCents = changeEuros * 100 - (changeCents[0] - '0') * 10 - (changeCents[1] - '0');

            int newTotalCents = currentTotalCents + changeTotalCents;

            if (newTotalCents < 0)
            {
                if (*isFirstPrint)
                    *isFirstPrint = 0;
                else
                    printf("\n");

                printf("update failed");

                return;
            }

            current->euros = newTotalCents / 100;
            int remainingCents = newTotalCents % 100;
            current->cents[0] = '0' + remainingCents / 10;
            current->cents[1] = '0' + remainingCents % 10;

            return;
        }
        current = current->next;
    }
    if (*isFirstPrint)
        *isFirstPrint = 0;
    else
        printf("\n");

    printf("update failed");

}

void delete(HashTable *hashTable, char *firstName, char *lastName, char *birth, int *isFirstPrint)
{
    char key[LENGTH * 3];
    createKey(key, firstName, lastName, birth);
    unsigned long index = hashFunctionDjb2(key, hashTable->size);

    Person *current = hashTable->table[index];
    Person *previous = NULL;
    while (current != NULL)
    {
        if (compareStrings(current->firstName, firstName) == 0 &&
            compareStrings(current->lastName, lastName) == 0 &&
            compareStrings(current->birth, birth) == 0)
        {
            if (previous == NULL)
                hashTable->table[index] = current->next;
            else
                previous->next = current->next;

            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
    if (*isFirstPrint)
        *isFirstPrint = 0;
    else
        printf("\n");

    printf("delete failed");
}

int main()
{
    HashTable *hashTable = createHashTable(INITIAL_SIZE);
    char firstName[LENGTH];
    char lastName[LENGTH];
    char birth[LENGTH];
    int euros;
    char cents[2];

    int isFirstPrint = 1;

    char command;
    while (scanf("%c", &command) != EOF)
    {
        switch (command)
        {
            case 'i':
                scanf("%s %s %s %d,%c%c", firstName, lastName, birth, &euros, &cents[0], &cents[1]);
                insert(hashTable, firstName, lastName, birth, euros, cents, &isFirstPrint);
                break;
            case 's':
                scanf("%s %s %s", firstName, lastName, birth);
                search(hashTable, firstName, lastName, birth, &isFirstPrint);
                break;
            case 'u':
                scanf("%s %s %s %d,%c%c", firstName, lastName, birth, &euros, &cents[0], &cents[1]);
                update(hashTable, firstName, lastName, birth, euros, cents, &isFirstPrint);
                break;
            case 'd':
                scanf("%s %s %s", firstName, lastName, birth);
                delete(hashTable, firstName, lastName, birth, &isFirstPrint);
                break;
            default:
                break;
        }
    }

    freeHashTable(hashTable);
    return 0;
}