

#include "DoubleList.h"
#include<iostream>

//inserts at the front of the list moving headptr forward
template<class ItemType>
bool DoubleList<ItemType>::insertFront(const ItemType& newEntry)
{
  DoubleNode<ItemType>* tempNode = nullptr;
  tempNode = new  DoubleNode<ItemType>(newEntry, tempNode->getPrev(), tempNode->getNext());
  
  if(tempNode == NULL)
	return false;
  itemCount++;
  if(headPtr==NULL)
  {
	  headPtr=tailPtr=tempNode;
	 
	  return true;
  }
  
	tempNode->setNext(headPtr);
	headPtr->setPrev(tempNode);
	headPtr = tempNode;
    
    return true; 
}

//inserts to the back of the list using the tailptr to insert from the back easily
template<class ItemType>
bool DoubleList<ItemType>::insertBack(const ItemType& newEntry)
{
    
	DoubleNode<ItemType>* tempNode = nullptr;
	tempNode = new  DoubleNode<ItemType>(newEntry, tempNode->getPrev(), tempNode->getNext());
	
	if(tempNode == NULL)
	return false;
	itemCount++;
	if(headPtr==NULL)
	{
		tailPtr = headPtr = tempNode;
		
		return true;
	}
	    tailPtr->setNext(tempNode);
	    tempNode->setPrev(tailPtr);
	    tempNode->setNext(NULL);
	    tailPtr = tempNode;
	    
	    return true;
}

//checks if empty
template<class ItemType>
bool DoubleList<ItemType>::isEmpty() const
{
    return itemCount == 0;
}


//returns class variable itemCount which is the length of the list
template<class ItemType>
int DoubleList<ItemType>::getLength() const
{
    return itemCount;
}

//removes an item from the position and moves the ptr accordingly
template<class ItemType>
bool DoubleList<ItemType>::remove(int position)
{
   bool ableToRemove = (position >= 1) && (position <= getLength());
   if (ableToRemove)
   {
      DoubleNode<ItemType>* curPtr = nullptr;
      if (position == 1)
      {
         curPtr = headPtr; 
         headPtr = headPtr->getNext();
      }
      else
      {
         DoubleNode<ItemType>* prevPtr = getNodeAt(position - 1);
         curPtr = prevPtr->getNext();
         prevPtr->setNext(curPtr->getNext());
      } 
      curPtr->setNext(nullptr);
      delete curPtr;
      curPtr = nullptr;
   }
   itemCount--;
   return ableToRemove;
}

//removes the entire list
template<class ItemType>
void DoubleList<ItemType>::clear()
{
    while(getLength() != 0)
    remove(1);
    itemCount = 0;
}

//returns the item at the position using the getNodeAt function to help
template<class ItemType>
ItemType DoubleList<ItemType>::getEntry(int position) const
{
     DoubleNode<ItemType>* nodePtr = getNodeAt(position);
     return nodePtr->getItem();
}

//done
template<class ItemType>
DoubleNode<ItemType>* DoubleList<ItemType>::getNodeAt(int position) const
{
   DoubleNode<ItemType>* curPtr = headPtr;
   for (int i = 1; i < position; i++)
      curPtr = curPtr->getNext();
      
   return curPtr;
}
