
#include "MyStack.h"


// pop the top 'n' items off the stack
// returns true if it was able to, otherwise false
// this function should not throw any exceptions
template<class ItemType>
bool MyStack<ItemType>::popmany(int n) {
	int num = n;
	bool value = false;
	while (num > 0 && !this->isEmpty())
	{
		this->pop();
		num--;
	}
	if (num == 0)
		value = true;

	return value;
}

// returns the total number of items on the stack
template<class ItemType>
int MyStack<ItemType>::size() {
	MyStack <ItemType> temp;
	ItemType holder;
	//create temp stack
	//while not is empty peek to get item then pop from original counting up
	//then push the values from the temp stack onto the original stack
	int count = 0;

	for (int i = 0; !this->isEmpty(); i++)
	{
		holder = this->peek();
		temp.push(holder);
		this->pop();
		count++;
	}

	for (int x = 0; x < count; x++)
	{
		this->push(temp.peek());
		temp.pop();
	}
	return count;
}
