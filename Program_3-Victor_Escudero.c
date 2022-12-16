#include <stdio.h>
#include <stdlib.h>// for exit( -1)
#include <string.h>//For string functions
/* ---------------------------------------------
Program 3: Best Wordle Word

Course: CS 211, Fall 2022. Tuesday 8am lab
System: Mac using Replit
Author: Victor S Escudero
---------------------------------------------
*/

//2. Struct that stores the score and word together
struct wordCountStruct
{
  char word[6];
  int score;
};

//wordStruct is a synonym of struct Words
typedef struct wordCountStruct wordStruct;

//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordStruct *) a)->score;
    int secondScore = ((wordStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
      // Hint: return the value of strcmp( parameter a word,  parameter b word)
        return strcmp(((wordStruct *) a)->word, ((wordStruct *) b)->word);
    }
} //end compareFunction(..)


    // ...
    // Sort the allWords array in descending order by score, and within score they should also
    // be sorted into ascending order alphabetically.  Use the built-in C quick sort qsort(...).
    // The last parameter is the name of the comparison function we use (a function pointer).
    //
    // ... uncomment out the line below once you have it in a meaningful context in your code.
    //qsort( theArray, numElementsInArray, sizeof(wordStruct), compareFunction);
// -----------------------------------------------------------------------------------------
void getWordCount(char fileName[], int *count)
{
  char extractedWord[100];
  // Connect logical name to filename
  FILE *inFilePtr  = fopen(fileName, "r");
  
  // Ensure file open worked correctly
  if( inFilePtr == NULL ) {
    printf("Error: could not open %s for reading\n", fileName);
    exit(-1);    // must include stdlib.h
  }
  
  // Read from file.
  while( fscanf( inFilePtr, "%s", extractedWord) != EOF) {
    (*count)++;
  }
  
  // Close the file
  fclose( inFilePtr);
}
//---------------------------------------------------------
void extractWords(char fileName[], wordStruct* *pArray, int startIndex)
{
  char extractedWord[100];
  
  // Connect logical name to filename
  FILE *inFilePtr  = fopen(fileName, "r");

  // Ensure file open worked correctly
  if( inFilePtr == NULL ){
    printf("Error: could not open %s for reading\n", fileName);
    exit(-1);    // must include stdlib.h
  }

  // Read from file and extract word by word
  while( fscanf( inFilePtr, "%s", extractedWord) != EOF)
  {
    strcpy((*pArray)[startIndex].word, extractedWord);// copies word to the struct;
    (*pArray)[startIndex].score = 0;
    startIndex++;
  }
  
  // Close the file
  fclose( inFilePtr);
}
//---------------------------------------------------------
//scoreCalculator takes 2 words, one from answer words and one from all words. It compares the guess word with the asnwer word and calculates score. 3 points for same letter same position and 1 point for same letter in word but wrong position
int scoreCalculator(char answer[], char wordToCompare[])
{
  int score = 0;// score of word

  //we make copies of the words we want to compare
  char * ansWord = malloc(6*sizeof(char));
  char * compareWord = malloc(6*sizeof(char));

  strcpy(ansWord, answer);//copy answer word
  strcpy(compareWord, wordToCompare);//copy guess word

  //First pass to check same position and same letter.
  for(int i = 0; i < 5; i++)
  {
    if(ansWord[i] == compareWord[i])
    {
      score = score + 3;//3 points for same character in the same position.
      //blank out both letters from both words so we don't give extra points.
      ansWord[i] = '+';
      compareWord[i] = '-';
    }
  }
  //second pass to score 1 point if letter is in the word but not in the correct spot
  for(int i = 0; i< 5; i++)
  {
    for(int j = 0; j< 5; j++)
    {
      if(ansWord[i] == compareWord[j])
      {
        score = score + 1;//1 point for same letter but wrong position
        //blank out both letters from both words so we don't give extra points.
        ansWord[i] = '+';
        compareWord[j] = '-';
      }
    }
  }

  //Free dynamically allocated c strings
  free(ansWord);
  free(compareWord);
  
  return score;//return total score for one word
}
//---------------------------------------------------------
//scoreFunc takes in the array of aswers of type struct word, takes the total word array of type struct word, and the size of the total and answer array. The function calculates the score for each word
void bestFirstWordsCalculator(wordStruct * pArray, wordStruct * pAnswers, int sizeOfTotalWords, int sizeOfAnswerWords)
{
  wordStruct * pAnswersCopy = (wordStruct*)malloc(sizeOfAnswerWords*sizeof(wordStruct));//copy of answers array
  wordStruct * pArrayCopy = (wordStruct*)malloc(sizeOfTotalWords*sizeof(wordStruct));//copy of total words array
  int score = 0;//score

  //copying answers array
  for(int i = 0; i < sizeOfAnswerWords; i++)
  {
    strcpy(pAnswersCopy[i].word, pAnswers[i].word);
    pAnswersCopy[i].score = pAnswers[i].score;
  }
  //copying total words array
  for(int i = 0; i < sizeOfTotalWords; i++)
  {
    strcpy(pArrayCopy[i].word, pArray[i].word);
    pArrayCopy[i].score = pArray[i].score;
  }

  //Compare words with all answers words from array and get score for each word
  for(int i = 0; i < sizeOfTotalWords; i++)// size of total word array
  {
    for(int j = 0; j< sizeOfAnswerWords; j++)//size of answer array
    {
      score = score + scoreCalculator(pAnswersCopy[j].word, pArrayCopy[i].word);//Let score calculator function deal with scoring
      pArrayCopy[i].score = score;// assign total score for word
    }
    score = 0;//reset score after it has compared one word with all answer words
  }

  //copy scores from copy array to original array
  for(int i = 0; i < sizeOfTotalWords; i++)
  {
    pArray[i].score = pArrayCopy[i].score;
  }
  
  //free array copies
  free(pAnswersCopy);
  free(pArrayCopy);
}
//---------------------------------------------------------
//This functions takes by refernce a c string of answer and by value a c strng to compare to answer word. The function will blank out the answer words in relation to the word we are comparing with for use in best second word calculator
void blankOutAnswerWord(char * answer, char wordToCompare[])
{
  //we make a copy of the word we want to compare
  char * compareWord = malloc(6*sizeof(char));
  strcpy(compareWord, wordToCompare);//copy guess word

  //First pass to check same position and same letter to blank them up
  for(int i = 0; i < 5; i++)
  {
    if(answer[i] == compareWord[i])
    {
      answer[i] = '+';
      compareWord[i] = '-';
    }
  }
  //second pass checks if letter is in the word but not in the correct spot
  for(int i = 0; i< 5; i++)
  {
    for(int j = 0; j< 5; j++)
    {
      if(answer[i] == compareWord[j])
      {
        answer[i] = '+';
        compareWord[j] = '-';
      }
    }
  }
  //Free dynamically allocated c string
  free(compareWord);
}
//---------------------------------------------------------
//This function takes a sorted array with top scores already calculated from bestFirstWordsCalculator function, it takes answer array with their respective index sizes. It also takes in a c string from the best first word and finally it takes an array for best second words. The function uses the best first word to compare it to a copy of answers array to blank the words already used. Once we have this modified array it utilizes bestFristWordCalculator function to calculate and store this new scores in bestSecondWords.
void bestSecodWordscalculator(wordStruct * pMainArray, wordStruct * pAnswerArray, int sizeOfTotalWords, int sizeOfAnswerWords, char *bestFirstWords, wordStruct * bestSecondWords)
{
  wordStruct * pAnswersCopy = (wordStruct*)malloc(sizeOfAnswerWords*sizeof(wordStruct));//copy of answers array

  //copying answers array
  for(int i = 0; i < sizeOfAnswerWords; i++)
  {
    strcpy(pAnswersCopy[i].word, pAnswerArray[i].word);
    pAnswersCopy[i].score = pAnswerArray[i].score;
  }

  //loop through all answer words and blank them up according to bestFirstWords
  for(int i = 0; i< sizeOfAnswerWords; i++)
  {
    blankOutAnswerWord(pAnswersCopy[i].word,bestFirstWords);
  }

  //pass the answer array copy and get new score for the array
  bestFirstWordsCalculator(bestSecondWords, pAnswersCopy, sizeOfTotalWords, sizeOfAnswerWords);
  
  //free array copy
  free(pAnswersCopy);
  
}
//-------------------------------------------------------------------------
//findTopScoredWords takes in an array of best words, the total words array and a starting index. The function check top score with everyother score and if we have a tie both words get stored into the array of best words.The starting index is usefull for retriving both first and second best words. 
void findTopScoredWords(wordStruct * bestWordsArray, wordStruct * pWordArray, int * startIndex)
{
  for (int i=0; pWordArray[i].score == pWordArray[0].score; i++) {
    strcpy(bestWordsArray[*startIndex].word, pWordArray[i].word);
    bestWordsArray[*startIndex].score = pWordArray[i].score;
    (*startIndex)++;
  }
}
//end of function---------------------------------------------------------
int main(void) {
  
  int sizeAns = 0;//Word count of file answer tiny
  int sizeGuess = 0;//Word count of file guesses tiny

  // Set default file names
  char answersFileName[81] = "answersTiny.txt";;  // Stores the answers file name
  char guessesFileName[81] = "guessesTiny.txt";  // Stores the guesses file name

  //-------Menu---------
  // Display menu, to allow partial credit for different program components
  int menuOption = 0;

   printf("Default file names are %s and %s\n", answersFileName, guessesFileName);
  
  do {
    printf("\n");
    printf("Menu Options:\n");
    printf("  1. Display best first words only\n");
    printf("  2. Display best first and best second words\n");
    printf("  3. Change answers and guesses filenames\n");
    printf("  4. Exit\n");
    printf("Your choice: ");
    scanf("%d", &menuOption);

    // Handle menu option 3 to exit the program
    if (menuOption == 4)
    {
      exit(1); // Exit the program
    }else if (menuOption == 3)
    {
      // Change file names.  Menu will then be redisplayed.
      printf("Enter new answers and guesses filenames: ");
      scanf("%s %s", answersFileName, guessesFileName);
    }
  } while( menuOption == 3);
  
  //1. read files and count how many words there are on the file:
  getWordCount(answersFileName, &sizeAns);
  getWordCount(guessesFileName, &sizeGuess);
  
  //index of total size of all words.
  int totalWords = sizeAns + sizeGuess;// Total amount of words

  //3. Allocate an array of Struct type to save words and scores from files:
  wordStruct *pAnswers = (wordStruct *)malloc(sizeAns*sizeof(wordStruct));// array for answer words
  wordStruct *pWordArray = (wordStruct *)malloc(totalWords*sizeof(wordStruct));// array for total words to guess including answer words

  //4. Reread files and merge answers and guess words into one array.
  //Fucntion reads and stores words starting at the given index.
  int startIndex = 0;// start index from where we want to store words.
  extractWords(answersFileName, &pAnswers, startIndex);//We extract answer words and put them in an array.
  startIndex = 0;//Reset index
  extractWords(answersFileName, &pWordArray, startIndex);//We extract answer to put them in the total words array.
  startIndex = sizeAns;//We change the index to start from where we left off to merge both answers and guess words.
  extractWords(guessesFileName, &pWordArray, startIndex);//We continue to extract words now from the guess file.
    
  //5. We calculate the score for best first words using bestFirstWordsCalculator function
  bestFirstWordsCalculator(pWordArray, pAnswers, totalWords, sizeAns);
  qsort(pWordArray, totalWords, sizeof(wordStruct), compareFunction);//6. We sort scores in descending order.

  int indexBestWords = 0;//declare index of best first words.

  //Calculate how big the best first words array will be
  for (int i=0; pWordArray[i].score == pWordArray[0].score; i++) {
    indexBestWords++;
  }
  
  //Allocte an array of wordStruct to store multiple best first words if score is tied:
  wordStruct * topFirstWords = (wordStruct *)malloc(sizeof(wordStruct)*indexBestWords);
  //Find best First Words and store it in an array
  startIndex = 0;//start index will be 0
  findTopScoredWords(topFirstWords,pWordArray, &startIndex);

  //initialize a new array that will contain wordStructs for best second words calculation
  wordStruct * bestSecondWordsArray = (wordStruct *)malloc(totalWords*sizeof(wordStruct));

  //initialize second total array:
  for(int i = 0; i < totalWords; i++)
  {
    strcpy(bestSecondWordsArray[i].word, pWordArray[i].word);
    bestSecondWordsArray[i].score = 0;
  }
  //Calculate Best Second Word:
  int indexSecondBestWords = 0;//declare index of best second words.
  
  for(int i = 0; i< indexBestWords; i++)//we loop through best first words size to find Second best word
  {
    bestSecodWordscalculator(pWordArray, pAnswers, totalWords, sizeAns, topFirstWords[i].word, bestSecondWordsArray);
    qsort( bestSecondWordsArray, totalWords, sizeof(wordStruct), compareFunction);//6. We sort scores in descending order.
    //get index of bestSecondWords
    for (int j=0; bestSecondWordsArray[j].score == bestSecondWordsArray[0].score; j++) {
      indexSecondBestWords++;//We find the size
    }
  }
  
  //Allocte an array of wordStruct to store multiple best second words if scores are tied:
  wordStruct *topSecondWords = (wordStruct *)malloc(indexSecondBestWords*sizeof(wordStruct));

  startIndex = 0;//reset Index for a new use;
  for(int i = 0; i< indexBestWords; i++)//we loop through best first words size to find Second best word
  {
    bestSecodWordscalculator(pWordArray, pAnswers, totalWords, sizeAns, topFirstWords[i].word, bestSecondWordsArray);
    qsort(bestSecondWordsArray, totalWords, sizeof(wordStruct), compareFunction);//6. We sort scores in descending order.
    findTopScoredWords(topSecondWords, bestSecondWordsArray, &startIndex);//we find the words containing the top score and store them in an array
  }

  //We handle the las 2 options of the menu after implementing them
  if(menuOption == 1)
  {
    printf("%s has %d words\n%s has %d words\n\n", answersFileName, sizeAns, guessesFileName, sizeGuess);
    printf("Words and scores for top first words:\n");
    for(int i = 0; i< indexBestWords; i++)
    {
      printf("%s %d\n", topFirstWords[i].word, topFirstWords[i].score);
    }
    printf("Done\n");
    
  }else if(menuOption == 2)
  {
    printf("%s has %d words\n%s has %d words\n\n", answersFileName, sizeAns, guessesFileName, sizeGuess);
    printf("Words and scores for top first words and second words: \n");
    for(int i = 0; i< indexBestWords; i++)
    {
      printf("%s %d\n", topFirstWords[i].word, topFirstWords[i].score);
      printf("\t%s %d\t", topSecondWords[i].word, topSecondWords[i].score);
      printf("\n");
    }
    printf("Done\n");
  }

  //Free up any dynamically allocated arrays
  free(pAnswers);
  free(pWordArray);
  free(topFirstWords);
  free(bestSecondWordsArray);
  free(topSecondWords);

  return 0;
}
//end of main----------------------------------------------------------