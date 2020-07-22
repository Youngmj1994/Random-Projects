
#include <iostream>
#include <string>
#include "LinkedStack.cpp"
#include "LinkedQueue.cpp"

bool isPalindrome(std::string str) {
   char tempQueue, tempStack;
   bool isPalindrome;
   LinkedStack<char> stack;
   LinkedQueue<char> queue;
   for(int i = 0; i<str.size(); i++)
   {
       stack.push(str[i]);
       queue.enqueue(str[i]);
   }
   if(stack.isEmpty() && !queue.isEmpty())
   return false;
   if(queue.isEmpty() && !stack.isEmpty())
   return false;
   while (!stack.isEmpty() && !queue.isEmpty())
   {
       tempQueue = queue.peekFront();
       queue.dequeue();
       tempStack = stack.peek();
       stack.pop();
       isPalindrome = (tempQueue == tempStack);
       if(isPalindrome == false)
       return isPalindrome;
       if(stack.isEmpty() && !queue.isEmpty())
  		 return false;
  		 if(queue.isEmpty() && !stack.isEmpty())
  		 return false;
   }
  return true;
}
