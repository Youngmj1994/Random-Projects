
//This program will allow users to enter info about clients and then view or update that info.

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>

using namespace std;
const int MAXCUST = 30;

struct Customer
{
	int id;    	// id
	string name;    // Name
	string zip;     // ZIP code
	double balance; // Account balance
	string lastPay; // Date of last payment
};

/* functions
1. add cust
2. update cust
3. display cust
4. find cust
5. menu */

void addcust(Customer a[], int &numcust);
void updatecust(Customer a[], int numcust);
void displaycust(Customer a[], int &numcust);
int findcust(Customer a[], int numcust, int value);
int menu();

int main()
{
	Customer a[MAXCUST];
	int numcust = 0, choice;
	choice = menu();
	//this is the loop that users will use for the menu, that will direct them to different functions
	while (choice != 4)
	{
		if (choice == 1)
		{
			addcust(a, numcust);
		}
		else if (choice == 2)
		{
			updatecust(a, numcust);
		}
		else if (choice == 3)
		{
			displaycust(a, numcust);
		}
		choice = menu();
	}
	if (choice == 4)
	{
		return 0;
	}

	
	return 0;
}

//this function will allow users to add customers to the data.
void addcust(Customer a[], int &numcust)
{
	
	int k = 0;
	if (numcust <= MAXCUST)
	{
		//this asks for all the relevant information.
			cout << "Please enter customer information." << endl;
			cout << "Please enter the ID number :" << endl;
			cin >> a[numcust].id;
			cin.ignore();
			cout << a[numcust].id << endl;
			cout << "Please enter name: " << endl;
			getline(cin, a[numcust].name);
			cout << a[numcust].name << endl;
			cout << "Please enter zip code: " << endl;
			getline(cin, a[numcust].zip);
			cout << a[numcust].zip << endl;
			cout << "Please enter balance: " << endl;
			cin >> a[numcust].balance;
			cin.ignore();
			cout << a[numcust].balance << endl;
			//data validation, can't be a negative or a zero
			while (a[numcust].balance <= 0)
			{
				cout << "Invalid answer. Balance cannot be negative. Please enter balance: " << endl;
				cin >> a[numcust].balance;
				cin.ignore();
				cout << a[numcust].balance << endl;
			}
			cout << "Please enter the date of the last payment: " << endl;
			getline(cin, (a[numcust].lastPay));
			//make sure to echo all cin statements.
			cout << a[numcust].lastPay << endl;
			numcust++;
	}
	else
		cout << "You have reached the max number of customers." << endl;
}
// this will allow the user to update the customers information after finding the correct id.
void updatecust(Customer a[], int numcust)
{
	int value=0;
	cout << "Please enter the ID of the customer you are looking for: " << endl;
	cin >> value;
	findcust(a, numcust, value);
	value = findcust(a, numcust, value);
	//if the id isn't found it will be asked for again
	if(value == -1)
	{
		cout << "Sorry that position does not exist please try again: " << endl;
		cin >> value;
		findcust(a, numcust, value);
		value = findcust(a, numcust, value);
	}
	
	cout << "Please enter customer information." << endl;
	cout << "Please enter the ID number :" << endl;
	cin >> a[value].id;
	cin.ignore();
	cout << a[value].id << endl;
	cout << "Please enter name: " << endl;
	getline(cin, a[value].name);
	cout << a[value].name << endl;
	cout << "Please enter zip code: " << endl;
	getline(cin, a[value].zip);
	cout << a[value].zip << endl;
	cout << "Please enter balance: " << endl;
	cin >> a[value].balance;
	cin.ignore();
	cout << a[value].balance << endl;
	while (a[value].balance <= 0)
	{
		cout << "Invalid answer. Balance cannot be negative or zero. Please enter balance: " << endl;
		cin >> a[value].balance;
		cin.ignore();
		cout << a[value].balance << endl;
	}
	cout << "Please enter the date of the last payment: " << endl;
	getline(cin, (a[value].lastPay));
	cout << a[value].lastPay << endl;
	

}
// this displays all the relevant information
void displaycust(Customer a[], int &numcust)
{
	cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(10) << "Zip Code"
		<< setw(10) << "Balance" << setw(8) << "Last Payment" << endl;
	cout << setw(5) << "--" << setw(15) << "----" << setw(10) << "--------"
		<< setw(10) << "-------" << setw(8) << "------------" << endl;
	for (int k = 0; k < numcust; k++)
	{
		cout << left << setw(0) << " " << a[k].id;
		cout << right << setw(1) << " " << a[k].name;
		cout << right << setw(5) << " " << a[k].zip;
		cout << right << setw(4) <<" " << a[k].balance;
		cout << right << setw(7) << " " << a[k].lastPay << endl;
		
	}
	cout << endl << endl;

}
// this is the linear search function that finds the users ID.
int findcust(Customer a[], int numcust, int value)
{
	int index = 0;       // Used as a subscript to search array
	int position = -1;   // To record position of search value
	bool found = false;  // Flag to indicate if the value was found

	while (index <= numcust && !found)
	{
		if (a[index].id == value)  // If the value is found
		{
			found = true;           // Set the flag
			position = index;       // Record the value's subscript
		}
		index++;                   // Go to the next element
	}
	
	return position;              // Return the position, or -1
}

// the menu which determines what the user will do.
int menu()
{
	int choice;
	// Display a menu.
	cout << "1. Enter new account information\n";
	cout << "2. Change account information\n";
	cout << "3. Display all account information\n";
	cout << "4. Exit the program\n\n";
	// Get the user's choice.
	cout << "Enter your choice: ";
	cin >> choice;
	cout << choice << endl;  //echo statement for bgunix
	//can't be a number greater than 5 or less than 1
	while (choice > 4 || choice < 1)
	{
		cout << "Invalid: please enter a number between 1-4 ";
		cin >> choice;
		cout << choice << endl;
	}
	return choice;
}