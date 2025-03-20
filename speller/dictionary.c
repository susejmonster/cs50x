// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];
int dict_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *dict_node = table[index];
    while (dict_node != NULL)
    {
        if (strcasecmp(word, dict_node->word) == 0)
        {
            return true;
        }
        dict_node = dict_node->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //  Hash function that uses a sum of ASCII values
    int sum = 0;
    int word_len = strlen(word);
    for (int i = 0; i < word_len; i++)
    {
        sum += tolower(word[i]);
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict_file = fopen(dictionary, "r");

    if (dict_file == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }

    char dict_word[LENGTH + 1];

    while (fscanf(dict_file, "%s", dict_word) != EOF)
    {
        node *dict_node = malloc(sizeof(node));

        if (dict_node == NULL)
        {
            return false;
        }

        strcpy(dict_node->word, dict_word);

        int index = hash(dict_word);
        dict_node->next = table[index];
        table[index] = dict_node;

        dict_size++;
    }

    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *dict_node = table[i];
        while (dict_node != NULL)
        {
            node *prev_node = dict_node;
            dict_node = dict_node->next;
            free(prev_node);
        }
        if (dict_node == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
