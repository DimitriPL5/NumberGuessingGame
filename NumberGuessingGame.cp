/*
 Description: Generates a random number that users have to guess as part of a guessing game. Multiple users are allowed to play and the wins of each player are recorded.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

class gameNumberGuess
{
public:
    int level;  //game level (1 or 2)
    int maxGuesses; //max number of guesses (example 5 or 8)
    int upperValue; //upper range value (example 100 or 1000)
    int currentGuess; //current guess input by the user
    int solution;  //pseudo random generated number
    int smallNum; //lower value for feedback
    int largeNum; //higher value for feedback
    bool above = false; //current guess above the solution
    bool below = false; //current guess below the solution
    string name; //name of current player
    bool wonOrLost = 0; //true or false for the current round of the game
};

//Precondition: User has selected yes to playing the guessing game.
//Postcondition: Array size is doubled if there is not enough space for new user.
void TestAndDouble(gameNumberGuess*& gameList, int count, int& size);

//Precondition: User(s) has succesfully played a round of the guessing game
//Postcondition: All user information will be printed to the screen and into an input file
void PrintPlayerResults(gameNumberGuess gameList[], int);

//Precondition: User has entered their name and has responded yes to playing a round.
//Postcondition: A full round is played, which stops when the user either runs out of tries or guesses correctly.
void PlayOneRound(const string&, gameNumberGuess&);

//Precondition: The user has selected yes to playing.
//Postcondition: The user is able to select their level which will have a predetermined number of guesses and a range to stay in.
void SetUpLevel(gameNumberGuess&);

//Precondition: User has selected yes to playing, and has selected a valid level.
//Postcondition: User will be able to enter guesses until they guess correctly or run out of guesses.
void GetGuessInput(gameNumberGuess&);

//Precondition: The user has entered a guess.
//Postcondition: Determines whether or not the guess was the solution (True or false).
bool ProcessGuess(gameNumberGuess&);

//Precondition: The user has selected yes to playing.
//Postcondition: Generates a random number in the level bounds that the user will have to try and guess correctly.
void GenerateSolutionNumber(gameNumberGuess&);

int main()
{
    string yesOrNo; //used to prompt user if they would like to play
    
    gameNumberGuess* gameList; //creates a pointer of gameNumberGuess objects.
    
    int gameCount = 0; //sets the round count to zero
    int size = 1; //sets array size to 1
    
    gameList = new gameNumberGuess[size]; //initializes a new array
    
    //Allows user to play or quit
    cout << "Hi, would you like to play the number guessing game? (y(Y) or n(N))" << endl;
    cin >> yesOrNo;
    
    //This loop continues to run rounds until the user quits
     while (yesOrNo != "n" && yesOrNo != "N")
     {
         //Tests to see if the array has enough space for a new player, doubles array size if not.
         TestAndDouble(gameList, gameCount, size);
         
         // Prompts user for their name and takes in the input.
         cout << "Enter your first name: " << endl;
         cin >> gameList[gameCount].name;
         
         PlayOneRound(gameList[gameCount].name, gameList[gameCount]); //Plays one round of the guessing game.
         
         gameCount++; //increase gameCount by 1 for every round user continues to play.
         
         //Prompting to see if user(s) would like to continue.
         cout << "Would you like to play another round? (y(Y) or n(N))" << endl;
         cin >> yesOrNo;
     }
     PrintPlayerResults(gameList, gameCount); // Print game results
     
     cout << "Thanks for playing! Have a great day." << endl;

     return 0;
}

//Description: Doubles array size if there is not enough space for a new user. 
void TestAndDouble(gameNumberGuess*& gameList, int count, int& size)
{
    if (count == size)
    {
        size *= 2;
        
        //create a new temp array
        gameNumberGuess* temp = new gameNumberGuess[size];
        
        //copy all the objects into the temp array
        for (int i = 0; i < count; i++)
            {
                temp[i] = gameList[i];
            }
        
        //delete the gameList pointer
        delete[] gameList;
        
        //point gameList to temp
        gameList = temp;
    }
}

//Description: Prints out users name, level, and whether they won or lost after quitting the game.
void PrintPlayerResults(gameNumberGuess gameList[], int gameCount)
{
    //Object for the ouput file
    ofstream out;
    
    //Initializing the output file and asking the user to input the name of it.
    char filename[40];
    cout << "Enter the file name(with .txt extension): " << endl;
    cin >> filename;
    out.open(filename, ios::app);
    
    cout << "\n *********************************** \n";
    cout << "Name: \t" << "Level: \t" << "Won or Lost: " << endl;
    
    //This for loop prints out info to the screen
    for(int i = 0; i < gameCount; i++)
    {
        cout << gameList[i].name << " \t " << gameList[i].level << " \t " << gameList[i].wonOrLost << endl;
    }
    
    out << "\n *********************************** \n";
    out << "Name: \t" << "Level: \t" << "Won or Lost: " << endl;
    
    //This for loop prints out info to the output file
    for(int i = 0; i < gameCount; i++)
    {
        out << gameList[i].name <<  " \t "  << gameList[i].level <<  " \t "  << gameList[i].wonOrLost << endl;
    }

    out.close();
}


//Description: Gives a description of levels to the user, and runs a loop that allows user to keep inputting guesses until they guess right or run out of tries.
void PlayOneRound(const string& name, gameNumberGuess& currentGame)
{
    currentGame.above = 0;
    currentGame.below = 0;
    currentGame.wonOrLost = 0;
    
    //describes levels to user
    cout << "What level would you like? (1 or 2)" << endl;
    cout << "(1. Intermediate) Level 1: 4 guesses, numbers 1-50." << endl;
    cout << "(2. Hard) Level 2: 6 guesses, number 1-100." << endl;
    
    SetUpLevel(currentGame); // based on user input, sets up the bounds and number of guesses
    
    GenerateSolutionNumber(currentGame); // Generates the random solution number
    
    //This loop counts the number of guesses, and allows user to keep inputting guesses until they hit the max

    currentGame.largeNum = currentGame.upperValue;
    currentGame.smallNum = 1;
    
    //Loop allows user to keep guessing until they run out of tries or guess correctly
    for(int i = 0; i < currentGame.maxGuesses; i++)
    {
        cout << "This is guess " << (i + 1) << " of " << currentGame.maxGuesses << endl;
            
        GetGuessInput(currentGame);
        //Reset above, below, and wonOrLost to 0
        currentGame.above = 0;
        currentGame.below = 0;
        currentGame.wonOrLost = 0;
            
        if (ProcessGuess(currentGame) == true)
        {
            currentGame.wonOrLost = 1;
            cout << "You won that round, " << currentGame.name <<  "!" << endl;
            return; //exit the loop if solution was guessed correctly
        }
    }
    
    cout << "The solution was " << currentGame.solution << "." << endl;
}

//Description: Allows user to select from 2 level options, and sets the number of guesses and the bounds for the chosen level.
void SetUpLevel(gameNumberGuess& currentGame)
{
    do{
    cin >> currentGame.level;
    if (currentGame.level == 1)
    {
        currentGame.upperValue = 50; // setting bounds for level 1
        currentGame.maxGuesses = 4;
    }else if (currentGame.level == 2)
    {
        currentGame.upperValue = 100; // setting bounds for level 2
        currentGame.maxGuesses = 6;
    }else
        cout << "That is not a valid entry. Enter 1 or 2." << endl;
    }while (currentGame.level < 1 || currentGame.level > 2); // while loop to ensure a valid response
}

//Description: Allows user to keep guessing while also letting them know how close they are to the correct solution.
void GetGuessInput(gameNumberGuess& currentGame)
{
    //This if statement changes bounds based on user's guesses to give them a closer range of the solution.
    if (currentGame.currentGuess > currentGame.solution && currentGame.currentGuess < currentGame.upperValue)
    {
        currentGame.largeNum = currentGame.currentGuess;
    }else if (currentGame.currentGuess < currentGame.solution && currentGame.currentGuess > currentGame.smallNum)
    {
        currentGame.smallNum = currentGame.currentGuess;
    }
    
    //This if statement lets the user know what bounds they should stay in based on their guesses.
    if (currentGame.below)
    {
        cout << "Enter a guess between " << currentGame.smallNum << " and " << currentGame.largeNum << endl;
    }else if (currentGame.above)
    {
        cout << "Enter a guess between " << currentGame.smallNum << " and " << currentGame.largeNum << endl;
    }else
        cout << "Enter a guess between " << currentGame.smallNum << " and " << currentGame.largeNum << endl;
    
    cin >> currentGame.currentGuess;
}

//Description: Determines whether or not the current guess was the solution
bool ProcessGuess(gameNumberGuess& currentGame)
{
    if (currentGame.currentGuess  > currentGame.solution)
    {
        cout << "Your guess was too high." << endl; // lets user know to guess lower
        currentGame.above = true;
        return false;
    }else if (currentGame.currentGuess < currentGame.solution)
    {
        cout << "Your guess was too low." << endl; //lets user know to guess higher
        currentGame.below = true;
        return false;
    }else
    {
        cout << "Your guess matches the solution." << endl; //user guessed correctly
        return true;
    }
}

//Description: Generates a random number that is in the bounds of the chosen level
void GenerateSolutionNumber(gameNumberGuess& currentGame)
{
    srand((unsigned)time(0));
    currentGame.solution = 1 + rand() % currentGame.upperValue;
    // This is to ensure that the random number is not higher than the upper value of the selected level.
}
