
// This program calls the Time class.
#include <iostream>
#include <iomanip>
#include "time.h"
using namespace std;

int main()
{
	int k, temphrs, tempmins, freehour, bghour;
	char colon; //used to skip over the colon in the input
				
	Time FreeTime;
	Time WrongTime(24, 45);
	Time BGTime(7, 30);

	cout << "FreeTime:";
	// display  FreeTime
	 FreeTime.display();
	cout << endl;

	cout << "WrongTime:";
	// display  WrongTime
	WrongTime.display();
	cout << endl;

	cout << "BGTime:";
	//display  BGTime
	BGTime.display();
	cout << endl;

	//Here is the code for #3's input
	cout << "Please enter the time in the form HH:MM: ";
	cin >> temphrs >> colon >> tempmins;
	// fill the object with the input values
	  
	FreeTime.setTime(temphrs, tempmins);

	cout << "FreeTime:";
	// display  FreeTime
	FreeTime.display();

	for (k = 0; k < 150; k++)
	{	
		FreeTime.addOneMinute();
	}
	cout << "FreeTime:";
	// display  FreeTime
	FreeTime.display();
	cout << endl;

	cout << "FreeTime's mealtime  : ";

	// display FreeTime's mealtime
	FreeTime.showmealtime();
	cout << endl;

	cout << "WrongTime's mealtime : ";
	// display WrongTime's mealtime
	WrongTime.showmealtime();
	cout << endl;

	cout << "BGTime's mealtime    : ";
	// display BGTime's mealtime
	BGTime.showmealtime();
	cout << endl;

	if (FreeTime.getHour() == BGTime.getHour())
		cout << "BG Time is Free Time!" << endl;

	if (FreeTime.getHour() > BGTime.getHour())
		cout << "Free Time Soon!" << endl;

	if (FreeTime.getHour() < BGTime.getHour())
		cout << "Free Time is OVER!" << endl;


	system("pause");
	return 0;
}
