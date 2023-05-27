/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const int STANDARD_BOGGLE_SIZE = 4;
const int BIG_BOGGLE_SIZE = 5;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


const string SENTINEL = "";
const int CUBE_SIDES = 6;
const int MIN_CHAR_WORD = 4;

/* Function prototypes */

void welcome();
void giveInstructions();
void setUpBoard(Grid<char>& board, Grid<bool>& used);
void userSetUp(Grid<char>& board);
void computerSetUp(int size, Grid<char>& board);
void userSetUp(int size, Grid<char>& board);
void play(Grid<char>& board, Grid<bool>& used);
void userTurn(Grid<char>& board, Grid<bool>& used);
void computerTurn(Grid<char>& board, Grid<bool>& used);
bool isValid(string word, Grid<char>& board, Grid<bool>& used);
bool isOnBoard(string word, int row, int col, Grid<char>& board, Grid<bool>& used);
bool isInBounds(int row, int col, int size);
void resetUsed(Grid<bool>& used);
void highlight(bool on, Grid<bool>& used);
void findWord(string word, int row, int col, Vector<string>& computerWords, Grid<char>& board, Grid<bool>& used);
bool bigBoggle();
bool setYourself();

// Lexicon 
Lexicon lexicon("EnglishWords.dat");

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();

	string playAgain;
	while(true){
		Grid<char> board;
		Grid<bool> used;
		setUpBoard(board, used);
		play(board, used);

		cout << "You lose. But don't worry, it's practically impossible to beat computer in this game. Do you wanna try again? ";
		getline(cin, playAgain);

		if(toLowerCase(playAgain) == "yes"){
			continue;
		} else {
			cout << "Goodbye!" << endl;
			break;
		}
	}
    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

/*
 * Function: setUpBoard
 * Usage: setUpBoard(board, used);
 * --------------------------
 * Finds out if user wants to play standard or big boggle. Also
 * asks user if they want to set charachters themselves and sets up a board.
 */
void setUpBoard(Grid<char>& board, Grid<bool>& used) {
	int boggleSize;
	if(bigBoggle()){
		boggleSize = BIG_BOGGLE_SIZE;
		board.resize(BIG_BOGGLE_SIZE, BIG_BOGGLE_SIZE);
		used.resize(BIG_BOGGLE_SIZE, BIG_BOGGLE_SIZE);
		drawBoard(BIG_BOGGLE_SIZE, BIG_BOGGLE_SIZE);
	} else {
		drawBoard(STANDARD_BOGGLE_SIZE, STANDARD_BOGGLE_SIZE);
		board.resize(STANDARD_BOGGLE_SIZE, STANDARD_BOGGLE_SIZE);
		used.resize(STANDARD_BOGGLE_SIZE, STANDARD_BOGGLE_SIZE);
		boggleSize = STANDARD_BOGGLE_SIZE;
	}

	if(setYourself()){
		userSetUp(boggleSize, board);
	} else {
		computerSetUp(boggleSize, board);
	}
}

/*
 * Function: bigBoggle
 * Usage: bigBoggle();
 * --------------------------
 * Asks user if they want to play big boggle instead of standard one
 * and returns true if they do or if they don't returns false.
 */
bool bigBoggle(){
	string answer;
	while(true){
		cout << "do you want to play big boggle instead of standard? " << endl;
		getline(cin, answer);

		if(toLowerCase(answer) == "yes"){
			return true;
		}

		if(toLowerCase(answer) == "no"){
			return false;
		}

		cout << "Answer should be yes or no." << endl;
	}
}

/*
 * Function: setYourself
 * Usage: setYourself();
 * --------------------------
 * Asks user if they want to set game board themselves
 * and returns true if they do or if they don't returns false.
 */
bool setYourself(){
	string answer;
	while(true){
		cout << "do you want to set charachters yourself? " << endl;
		getline(cin, answer);

		if(toLowerCase(answer) == "yes"){
			return true;
		}

		if(toLowerCase(answer) == "no"){
			return false;
		}

		cout << "Answer should be yes or no." << endl;
	}
}


/*
 * Function: computerSetUp
 * Usage: computerSetUp(size, board);
 * --------------------------
 * Sets Game board automatically. Shuffles cubes
 * and randomly chooses one from 6 characters to 
 * display on board.
 */
void computerSetUp(int size, Grid<char>& board){
	Vector<string> cubes;

	if(size == STANDARD_BOGGLE_SIZE){
		foreach (string s in STANDARD_CUBES){
			cubes.add(s);
		}
	} else {
		foreach (string s in BIG_BOGGLE_CUBES){
			cubes.add(s);
		}
	}
	

	for(int i = 0; i < cubes.size(); i++){
		int r = randomInteger(i, cubes.size()-1);
		string temp = cubes.get(i);
		cubes.set(i, cubes.get(r));
		cubes.set(r, temp);
	}

	for(int i = 0; i < cubes.size(); i++){
		int row = i / size;
		int col = i % size;

		char letter = cubes[i][randomInteger(0, CUBE_SIDES - 1)];
		board[row][col] = letter;
		labelCube(row, col, letter);
	}
}

/*
 * Function: userSetUp
 * Usage: userSetUp(size, board);
 * --------------------------
 * Asks user to enter cube configurations and 
 * then randomly chooses one charachter from that
 * configurations to display on board.
 */
void userSetUp(int size, Grid<char>& board){
	Vector<string> cubes;
	cout << "Please enter your cube configurations!" << endl;
	cout << "One configuration is " << CUBE_SIDES << " charachters long." << endl;

	int boardSize = size * size;
	for(int i = 0; i < boardSize; i++){
		string userString;
		cout << "Enter configuration for cube #" << i+1 << ": ";
		while(true){
			getline(cin, userString);
			if(userString.size() >= CUBE_SIDES) break;
			cout << "String should be at least " << CUBE_SIDES << " characters long. Please enter again: " << endl;
		}
		cubes.add(toUpperCase(userString));
	}

	for(int i = 0; i < cubes.size(); i++){
		int row = i / size;
		int col = i % size;

		char letter = cubes[i][randomInteger(0, CUBE_SIDES - 1)];
		board[row][col] = letter;
		labelCube(row, col, letter);
	}

	//IN ENGLISH VERSION OF THE ASSIGNMENT THIS PART WAS DIFFERENT SO I WROTE BOTH
	// VERSIONS.
	/* int stringSize = size * size;
	cout << "Please enter a string with " << stringSize << " characters." << endl;
	cout << "First " << size << " characters are first row letters. Next " << size << " characters are second row letters and so on." << endl;

	string userString;
	while(true){
		getline(cin, userString);
		if(userString.size() >= stringSize) break;
		cout << "String should be at least " << stringSize << " characters long. Please enter again: " << endl;
	}

	userString = toUpperCase(userString);
	for(int i = 0; i < userString.size(); i++){
		int row = i / size;
		int col = i % size;

		char letter = userString[i];
		board[row][col] = letter;
		labelCube(row, col, letter);
	} */
}

/*
 * Function: play
 * Usage: play(board, used);
 * --------------------------
 * Function for gameplay.
 */
void play(Grid<char>& board, Grid<bool>& used){
	cout << "Board is all set up. Let's start playing!" << endl;
	userTurn(board, used);
	computerTurn(board, used);
}

/*
 * Function: userTurn
 * Usage: userTurn(board, used);
 * --------------------------
 * lets user start entering words. Checks if word is valid
 * and if its alredy guessed by user or not. If it is gussed
 * word is added in guessed words and highlighted. Also keeps 
 * track of score.
 */
void userTurn(Grid<char>& board, Grid<bool>& used) {
	Vector<string> userWords;
	cout << "You can enter the words now! When you are done just enter '" << SENTINEL << "'." << endl;
	string word;
	while(getline(cin,word)){
		if(word == SENTINEL) break;

		if(isValid(toUpperCase(word), board, used)){
			bool alreadyGuessed = false;
			foreach(string s in userWords){
				if(s == word){
					alreadyGuessed = true;
				}
			}

			if(alreadyGuessed){
				cout << "You have already guessed that!" << endl;
			} else {
				userWords.add(word);
				recordWordForPlayer(word, HUMAN);
			}
		}

		cout << "Enter word: ";
	}
}

/*
 * Function: computerTurn
 * Usage: computerTurn(board, used);
 * --------------------------
 * Computer starts finding words.
 */
void computerTurn(Grid<char>& board, Grid<bool>& used) {
	Vector<string> computerWords;
	for(int i = 0; i < board.numRows(); i++){
		for(int j = 0; j < board.numCols(); j++){
			resetUsed(used);
			used[i][j] = true;
			string s;
			s += board[i][j];
			findWord(s, i, j, computerWords, board, used);
		}
	}
}

/*
 * Function: findWord
 * Usage: findWord(word, row, col, computerWords, board, used);
 * --------------------------
 * Recursive function for computer to find every word
 * possible on the board. If word is found computer adds
 * it to computer guessed words and keeps track of score.
 * If word is not found function is called again, checking
 * next row and/or next column.
 */
void findWord(string word, int row, int col, Vector<string>& computerWords, Grid<char>& board, Grid<bool>& used){
	if(lexicon.contains(word) && word.size() >= MIN_CHAR_WORD){
		bool alreadyGuessed = false;
		foreach(string s in computerWords){
			if(s == word){
				alreadyGuessed = true;
			}
		}
		if(!alreadyGuessed){
			highlight(true, used);
			pause(20);
			highlight(false, used);
			computerWords.add(word);
			recordWordForPlayer(word, COMPUTER);
		}
	}

	if (!lexicon.containsPrefix(word)) return;

	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			row += i;
			col += j;

			if(isInBounds(row, col, board.numRows()) && !used[row][col]){
				used[row][col] = true;
				word += board[row][col];
				
				findWord(word, row, col, computerWords, board, used);

				word = word.substr(0, word.size()-1);
				used[row][col] = false;
			}

			row -= i;
			col -= j;
		}
	}

}

/*
 * Function: isValid
 * Usage: isValid(word, board, used);
 * --------------------------
 * Checks if given word is enough length, if its real word
 * and if its on board.
 */
bool isValid(string word, Grid<char>& board, Grid<bool>& used){
	if(word.size() < MIN_CHAR_WORD) {
		cout << "Word should be at least " << MIN_CHAR_WORD << " charachters." << endl;
		return false;
	}

	bool onBoard = false;

    for (int i = 0; i < board.numRows(); i++){
        for (int j = 0; j < board.numCols(); j++){
            if (word[0] == board[i][j]){
                used[i][j] = true;
                if(isOnBoard(word, i, j, board, used)){
                    onBoard = true;
					highlight(true, used);
					pause(20);
					highlight(false, used);
                    break;
                }
            } else {
                resetUsed(used);
            }
        }
    }

	if(onBoard){
		if(lexicon.contains(word)){
			return true;
		} else {
			cout << "That is not a real word." << endl;
			return false;
		}
	} else {
		cout << "Word is not on board." << endl;;
		return false;
	}
}

/*
 * Function: highlight
 * Usage: highlight(on, used);
 * --------------------------
 * highlights or un-highlights the cubes used
 * to create word.
 */
void highlight(bool on, Grid<bool>& used){
	for(int i = 0; i < used.numRows(); i++){
		for(int j = 0; j < used.numCols(); j++){
			if(on){
				highlightCube(i, j, used[i][j]);
			} else {
				highlightCube(i, j, false);
			}
		}
	}
}

/*
 * Function: isOnBoard;
 * Usage: isOnBoard(word, row, col, board, used);
 * --------------------------
 * Recursive function to check if word is on board, by
 * checking neighbour cubes.
 */
bool isOnBoard(string word, int row, int col, Grid<char>& board, Grid<bool>& used){
	if(word.size() == 1 && word[0] == board[row][col]) return true;
	if(word[0] != board[row][col]) return false;

	int newRow;
	int newCol;

	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			newRow = row + i;
			newCol = col + j;

			if(isInBounds(newRow, newCol, board.numRows()) && !used[newRow][newCol]){
				used[newRow][newCol] = true;
				if(isOnBoard(word.substr(1), newRow, newCol, board, used)){
					return true;
				}

				used[newRow][newCol] = false;
			}
		}
	}

	return false;
}

/*
 * Function: isInBounds
 * Usage: isInBounds(row, col, size);
 * --------------------------
 * Checks if coordinates are in the bounds of board.
 */
bool isInBounds(int row, int col, int size){
	return (row >= 0) && (row < size) && (col >= 0) && (col < size);
}

/*
 * Function: resetUsed
 * Usage: resetUsed(used);
 * --------------------------
 * Cleans up the grid that shows if
 * letter is used or not.
 */
void resetUsed(Grid<bool>& used){
	for(int i = 0; i < used.numRows(); i++){
		for(int j = 0; j < used.numCols(); j++){
			used[i][j] = false;
		}
	}
}
