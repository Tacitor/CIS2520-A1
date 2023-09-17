#include <stdio.h>
#include <stdlib.h> // for malloc(), free()
#include <string.h>

#include "word_extractor.h"
#include "word_tally.h"

/**
 * A tool to hold the words we care about in an array, and
 * provide a method for processing the file.
 *
 */

// forward declarations
static int updateWordInTallyList(LLNode **wordLists, int maxLen, char *word);

/*
 * Here we do all the work, processing the
 * file and determining what to do for each word as we
 * read it.  Once we are done reading, we can then
 * examine our wordsToCheck list and see what their
 * state is
 */
int tallyWordsInFile(char *filename, LLNode **wordLists, int maxLen)
{
	struct WordExtractor *wordExtractor = NULL;
	char *aWord = NULL;
	// add any more variables that you need

	// create the extractor and open the file
	wordExtractor = weCreateExtractor(filename, maxLen);

	if (wordExtractor == NULL)
	{
		fprintf(stderr, "Failed creating extractor for '%s'\n", filename);
		return 0;
	}

	// loop through the word list heads and set all the heads to null
	for (int i = 0; i <= maxLen; i++)
	{
		wordLists[i] = NULL;
	}

	// read each word from the file using the WordExtractor,
	// and for each tally how often it has been used in
	// the appropriate list

	// In the loop, figure out whether we have seen this
	// word before, and do the appropriate thing if we have

	int totalWordCount = 0;

	while (weHasMoreWords(wordExtractor))
	{
		aWord = weGetNextWord(wordExtractor);
		totalWordCount++;

		// figure out whether we have seen this word before,
		// and do the appropriate thing if we have
		updateWordInTallyList(wordLists, maxLen, aWord);
	}

	// is there anything else you need to do at the end of the loop?
	printf("Total word count %d\n", totalWordCount);

	// Close the file when we are done
	weDeleteExtractor(wordExtractor);

	return 1;
}

/**
 * Either update the tally in the list, or add it to the list
 */
static int
updateWordInTallyList(LLNode **wordListHeads, int maxLen, char *word)
{

	// grab the list of the length of the word
	LLNode *wordList = wordListHeads[strlen(word)];
	// whether or not to keep searching the LL for the current word
	// 1 for true, and to keep searching
	// 0 for false, and that the matching word was found in the wordList
	int doSearchList = 1;

	// traverse the list and see if we find the word in it
	// start at the first node and work down
	while (doSearchList && wordList != NULL)
	{
		// test to see if this is the right word
		if (strcmp(word, wordList->key) == 0)
		{
			// if we have a match stop searching
			doSearchList = 0;
		}
		else
		{
			// move one node over
			wordList = wordList->next;
		}
	}

	if (!doSearchList) // if the word was found in the wordList
	{
		// the wordList was stoped at the matching node
		// so increment the value (number of sightings of the word)
		wordList->value++;
	}
	else
	{
		// check to see if the length of the word is less than maxLen
		// make this check because anything longer than the maxLen will be truncated
		// to the first n=maxLen characters. Since the truncated word is not found
		// if the data file, it would not be accurate in saying whether or not is is
		// a hapax legomenon.
		if (strlen(word) < maxLen)
		{
			// create a new node for the new word. Since it is a new word set the occourence to 1
			// then prepend this new node to the top of the LL
			wordListHeads[strlen(word)] = llPrepend(wordListHeads[strlen(word)], llNewNode(strdup(word), 1));
		}
	}

	/** return success if no error */
	return 1;
}
