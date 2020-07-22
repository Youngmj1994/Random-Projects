
#ifndef MY_STACK_
#define MY_STACK_


#include "LinkedStack.cpp"

template<class ItemType>
class MyStack : public LinkedStack<ItemType>
{
public:
	bool popmany(int n);
	int size();
};

#endif
