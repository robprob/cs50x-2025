// Original program provided by CS50

// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

int dictSize = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 456976; // 26^4

// Precompute hash multipliers (square roots) to prevent redundant operations
const unsigned int multipliers[4] = {17576, 676, 26, 1};

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // Convert to lowercase, preventing use of tolower() in multiple other locations
    char lowerWord[LENGTH + 1];
    strcpy(lowerWord, word);

    for (int i = 0; lowerWord[i] != '\0'; i++)
    {
        lowerWord[i] = tolower(lowerWord[i]);
    }

    // Calculate hash index for word
    int hashIndex = hash(lowerWord);

    // Iterate through hash table/linked list, attempting to find a match
    for (node *ptr = table[hashIndex]; ptr != NULL; ptr = ptr->next)
    {
        // Perform case-sensitive comparison
        if (strcmp(ptr->word, lowerWord) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    unsigned int hashIndex = 0;

    for (int i = 0; i < 4 && word[i] != '\0'; i++)
    {
        char c = word[i];

        // Apostrophe (39) acts like 'z'
        if (c == 39)
        {
            c = 122;
        }

        // Add place in alphabet * square root table size every iteration
        // This creates a "spot" for each permutation of 3 letters, such as "cat"
        hashIndex += (c - 'a') * multipliers[i];
    }

    return hashIndex;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Open dictionary file in read mode
    FILE *dictFile = fopen(dictionary, "r");

    // Read lines (words) from dictionary
    char word[LENGTH + 1];

    // Use fscan to read entire words at a time
    while (fscanf(dictFile, "%s", word) != EOF)
    {
        // Create new hash node
        node *n = malloc(sizeof(node));

        // Copy dictionary word into node
        strcpy(n->word, word);

        // Hash word to determine bucket index
        unsigned int hashIndex = hash(n->word);

        n->next = table[hashIndex]; // Link to existing node or NULL
        table[hashIndex] = n;       // Connect node to head of linked list

        // Increment global size variable
        dictSize++;
    }

    // Close dictionary file
    fclose(dictFile);

    // Communicate load confirmation
    printf("Dictionary loaded, all good!\n");
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO

    return dictSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // Iterate length of hash table
    for (int i = 0; i < N; i++)
    {
        // Iterate pointers, freeing memory
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }

    return true;
}
