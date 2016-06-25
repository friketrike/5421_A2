#include <iostream>
#include "Command.h"

int main (){
    std::string something{"Bueno bueno, probando:"};
    std::cout << something;
    std::getline(std::cin, something); 
    std::cout << std::endl;
    std::cout << something << std::endl;
    Command command;
    command.setCommandBuffer(something);
    command.evaluateAddress();
}
