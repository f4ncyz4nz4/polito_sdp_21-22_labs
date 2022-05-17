#include <iostream>
#include "StackClass.h"
#include "StackClass.cpp"
#include <vector>

int main() {
    StackClass<int> myStack;
    int val;

    myStack.push(2);
    myStack.push(3);
    myStack.push(7);
    std::cout << myStack;


    // TEST POP AND EXCEPTION

    for(int i=0; i<4; i++){
        try {
            val = myStack.pop();
            std::cout << val << std::endl;
        }
        catch(const StackEmptyException& exception){
            std::cout << exception.what() << std::endl;
        }
    }


//TEST operator+
    StackClass<int> toAdd;
    toAdd.push(8);
    toAdd.push(9);
    toAdd.push(1);
    StackClass<int> concatenation;
    concatenation = myStack + toAdd;
    std::cout << concatenation;


    //test GETASVECTOR
    std::vector<int> dataVector;
    dataVector = myStack.getStackAsVector();
    std::cout << "v = { ";
    for (int n: dataVector) {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";

    //TEST copy constructor and assignment operator
    StackClass<int> stack1 = myStack; //copy constructor
    StackClass<int> stack2;
    stack2 = myStack; // assignment operator
    std::cout << "STACK 1" << std::endl;
    std::cout << stack1;
    std::cout << "STACK 2" << std::endl;
    std::cout << stack2;


// test REVERSE
    StackClass<int> reversedStack = myStack;
    reversedStack.reverse();
    std::cout << reversedStack;

    return 0;
}
