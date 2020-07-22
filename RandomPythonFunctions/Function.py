import sys
print(string, file=sys.stderr)
Array = [10,14,11,16,13,12,15,17]


def InsertionSort(Array) :
    print (Array)
    for x in range (1,len(Array)):
        y=x
        
        while y>0 and Array[y-1]>Array[y]:
            Array[y],Array[y-1]=Array[y-1],Array[y]
            y=y-1
    print (Array)
    
         
def recursiveBinarySearch(Array, value, first=0, last = (len(Array) -1) ):
    if first > last:
        return False
    mid = (first + last) // 2
    if value == Array[mid]:
        return mid
    if value < Array[mid]:
        return  recursiveBinarySearch(Array, value, first, mid-1)
    value > Array[mid]
    return  recursiveBinarySearch(Array, value, mid+1, last)


def Splitter (sentence, char):
   newArray = []
   temp = ''
   for c in sentence:
       if c == char:
            newArray.append(temp)
            temp = ''
       else:
           temp += c
   if temp:
        newArray.append(temp)
   print(newArray)
            
InsertionSort(Array)
print ("Value is at ",  recursiveBinarySearch(Array, 15, 0, len(Array) -1), "in the array.")
Splitter('Kadabra', 'a')
input()