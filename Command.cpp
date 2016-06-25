#include "Command.h"

// TODO first have a function that trims space


void Command::evaluateAddress(){
    auto it = commandBuffer.begin();
    std::size_t first = commandBuffer.find_first_of("0123456789");
    std::size_t last = commandBuffer.find_last_of("0123456789");
    // special find for .$n
    bool hasDigits = (first != std::string::npos);
    // TODO also check that the comma is surrounded or adjacent to the numbers
    std::size_t comma = commandBuffer.find_first_of(",");
    bool hasComma = (comma != std::string::npos);
    AddressRange range;

    if (hasDigits && hasComma) {
        if(first < comma) {
            // has a first number TODO just printing for now
            std::string sFirstNum = 
                commandBuffer.substr(first, comma-first);
            std::cout << sFirstNum << std::endl;
            //range.start{startAddr};
            //range.end{startAddr};
        }
        if (last > comma) {
            // has a second number
            std::string sLastNum = 
                commandBuffer.substr(comma+1, last-comma);
            std::cout << sLastNum  << std::endl;
            // range.end{startAddr};
        }
    }
    else if (hasDigits) {}
    else { // has comma or has nothing TODO figure out .$n
    // TODO fix this temp crap
    } 
}

void Command::evaluateCommand() {
    auto it = commandBuffer.begin();
    //std::size_t = commandBuffer;
    
}

void Command::setCommandBuffer(std::string& inputString){
    commandBuffer = inputString;
}
