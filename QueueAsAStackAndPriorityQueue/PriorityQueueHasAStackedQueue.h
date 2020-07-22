

#ifndef PRIORITY_QUEUE_HAS_A_STACKED_QUEUE_
#define PRIORITY_QUEUE_HAS_A_STACKED_QUEUE_
#include "PriorityQueueInterface.h"
#include "QueueAsAStack.cpp"

template<class ItemType>
class PriorityQueueHasAStackedQueue : public PriorityQueueInterface<ItemType>
{
private:
	QueueAsAStack<ItemType>* item;
public:
	/** Sees whether this priority queue is empty.
	 @return  True if the priority queue is empty, or false if not. */
	bool isEmpty() const;
	
	/** Adds a new entry to this priority queue.
	 @post  If the operation was successful, newEntry is in the
		priority queue.
	 @param newEntry  The object to be added as a new entry.
	 @return  True if the addition is successful or false if not. */
	bool enqueue(const ItemType& newEntry);

	/** Removes from this priority queue the entry having the
		highest priority.
	 @post  If the operation was successful, the highest priority
		entry has been removed.
	 @return  True if the removal is successful or false if not. */
	bool dequeue();

	/** Returns the highest-priority entry in this priority queue.
	 @pre  The priority queue is not empty.
	 @post  The highest-priority entry has been returned, and the
		priority queue is unchanged.
	 @return  The highest-priority entry. */
	ItemType peek() const;

	/** Destroys object and frees memory allocated by object. */
	~PriorityQueueHasAStackedQueue() {
		while (!isEmpty())
			dequeue();
	};
}; // end PriorityQueueInterface
#endif