#pragma once
// This program calls the Time class.
#include <iostream>
#include <iomanip>
using namespace std;

class Time
{
private:
	int hours;
	int minutes;
	

public:
	Time();
	Time(int phh, int pmm);
    
	void setTime(int phh, int pmm);
	
	int getHour();
	
	void addOneMinute();
	void showmealtime();
	
	void display();


};
