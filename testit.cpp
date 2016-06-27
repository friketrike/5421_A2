#include <iostream>
#include "Command.h"

int main (){
    std::string something{"Bueno bueno, probando:"};
    std::cout << something;
    std::getline(std::cin, something); 
    std::cout << std::endl;
    std::cout << something << std::endl;
    Command command;
    bool valid = command.parse(something, 2, 3);
    if (valid)
        cout << "it's valid" << endl;
    else
        cout << "it's invalid" << endl;
//    command.evaluateAddress();
}
