
#include "DoubleNode.h"
#include<iostream>
template<class ItemType>
DoubleNode<ItemType>::DoubleNode(const ItemType& anItem, DoubleNode<ItemType>* prevPtr, DoubleNode<ItemType>* nextPtr) :
	item(anItem), prev(prevPtr), next(nextPtr)
{
}

template<class ItemType>
DoubleNode<ItemType>::~DoubleNode()
{
	this->prev = this->next = nullptr;
}

template<class ItemType>
ItemType DoubleNode<ItemType>::getItem() const
{
	return this->item;
}

template<class ItemType>
DoubleNode<ItemType>* DoubleNode<ItemType>::getPrev() const
{
	if (this->prev != nullptr)
		return this->prev;
	else
		return 0;
}

template<class ItemType>
DoubleNode<ItemType>* DoubleNode<ItemType>::getNext() const
{
	if (this->next != nullptr)
		return this->next;
	else
		return 0;
}

template<class ItemType>
void DoubleNode<ItemType>::setPrev(DoubleNode<ItemType>* prevPtr)
{
	this->prev = prevPtr;
}

template<class ItemType>
void DoubleNode<ItemType>::setNext(DoubleNode<ItemType>* nextPtr)
{
	this->next = nextPtr;
}
