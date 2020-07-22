

#include "QueueAsAStack.h"

template<class ItemType>
bool QueueAsAStack<ItemType> ::isEmpty() const
{
	return this->isEmpty();
}

//maybe
template<class ItemType>
bool QueueAsAStack<ItemType> ::enqueue(const ItemType& newEntry)
{
	//create a new queue object and throw eveything from LinkedStack into it. making it a queue.
 //if the queue is empty then just push the item on and return TRUE
	QueueAsAStack <ItemType> * temp2;

	if (this->isEmpty())
	{
		this->push(newEntry);
		return true;
	}

	while (!this->isEmpty())
	{
		temp2->push(this->peek());
		this->pop();
	}

	this->push(newEntry);

	while (!temp2->isEmpty())
	{
		this->push(temp2->peek());
		temp2->pop();
	}
	return true;
}

//done 
template<class ItemType>
bool QueueAsAStack<ItemType> ::dequeue()
{
	//call regular pop
	return this->pop();

}

//done
template<class ItemType>
ItemType QueueAsAStack<ItemType> ::peekFront() const
{
	return this->peek();
}
