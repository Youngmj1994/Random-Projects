// This program calls the Time class.
#include <iostream>
#include <iomanip>
#include "time.h"
using namespace std;



Time::Time(int phh, int pmm)	//this overloaded constructor
{								//will call setTime, so you
	setTime(phh, pmm);			//can write your code once 
}								// "time" saver

Time::Time()
{

		hours = 0;
		minutes = 0;
}

void Time::setTime(int phh, int pmm)
{
	if (phh >= 24 || pmm > 59)
	{
		hours = 0;
		minutes = 0;
	}
	else if (phh < 0 || pmm < 0)
	{
		hours = 0;
		minutes = 0;
	}
	else
	{
		hours = phh;
		minutes = pmm;
	}
}
int Time::getHour()
{
	return hours;
}
void Time::addOneMinute()
{

	if (minutes == 59 && hours == 23)
	{
		hours = 0;
		minutes = 0;
	}
	else if (minutes == 59)
	{
		hours += 1;
		minutes = 0;
	}
	else if (hours <= 23 && minutes < 59)
	{
		minutes += 1;
		hours = hours;

	}
}
void Time::showmealtime()
{
	if (hours == 7 )
	{
		cout << "It's Breakfeast " << endl;
	}
	else if (hours == 12 )
	{
		cout << "It's Lunch " << endl;

	}
	else if (hours == 17 )
	{
		cout << "It's Dinner " << endl;

	}
	else
	{
		cout << "No Meal? " << endl;

	}
}
void Time::display()
{
	cout << hours << ":" << setw(2) << setfill('0') << minutes << endl;
}

