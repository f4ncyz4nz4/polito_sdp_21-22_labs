#include <iostream>
#include "Queue.h"

void printClient(enum Client client){
    switch(client){
        case PRIORITY:
            std::cout << "PRIORITY" << std::endl;
            break;
        case POSTAL:
            std::cout << "POSTAL" << std::endl;
            break;
        case MONEY:
            std::cout << "MONEY" << std::endl;
            break;
    }
}

int main() {
    Queue q;
    int input;
    enum Client client;
    /* TEST without console interaction
    for(int i = 0; i < 3; i++){
        q.push(PRIORITY);
    }
    for(int i = 0; i < 5; i++){
        q.push(MONEY);
    }
    q.push(POSTAL);
    q.printQueue();
     */
    do{
        std::cout << "1. Request a new number for a client" << std::endl;
        std::cout << "2. Serve next client" << std::endl;
        std::cout << "3. Serve all and close the office" << std::endl;
        std::cin >> input;
        if(input == 1) {
            int c, clientNumber;
            std::cout << "Insert client type: 1 (PRIORITY), 2 (POSTAL), 3 (MONEY)" << std::endl;
            std::cin >> c;
            client = (enum Client) c;
            clientNumber = q.push(client) + 1; //+1 because indexes starts from zero
            std::cout << "Client number: " << clientNumber << std::endl;
        }
        else if(input == 2) {
            if(!q.empty()){
                client = q.pop();
                std::cout << "Client served: ";
                printClient(client);
            }
            else{
                std::cout << "There is no client!" << std::endl;
            }
        }
        std::cout << std::endl;
    } while(input != 3);

    while(!q.empty()){
        client = q.pop();
        std::cout << "Client served: ";
        printClient(client);
    }
    std::cout << "The office was closed" << std::endl;

    return 0;
}
