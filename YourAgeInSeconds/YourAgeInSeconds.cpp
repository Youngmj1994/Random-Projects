#include <iostream>
#include <ctime>
#include <stdio.h>
#pragma warning(disable:4996)
using namespace std;

int * getBirthDate();
int convertDate(int arrayDate[]);
int * getCurrentDate();

int main()
{
	int birthSeconds = 0, currentSeconds = 0, total = 0;
	int * birthdayArray;
	int * currentArray;

	birthdayArray = getBirthDate();
	currentArray = getCurrentDate();

	birthSeconds = convertDate(birthdayArray);
	currentSeconds = convertDate(currentArray);

	total = (currentSeconds - birthSeconds);

	cout << "This is your age in Seconds! : " << total << endl;

	//get birthday
	//convert birthday into seconds.
	//get current time
	//convert into seconds
	//subtract current time from birthday time

	system("PAUSE");
	return 0;
}


int * getBirthDate()
{
	int * arrayDate = new int [3];
	int month = 0, day = 0, year = 0;
	cout << "Find out your age in seconds!" << endl;
	cout << "Enter your Birth Month in mm format : ";
	cin >> month;
	cout << "Enter your Birth Day in dd format : ";
	cin >> day;
	cout << "Enter your Birth Year in yyyy format : ";
	cin >> year;
	cout << endl;
	arrayDate[0] = month;
	arrayDate[1] = day;
	arrayDate[2] = year;

	
	return arrayDate;
}

int convertDate(int arrayDate[])
{
	int seconds = 0, month = 0, day = 0;
	long long year = 0;

	month = arrayDate[0];
	day = arrayDate[1];
	year = arrayDate[2];

	//have to get the amount of days into the year we are
	if (month == 1)
		month = 0;
	if (month == 2)
		month = 31;
	if (month == 3)
		month = 59;
	if (month == 4)
		month = 90;
	if (month == 5)
		month = 120;
	if (month == 6)
		month = 151;
	if (month == 7)
		month = 181;
	if (month == 8)
		month = 212;
	if (month == 9)
		month = 243;
	if (month == 10)
		month = 273;
	if (month == 11)
		month = 304;
	if (month == 12)
		month = 334;

	//if ((year % 4) == 0)
		//month = month + 1;

	
	year = (year * 365.2425 * 24 * 60 * 60);
	day = (day + month);
	day = (day * 24 * 60 * 60);
	
	seconds = year + day;

	return seconds;
}


int * getCurrentDate()
{
	int month = 0, day = 0, year = 0;
	time_t now = time(0);
	tm *ltm = localtime(&now);

	int * arrayCurrent = new int[3];
	month = 1 + ltm->tm_mon;
	year = 1900 + ltm->tm_year;
	day = ltm->tm_mday;

	arrayCurrent[0] = month;
	arrayCurrent[1] = day;
	arrayCurrent[2] = year;
	
	return arrayCurrent;
}