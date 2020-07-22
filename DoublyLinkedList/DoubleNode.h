
#ifndef DOUBLE_NODE_H_
#define DOUBLE_NODE_H_
#include<iostream>
template<class ItemType>
class DoubleNode
{
private:
	ItemType item;
	DoubleNode<ItemType>* prev;
	DoubleNode<ItemType>* next;

public:
	DoubleNode(const ItemType& anItem, DoubleNode<ItemType>* prevPtr, DoubleNode<ItemType>* nextPtr);
	~DoubleNode();

	ItemType getItem() const;
	DoubleNode<ItemType>* getPrev() const;
	DoubleNode<ItemType>* getNext() const;

	void setPrev(DoubleNode<ItemType>* prevPtr);
	void setNext(DoubleNode<ItemType>* nextPtr);
};

#endif
