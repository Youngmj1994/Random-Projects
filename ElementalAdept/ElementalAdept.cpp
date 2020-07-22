#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

//this is my characters spell modifier, i.e. his CHA bonus. 
//you can change it to whatever you need it to be, also change the spells however you need to.

int mod = 4;
void rollDamage(int numberOfDice, int sizeOfDice);
int main()
{
	int num = 0;
	srand(time(NULL));
	while (num != 5)
	{
		cout << "1. Fireball" << endl;
		cout << "2. Firebolt" << endl;
		cout << "3. Scorching Ray" << endl;
		cout << "4. Wall of Fire" << endl;
		cout << "5. Quit" << endl;
		cout << "Pick a number." << endl;
		cin >> num;
		switch (num)
		{
		case(1):  rollDamage(8, 6);
		break;
		case(2):  rollDamage(2, 10);
		break;
		case(3): rollDamage(2, 6);
		break;
		case(4):  rollDamage(5, 8);
		break;
		case(5):
		break;
		default:
		cout << "That's the wrong number." << endl;
		}
	}

	return 0;
}

void rollDamage(int numberOfDice, int sizeOfDice)
{
	int a = numberOfDice, b = sizeOfDice, total = 0, temp = 0;
	while (a >= 0)
	{
		
		temp = 1 + (rand() % b);
		if (temp == 1)
		{
			temp = 2;
		}
		total += temp;
		a--;
		cout << total << endl;
		
	}
	total = total + mod;
	cout << "Total Damage : " << total << endl;
}