#include "Password.h"
#include <iostream>

using namespace std;
using CSC2110::ListArrayIterator;
using CSC2110::ListArray;

Password::Password()
{
   viable_words = new ListArray<String>();
   all_words = new ListArray<String>();
   len = 0;
}

Password::~Password()
{
  delete viable_words;
  delete all_words;
}

int Password::getNumMatches(String* curr_word, String* word_guess)
{
	int char_matches = 0;
	
	for(int i = 0; i<len; i++)
	   {
		   if (curr_word->charAt(i) == word_guess->charAt(i))
			   char_matches++;
	   }
	return char_matches;
}

void Password::addWord(String* word)
{
	//viable_words is a ListArray<String>* variable which has an add function.
	
	if (len == 0) len = word->length();

    if(word->length() == len)
	{
	   viable_words->add(word); 
	   all_words->add(word);
	}
	
	
}

void Password::guess(int try_password, int num_matches)
{
	ListArray<String>* temp = new ListArray<String>(); //Temp ListArray that will contain new viable_words.
	String* word_guessed = getOriginalWord(try_password); //Covert index to String* for getNumMatches()
	
	
	//Need to iterate through each item in viable_words.
	//Used bestGuess as example:
	ListArrayIterator<String>* viable_iter = viable_words->iterator();
    String* curr_word; //Declare ADT outside of loops to prevent constructor overhead.
	
	while(viable_iter->hasNext())
    {
	   
	   curr_word = viable_iter->next(); //Need a variable to hold next() as calling changes the state of iterator
	   
	   
	   //Currently, getNumMatches does not exist yet.
	   if (num_matches == getNumMatches(curr_word, word_guessed))
	      temp->add(curr_word);  //We found a possible password within the list of viable_words. Add this to temp.
	}
	//Delete the iterator (see bestGuess())
	delete viable_iter;
	
	//Delete the old viable_words. This deallocates the memory that holds the actual
	//ListArray, not the pointer itself.
	delete viable_words;
	
	//Now viable_words is a dangling pointer.
	//Give viable_words the address of the ListArray that temp points to.
	viable_words = temp; 
	
	//Let the pointer named temp expire (end of scope). Now viable_words does not have an alias.
}

String* Password::getOriginalWord(int index)
{
   return all_words->get(index);
}

void Password::displayViableWords()
{
   ListArrayIterator<String>* viable_iter = viable_words->iterator();
   while(viable_iter->hasNext())
   {
     viable_iter->next()->displayString();
	 cout << endl;
   }
   delete viable_iter;
}

int Password::getNumberOfPasswordsLeft()
{
   return viable_words->size();
}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}
