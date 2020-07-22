

#ifndef QUEUE_AS_A_STACK_
#define QUEUE_AS_A_STACK_
#include "QueueInterface.h"
#include "LinkedStack.cpp"


template<class ItemType>
class QueueAsAStack : public QueueInterface<ItemType>, private LinkedStack<ItemType>
{
public:
	/** Sees whether this queue is empty.
	  @return True if the queue is empty, or false if not. */
	bool isEmpty() const;

	/** Adds a new entry to the back of this queue.
	 @post If the operation was successful, newEntry is at the
		back of the queue.
	 @param newEntry  The object to be added as a new entry.
	 @return True if the addition is successful or false if not. */
	bool enqueue(const ItemType& newEntry);

	/** Removes the front of this queue.
	 @post If the operation was successful, the front of the queue
		has been removed.
	 @return True if the removal is successful or false if not. */
	bool dequeue();

	/** Returns the front of this queue.
	 @pre The queue is not empty.
	 @post The front of the queue has been returned, and the
		queue is unchanged.
	 @return The front of the queue. */
	ItemType peekFront() const;

	/** Destroys object and frees memory allocated by object. */
	~QueueAsAStack() {
		while (!isEmpty()) { dequeue(); }
	}

};
#endif