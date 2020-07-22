/* 
this program will fill 4 arrays with information about an insurance company. this information includes the clients of the company, loyal clients, those that have premiums with the company and etc.
*/

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>

using namespace std;

//declaration of all the functions
void fillArray(string clientName[], int yrs[],string policy[], int premium[],int &numClient);
void printArrays(string clientName[], int yrs[], string policy[], int premium[], int &numClient);
void BSsort(string clientName[], int yrs[], string policy[], int premium[], int numClient);
void LoyalClient(string clientName[], int yrs[], string policy[], int premium[], int &numClient);
void showboats(string clientName[], int yrs[], string policy[], int premium[], int &numClient);
int highestpremium(string clientName[], int yrs[], string policy[], int premium[], int numClient);
void findclient(string clientName[], int numClient, string targetName);
int binarysearch(string clientName[], int numClient, string value);


const int maxClient = 50;


int main()
{
	//declaring the arrays and various other things that need declared
	string clientName[maxClient], policy[maxClient], targetName;
	int yrs[maxClient], premium[maxClient];
	int numClient = 0, hipre;

	fillArray(clientName, yrs, policy, premium, numClient);
	printArrays(clientName, yrs, policy, premium, numClient);
	BSsort(clientName, yrs, policy, premium, numClient);
	printArrays(clientName, yrs, policy, premium, numClient);
	LoyalClient(clientName, yrs, policy, premium, numClient);
	showboats(clientName, yrs, policy, premium, numClient);
	highestpremium(clientName, yrs, policy, premium, numClient);
	//printing out the highest premium witha value that was returned from the highestpremium function
	hipre = highestpremium(clientName, yrs, policy, premium, numClient);
	cout << endl << "The highest premium is " << premium[hipre] << endl;
	findclient(clientName, numClient, targetName);
	system("pause");
	return 0;

}
//this function fills the array with info from insur.txt
void fillArray(string clientName[], int yrs[], string policy[], int premium[], int &numClient)
{
	ifstream infile;
	infile.open("insur.txt");
	while (!infile.eof() && numClient<maxClient)
	{
		getline(infile, clientName[numClient]);
		getline(infile, policy[numClient]);
		infile >> yrs[numClient];
		infile >> premium[numClient];
		infile.ignore();
		numClient++;
		
	}

	infile.close();

}
//this function prints the info of the arrays, in the unsorted and then sorted form.
void printArrays(string clientName[], int yrs[], string policy[], int premium[], int &numClient)
{
	cout << left << setw(15) << "Clients" << setw(8) << "Years" << setw(8) << "Policy"
		<< setw(8) << "Premium" << endl;
	cout << setw(15) << "------" << setw(8) << "----" << setw(8) << "-----"
		<< setw(8) << "-------" << setw(8) << "---------" << endl;
	//making sure all the info is printed out
	for (int k = 0; k < numClient; k++)
	{
		cout << setw(15) << clientName[k] << setw(8) << yrs[k] << setw(8) << policy[k]
			<< setw(8) << premium[k]  << endl;
	}
	cout << endl << endl;
}
//this sorts the arrays into the correct order.
void BSsort(string clientName[], int yrs[], string policy[], int premium[], int numClient)
{

	int  itemp, ytemp;
	string stemp, ctemp;
	bool sort = false;

	//sorting all the arrays at the same time
	while (!sort)
	{ sort = true;
		
		for (int k = 0; k < (numClient - 1); k++)
		{
			
			if (clientName[k] > clientName[k + 1])
			{
				stemp = policy[k];
				policy[k] = policy[k + 1];
				policy[k + 1] = stemp;


				itemp = yrs[k];
				yrs[k] = yrs[k + 1];
				yrs[k + 1] = itemp;


				ytemp = premium[k];
				premium[k] = premium[k + 1];
				premium[k + 1] = ytemp;

				ctemp = clientName[k];
				clientName[k] = clientName[k + 1];
				clientName[k + 1] = ctemp;

				sort = false;
			}
		}
		numClient--;
	}
}
//this function finds out which client has been good, by being with the compan for 10 years or more.
void LoyalClient(string clientName[], int yrs[], string policy[], int premium[], int &numClient)
{
	cout << "Loyal Clients of 10 years or more: " << endl;
	for (int y = 0; y < numClient; y++)
	{
		if (yrs[y] >= 10)
		{
			cout <<	clientName[y] << endl;
			cout << endl;
		}
	}
}
//this shows everyone that has a boat policy and a premium of over 4800.
void showboats(string clientName[], int yrs[], string policy[], int premium[], int &numClient)
{
	cout << "Clients that have boats and a premium at or over 4800: " << endl;
	for (int a = 0; a < maxClient; a++)
	{
		if (policy[a] == "Boat" && premium[a] >= 4800)
		{
			cout <<	clientName[a] << endl;
			cout << endl;
		}
	}
}
//this shows who has the highest premium so the insurance company can know who to be nicest too.
int highestpremium(string clientName[], int yrs[], string policy[], int premium[], int numClient)
{
	int temp = 0;

	for (int i = 1; i<numClient; i++)
	{
		if (premium[i]>premium[temp])
			temp = i;
	}
	return temp;
}
//this finds the client at the exact position on the sorted list.
void findclient(string clientName[], int numClient, string targetName)
{
	int results;
//getting the name from the employee
	cout << "Enter the Client Name you wish to search for: ";
	getline(cin,targetName);
	results = binarysearch(clientName, numClient, targetName);
	if (results == -1)
		cout << "That name does not exist in the array.\n";
	else
	{
		//I'm not really sure if you want to find the name at the exact array placement starting from 0 or starting from 1 like normal, so i add 1 so that in the list provided it is the correct spot
		cout << "That name is found at element " << results +1;
		cout << " in the array.\n";
	}
}
//this search's for the client that was inputted in findClient, through binary search.
int binarysearch(string clientName[], int numClient, string value)
{
	{
		int  first = 0,                    // First array element
			last = numClient - 1,          // Last array element
			middle,                       // Midpoint of search
			position = -1;                // Position of search value
		bool found = false;                // Flag

		while (!found && first <= last)
		{
			middle = (first + last) / 2;    // Calculate midpoint
			if (clientName[middle] == value)     // If value is found at mid
			{
				found = true;
				position = middle;
			}
			else if (clientName[middle] > value) // If value is in lower half
				last = middle - 1;
			else
				first = middle + 1;          // If value is in upper half
		}
		return position;
	}
}
