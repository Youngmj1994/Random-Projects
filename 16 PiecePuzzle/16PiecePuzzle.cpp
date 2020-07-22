#include <iostream>
#include <string>     
#include <algorithm>    
#include <iomanip>
using namespace std;

void swapPosition(string choice);
void display();
bool checkIfWin();
void displayWinScreen();
void displayOpenScreen();

int puzzleArray[4][4] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

int main()
{
	
	
	string choiceString;
	/*steps to take
	1. display open
	2.scramble the array
	3. display the numbers
	4. get zero position
	5. get move
	6. move left,right,up,down accordingly
	7. display numbers
	8. check win
	9. if not win then continue loop
	10. if win then display win screen */

	displayOpenScreen();
	std::srand(time(NULL));
	std::random_shuffle(&puzzleArray[0][0], &puzzleArray[0][0] + sizeof(puzzleArray) / sizeof(puzzleArray[0][0]));
	
	while (!checkIfWin() && choiceString != "q")
	{
		display();
		
		cout << "Enter Choice w,a,s,d for up, left, down, and right respectively: ";
		cin >> choiceString;
		cout << endl;
		//get move from user
		
		swapPosition(choiceString);
		checkIfWin();
		system("CLS");
	}

	if (choiceString == "q")
		cout << "Goodbye, Thanks For Playing!" << endl;
	if (checkIfWin())
		displayWinScreen();

	system("PAUSE");
	return 0;
}

void swapPosition(string choice)
{
	int n = 0, m = 0,a,b, temp = 0;
	for (n = 0; n < 4; n++)
	{
		for (m = 0; m < 4; m++)
		{
			if (puzzleArray[m][n] == 0) {
				a = m;
				b = n;
			}
		}
		
	}
	if (choice == "w") 
	{
		if ((b - 1) < 0)
		{
			cout << "Incorrect Choice, Please Try Again." << endl;
			return;
		}
		swap(puzzleArray[a][b], puzzleArray[a][b - 1]);
	}
	if (choice == "a")
	{
		if ((a - 1) < 0)
		{
			cout << "Incorrect Choice, Please Try Again." << endl;
			return;
		}
		swap(puzzleArray[a][b], puzzleArray[a-1][b]);
	}
	if (choice == "s")
	{
		if ((b + 1) > 3)
		{
			cout << "Incorrect Choice, Please Try Again." << endl;
			return;
		}
		swap(puzzleArray[a][b], puzzleArray[a][b + 1]);
	}
	if (choice == "d")
	{
		if ((a + 1) > 3)
		{
			cout << "Incorrect Choice, Please Try Again." << endl;
			return;
		}
		swap(puzzleArray[a][b], puzzleArray[a+1][b]);
	}

	//swap position from 0 to up, or down, or right, or left depending on up, down, left, right, or w,s,a,d

}


void display()
{
	for (int n = 0; n < 4; n++)
	{
		for (int m = 0; m < 4; m++)
		{
			if(puzzleArray[m][n] == 0)
				cout << "| " << setw(2) << "  " << " |";
			else
			    cout << "| " << setw(2) << puzzleArray[m][n] << " |";
		}
		cout << endl;
	}
	cout << endl;
}


bool checkIfWin()
{
	int p = 0;
	bool check = true;
	for (int i = 0; i < 5; i++)
	{
		for (int x = 0; x < 16; x++) 
		{
			if (puzzleArray[i][x] != p)
			{
				check = false;
				return check;
			}
			p++;
		}
		return check;
	}
}

void displayWinScreen()
{
	cout << "*****************************" << endl;
	cout << "* You Win! Congratulations! *" << endl;
	cout << "*****************************" << endl;
}

void displayOpenScreen()
{
	cout << "****************************************" << endl;
	cout << "* Welcome to the 16 Piece Puzzle Game! *" << endl;
	cout << "* For this game you must get all the   *" << endl;
	cout << "* pieces in a row. You can swap one    *" << endl;
	cout << "* piece at a time with the W,A,S, or D *" << endl;
	cout << "* keys. You cant go out of bounds      *" << endl;
	cout << "* and when all the pieces are lined    *" << endl;
	cout << "* up you win the game. You can quit    *" << endl;
	cout << "* at any time with Q. Good Luck!       *" << endl;
	cout << "****************************************" << endl;
	cout << endl;
}