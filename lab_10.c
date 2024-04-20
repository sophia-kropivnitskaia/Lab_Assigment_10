#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    int isWord;
    struct Trie* next[26];
    int count;	
};

struct Trie* createTrie();
void insert(struct Trie* pTrie, char* word);
int numberOfOccurrences(struct Trie* pTrie, char* word);
struct Trie* deallocateTrie(struct Trie* pTrie);
int readDictionary(char* filename, char** pInWords);

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    int len = strlen(word);
    if (0 == len){
        pTrie->isWord = 1;
        return;
    }

    for(int i = 0; i < len; i++){
        int index = word[i]-'a';
        if(current->next[index] == NULL){
            current->next[index] = createTrie();
        }
        current = current->next[index];
    }
    current->isWord = 1;
    current->count++;

}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    int len = strlen(word);
    for(int i = 0; i < len; i++){
        int index = word[i]-'a';
        if(!current->next[index]){
            return 0;
        }
        current = current->next[index];
     }
    if (current != NULL && current->isWord){
        return current->count;
     }
    return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    for(int i  = 0; i < 26; i++){
        if(pTrie->next[i] != NULL){
            deallocateTrie(pTrie->next[i]);
        }
        free(pTrie); 
    }
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* myTree = malloc(sizeof(struct Trie));
    myTree->isWord = 0;
    myTree->count = 0;
    int i;
    for(i = 0; i < 26; i ++){
        myTree->next[i] = NULL;
    }
    return myTree; 
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    int n;
    FILE* ifp = fopen(filename, "r");
    fscanf(ifp, "%d", &n);

    for(int i = 0; i < n; i++){
        pInWords[i] = (char *)malloc(100 * sizeof(char));
        fscanf(ifp, "%s", pInWords[i]);
    }
    fclose(ifp);
    return n;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse line by line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}