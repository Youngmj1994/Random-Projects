// This program demonstrates a simple append
// operation on a linked list.
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
struct Room {
	int roomid;
	int cap;
	int occ;
	Room *nextroom;
};

int getChoice();
Room *createroom();
void addfrontroom(Room *&firstroom, Room *newRoom);
void printrooms(Room *firstroom);
void addendroom(Room *&firstroom, Room *newRoom);
void removeroom(Room *&firstroom, int targetid);
void destroyList(Room *&firstroom);
void hm(Room *&firstroom);

int main()
{
	int choice, targetid;
	Room *firstroom = NULL;
	Room *newRoom = NULL;
	cout << left;
	// Display the menu and process the user's choice.
	choice = getChoice();
	while (choice != 7)
	{
		if (choice == 1)
		{
			newRoom = createroom();
			addfrontroom(firstroom, newRoom);
		}
		else if (choice == 2)
		{
			newRoom = createroom();
			addendroom(firstroom, newRoom);
		}
		else if (choice == 3)
		{
			cout << "Enter Roonm ID:";
			cin >> targetid;
			removeroom(firstroom, targetid);
		}
		else if (choice == 4)
			hm(firstroom);
		else if (choice == 5)
			printrooms(firstroom);
		else if (choice == 6)
			destroyList(firstroom);
		choice = getChoice();
	}
	return 0;
}

int getChoice()
{
	// The user's choice
	int choice;

	cout << endl;
	cout << "1: Add Room to Front" << endl;
	cout << "2: Add Room to Rear" << endl;
	cout << "3: Remove a Room" << endl;
	cout << "4: Hm" << endl;
	cout << "5: Display All Rooms" << endl;
	cout << "6: Destroy All Rooms" << endl;
	cout << "7: Quit" << endl;
	// Get the user's choice.
	cout << "Choose one: ";
	cin >> choice;

	// Validate the choice.
	while (choice < 1 || choice > 7)
	{
		cout << "Bad choice. Choose one: ";
		cin >> choice;
	}

	// Return the choice, as a subscript into
	// the array of drinks.
	return choice;
}
Room *createroom()
{
	Room *newRoom = new Room;

	cout << "Enter Data -  ID Cap Occ: ";
	cin >> newRoom->roomid >> newRoom->cap >> newRoom->occ;
	newRoom->nextroom = NULL;

	return newRoom;
}


void addfrontroom(Room *&firstroom, Room *newRoom)		// add a node
{
	newRoom->nextroom = firstroom;
	firstroom = newRoom;
}
void printrooms(Room *firstroom)		// print the rooms
{
	Room *tptr = firstroom;

	if (!tptr)
		cout << "The list is empty" << endl;
	else
	{
		cout << setw(20) << "The Room List" << endl;
		cout << endl << "**********************************************************" << endl;
		cout << setw(7) << "ID" << setw(12) << "Capacity" << setw(12) << "Occupancy" << endl;
		cout << setw(7) << "--" << setw(12) << "--------" << setw(12) << "---------" << endl;

		while (tptr)
		{
			cout << setw(7) << tptr->roomid << setw(12) << tptr->cap << setw(12) << tptr->occ << endl;
			tptr = tptr->nextroom;
		}
		cout << endl << "**********************************************************" << endl;
	}
}

void addendroom(Room *&firstroom, Room *newRoom)		// Adding nodes to the end of the list
{
	Room *tptr = NULL;

	if (!firstroom)
		firstroom = newRoom;
	else
	{
		tptr = firstroom;
		while (tptr->nextroom)
			tptr = tptr->nextroom;
		tptr->nextroom = newRoom;
	}
}

void removeroom(Room *&firstroom, int targetid)
{
	Room *nodePtr;       // To traverse the list
	Room *previousNode;  // To point to the previous node

	// If the list is empty, do nothing.
	if (!firstroom)
	{
		cout << "The list is empty" << endl;
		return;
	}
	// Determine if the first node is the one.
	if (firstroom->roomid == targetid)
	{
		nodePtr = firstroom;
		firstroom = firstroom->nextroom;
		delete nodePtr;
		cout << "Room: " << targetid << " has been removed." << endl;

	}
	else
	{
		// Initialize nodePtr to head of list
		nodePtr = firstroom;

		// Skip all nodes whose value member is 
		// not equal to num.
		while (nodePtr != NULL && nodePtr->roomid != targetid)
		{
			previousNode = nodePtr;
			nodePtr = nodePtr->nextroom;
		}
		// If nodePtr is not at the end of the list, 
		// link the previous node to the node after
		// nodePtr, then delete nodePtr.
		if (nodePtr)
		{
			previousNode->nextroom = nodePtr->nextroom;
			delete nodePtr;
			cout << "Room: " << targetid << " has been removed." << endl;
		}
		else
			cout << "Your value was not on the list" << endl;
	}
}
void destroyList(Room *&firstroom)
{
	Room *holdPtr;   // To hold the remaining  list



	// While firstroom is not at the end of the list...
	while (firstroom)
	{
		// Save a pointer to the next node.
		holdPtr = firstroom->nextroom;

		// Delete the head node.
		delete firstroom;

		// Position firstroom at the next node.
		firstroom = holdPtr;
	}

	cout << "The list is now empty" << endl;
}
void hm(Room *&firstroom)
{
	Room *nodePtr;       // To traverse the list
	Room *previousNode;  // To point to the previous node

	// If the list is empty, do nothing.
	if (!firstroom)
	{
		cout << "The list is empty" << endl;
		return;
	}
	// Determine if the first node is the one.
	if (!firstroom->nextroom)
	{
		delete firstroom;
		firstroom = NULL;
		cout << "The last room has been removed." << endl;
	}
	else
	{
		// Initialize nodePtr to head of list
		nodePtr = firstroom;

		// Skip to the last node 
		while (nodePtr->nextroom)
		{
			previousNode = nodePtr;
			nodePtr = nodePtr->nextroom;
		}
		previousNode->nextroom = NULL;
		delete nodePtr;
		cout << "The last room has been removed." << endl;
	}
}