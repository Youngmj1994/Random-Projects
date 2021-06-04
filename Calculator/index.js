var storage = [];

function isNumeric(num){
    num = "" + num; //coerce num to be a string
    return !isNaN(num) && !isNaN(parseFloat(num));
  }

function pushToStorage(value) {
    if(storage.length < 0)
    {storage.push(value); return;}
    else if (isNumeric(storage[storage.length-1]) && isNumeric(value))
    {storage[storage.length-1] += value;}
    else 
    { storage.push(value);}
    
}

function clr() {
    document.getElementById("result").value = "";
}

function dis(val)
{
    document.getElementById("result").value+=val
}

function Add(x, y) {
    return x+y;
}
function Subtract(x,y) {
    return x-y;
}
function Multiply(x,y) {
    return x*y;
}
function Divide(x,y) {
    return x/y;
}
function Exponent(x,y)
{
    return Math.pow(x,y);
}
function printAll()
{
    storage.forEach(element => {
        console.log(element);
    });
    return;
}
function precedence(a)
{
    if(a == '^') {return 3;}
    else if(a == '*' || a == '/') {return 2;}
    else if(a == '+' || a == '-') {return 1;}
    else {return -1;}
}

//infix to postfix function, makes it easier for a computer to read to the calculator. 
function infix()
{
   postfix = [];
   stack = [];
   storage.forEach(element => {
       if(isNumeric(element))
       {postfix.push(element);}
       else if(stack.length == 0)
       {stack.push(element);}
       else if(element == '(')
       {stack.push(element);}
       else if(element == ')')
       {
           for(var i = stack.length -1; i > 0; i--)
           {
               if(stack[i] == '(')
               {break;}
               else 
               {postfix.push(stack.pop());}
           }
           stack.pop();
       }
       else if (precedence(stack[stack.length-1]) > precedence(element))
       { postfix.push(stack.pop()); stack.push(element); }
       else if (precedence(stack[stack.length-1]) < precedence(element))
       {stack.push(element);}
       else if (precedence(stack[stack.length-1]) == precedence(element))
       {postfix.push(stack.pop()); stack.push(element);}
   });

   while(stack.length > 0)
   {
       postfix.push(stack.pop());
   }

   result = eval(postfix);
   document.getElementById("result").value = result;
   return result;
}
function eval(output) 
{
    stack = [];
    x = 0;
    y = 0;
    
    output.forEach(element => 
    {
        if(isNumeric(element)){
            stack.push(parseInt(element));
        }
        else if(element == '+')
        {
            y = stack.pop();
            x = stack.pop();
            stack.push(Add(x,y));        
        }
        else if(element == '-')
        {
            y = stack.pop();
            x = stack.pop();
            stack.push(Subtract(x,y));        
        }
        else if(element == '*')
        {
            y = stack.pop();
            x = stack.pop();
            stack.push(Multiply(x,y));        
        }
        else if(element == '/')
        {
            y = stack.pop();
            x = stack.pop();
            stack.push(Divide(x,y));        
        }
        else if(element == '^')
        {
            y = stack.pop();
            x = stack.pop();
            stack.push(Exponent(x,y));        
        }       
    });
    storage = [];
    return stack[stack.length-1];
}