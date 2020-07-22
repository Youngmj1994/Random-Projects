
#ifndef DOUBLE_LIST_
#define DOUBLE_LIST_
#include "DoubleListInterface.h"
#include "DoubleNode.cpp"
#include<iostream>

template<class ItemType>
class DoubleList : public DoubleListInterface<ItemType>
{
private:
	DoubleNode<ItemType>* headPtr;
	DoubleNode<ItemType>* tailPtr;
	int itemCount;
	DoubleNode<ItemType>* getNodeAt(int position) const;

public:

	DoubleList() : headPtr(nullptr), itemCount(0), tailPtr(nullptr) {};
	~DoubleList() { clear(); }

	/** Sees whether this list is empty.
		 @return True if the list is empty; otherwise returns false. */
	bool isEmpty() const;

	/** Gets the current number of entries in this list.
	  @return The integer number of entries currently in the list. */
	int getLength() const;

	/** Inserts an entry into this list at the front.
	  @pre  None.
	  @post  If the insertion is successful, newEntry is at the front of
		the list, other entries are renumbered accordingly, and the returned value
		is true.
	  @param newEntry  The entry to insert into the list.
	  @return  True if insertion is successful, or false if not. */
	bool insertFront(const ItemType& newEntry);

	/** Inserts an entry into this list at the back.
	   @pre  None.
	   @post  If the insertion is successful, newEntry is at the back of
		 the list, other entries are renumbered accordingly, and the returned value
		 is true.
	   @param newEntry  The entry to insert into the list.
	   @return  True if insertion is successful, or false if not. */
	bool insertBack(const ItemType& newEntry);

	/** Removes the entry at a given position from this list.
	  @pre  None.
	  @post  If 1 <= position <= getLength() and the removal is successful,
		the entry at the given position in the list is removed, other
		items are renumbered accordingly, and the returned value is true.
	  @param position  The list position of the entry to remove.
	  @return  True if removal is successful, or false if not. */
	bool remove(int position);

	/** Removes all entries from this list.
	  @post  List contains no entries and the count of items is 0. */
	void clear();

	/** Gets the entry at the given position in this list.
	  @pre  1 <= position <= getLength().
	  @post  The desired entry has been returned.
	  @param position  The list position of the desired entry.
	  @return  The entry at the given position. */
	ItemType getEntry(int position) const;
};

#endif 
