// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

typedef struct trienode
{
    struct trienode* children[CHAR_SIZE];
    bool is_word;
}
trienode;

trienode* root;

int words_in_dict = 0; //global variable to determine the number of words in the dictionaryh

void free_node(trienode* node) //recursive way to free all nodes
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            free_node(node->children[i]);
        }
    }
    free(node);
    return;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    trienode* trav = root; //sets traversal pointer to root
    int trie_index; //initializes index inside trie node

    for (int l = 0; word[l] != '\0'; l++)
    {
        if (word[l] == '\'') //sets index for apostrophe
            {
                trie_index = 26;
            }
        else
            {
                trie_index = tolower(word[l]) - 'a'; //sets index for alphabet
            }

        if (trav->children[trie_index] == NULL) //checks if there's a node for that
        {
            return false;
        }
        else
        {
            trav = trav->children[trie_index]; //moves traversal pointer through the children
        }
    }
    if (trav->is_word == true) //checks if it's the end of a dictionary word
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    root = malloc(sizeof(trienode)); //mallocs head/root trie node
    root->is_word = false;

    for (int p = 0; p < CHAR_SIZE; p++)
    {
        root->children[p] = NULL;
    }

    char letters[LENGTH + 1]; //sets array with maximum number of letters in a word +1 for null
    int trie_index; //initializes the index for the children in each trie node

    FILE* dict = fopen(dictionary, "r"); //opens dictionary and checks if it opened succesfully
    if (dict == NULL)
    {
        return false;
    }


    while (fscanf(dict, "%s", letters) != EOF) //scans dictionary file over every word until EOF
    {
        trienode* trav = root; //sets a traversal pointer to the root

        for (int j = 0; letters[j] != '\0'; j++) //iterates over every letter of each word
        {
            if (letters[j] == '\'') //sets index for apostrophe
            {
                trie_index = 26;
            }
            else
            {
                trie_index = tolower(letters[j]) - 'a'; //sets index for alphabet
            }

            if (trav->children[trie_index] == NULL) //checks if there's already a node or not
            {
                trienode* new_node = malloc(sizeof(trienode)); //mallocs new node
                new_node->is_word = false;
                for (int k = 0; k < CHAR_SIZE; k++)
                {
                    new_node->children[k] = NULL;
                }
                trav->children[trie_index] = new_node; //has traversal's children index point to new node
            }
            trav = trav->children[trie_index]; //moves traversal pointer down the children line to next node
        }
        //once the end of a word is reached as specificied by "\0", the is_word bool is set to true in the last children node reached by the word.
        trav->is_word = true;
        words_in_dict++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words_in_dict;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    trienode* trav = root;
    free_node(trav);
    return true;
}
