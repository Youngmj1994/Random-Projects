

#include "PriorityQueueHasAStackedQueue.h"
#include <vector>
#include <algorithm>

template<class ItemType>
bool PriorityQueueHasAStackedQueue<ItemType> ::isEmpty() const
{
	return item->isEmpty();
}

template<class ItemType>
bool  PriorityQueueHasAStackedQueue<ItemType> ::enqueue(const ItemType& newEntry)
{
	if (item->isEmpty())
	{
		item->enqueue(newEntry);
		return true;
	}
	item->enqueue(newEntry);
	int i = 0, x = 0;
	std::vector <ItemType> temp;
	for (i; !isEmpty(); i++)
	{
		temp[i] = item->peekFront();
		item->dequeue();
	}
	//sort it into descending order so it can easily be transfered back into the queue.
	std::sort(temp.end(), temp.begin());
	//for this, you have to create a temporary array, then sort it into largest on top and 
	//smallest on bottom, so that its a priority queue, then put it back in from the front, since
	//the first item in is the first one interacted with. 
	for (x; x < temp.size(); x++)
	{
		item->enqueue(temp[x]);
	}
	return true;
}

template<class ItemType>
bool  PriorityQueueHasAStackedQueue<ItemType> ::dequeue()
{
	return item->dequeue();
}

template<class ItemType>
ItemType  PriorityQueueHasAStackedQueue<ItemType> ::peek() const
{
	return item->peekFront();
}