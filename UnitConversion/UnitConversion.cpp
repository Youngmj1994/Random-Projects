#include <iostream>
#include <string>

using namespace std;

void convertWeight(string choice);
void convertTemp(string choice);
string getChoice();
int main()
{
	string choice;
	choice = getChoice();
	
	while (choice != "q")
	{
		if (choice == "1")
		{
			cout << "1. Convert lbs to kilos." << endl;
			cout << "2. Convert kilos to lbs." << endl;
			cout << "q. Exit " << endl;
			cout << "What would you like to convert? :" << endl;
			cin >> choice;
			convertWeight(choice);
		}
		else if (choice == "2")
		{
			cout << "1. Convert Farenheit to Celcius." << endl;
			cout << "2. Convert Celcius to Farenheit." << endl;
			cout << "q. Exit " << endl;
			cout << "What would you like to convert? :" << endl;
			cin >> choice;
			convertTemp(choice);
		}
		else if (choice == "q")
			choice = "q";
		else if(choice != "1" & choice != "2" & choice != "q")
		{
			cout << "You have entered an invalid number please try again." << endl;
			cin >> choice;
		}
		choice = getChoice();
	}

	system("pause");
	return 0;
}


string getChoice()
{
	string choice;
cout << "----------------------------------------\n";
		cout << "(1)  Convert Weights\n"
			<< "(2)  Convert Tempurature\n"
			<< "(q)  Quit\n";
		cout << "----------------------------------------\n"
			<< "Make a selection : ";
		cin >> choice;
		if (choice == "1")
			choice = "1";
		else if (choice == "2")
			choice = "2";
		else if (choice == "q")
			choice = "q";
		else
		{
			cout << "wrong. try again." << endl;
			cin >> choice;
		}
		return choice;
}

void convertWeight(string choice)
{
	double lbs = 0, kilos = 0;
	if (choice == "1")
	{
		cout << "Enter lbs: " << endl;
		cin >> lbs;
		kilos = lbs * .454;
		cout << "Your weight in kilos is :" << kilos << endl;
	}
	else if (choice == "2")
	{
		cout << "Enter kilos: " << endl;
		cin >> kilos;
		lbs = kilos * 2.205;
		cout << "Your weight in lbs is :" << lbs << endl;
	}
	if (choice != "1" & choice != "2" & choice != "q")
		cout << "You have entered an invalid number please try again." << endl;
	
	//lbs to kilo = 1lb to .454kilo, kilo to lbs = 1kg to 2.205kilos
	
}

void convertTemp(string choice)
{
	double f, c;
	if (choice == "1")
	{
		cout << "Enter Farenheit: " << endl;
		cin >> f;
		c = ((f - 32)/1.8);
		cout << "Your Temp in Celcius is :" << c << endl;
	}
	else if (choice == "2")
	{
		cout << "Enter Celcius: " << endl;
		cin >> c;
		f = ((c +32) * (9/5));
		cout << "Your Temp in Farenheit is :" << f << endl;
	}
	if (choice != "1" & choice != "2" & choice != "q")
		cout << "You have entered an invalid number please try again." << endl;
	//f to c, c to f
	
}