#include "Command.h"

// using namespace std;
// TODO first have a function that trims space

//void Command::
Command::AddressRange& Command::AddressRange::operator=(
        const AddressRange& rhs) {
    if(this != &rhs){
        start=rhs.start; 
        end=rhs.end;
    }
    return *this;
}

void Command::removeWhiteSpace(string& s) {
    size_t foundSpace = s.find_first_of(" \t");
    auto it = s.begin();
    while (foundSpace != string::npos) {
        s.erase(foundSpace, 1);
        foundSpace = s.find_first_of(" \t");
    }
}

bool Command::parse(string& commandBuffer, 
        const size_t& currentLine, const size_t& totalLines){
    removeWhiteSpace(commandBuffer);

    const string validCharacters = 
        "iarpncudw=0123456789$,";
    size_t invalid = commandBuffer.find_first_not_of(validCharacters);
    if (invalid != string::npos) {
        return INVALID;
    }
    
    // TODO remove space from commandBuffer
    ar = AddressRange(currentLine); // set default

    // find digits
    size_t firstNum = commandBuffer.find_first_of("0123456789");
    size_t lastNum = commandBuffer.find_last_of("0123456789");
    bool hasDigits = (firstNum != std::string::npos);
    
    // find special address characters
    size_t firstSpecial = commandBuffer.find_first_of("$");
    size_t lastSpecial = commandBuffer.find_last_of("$");
    // special find for .$n
    bool hasSpecial = (firstSpecial != string::npos);

    size_t comma = commandBuffer.find_first_of(",");
    size_t check = commandBuffer.find_last_of(",");
    // make sure we have at most one comma
    if (comma != check) {return INVALID;}
    bool hasComma = (comma != string::npos);
    // find the command now
    size_t commandChar = commandBuffer.find_first_of("iarpncudw=");
    check = commandBuffer.find_last_of("iarpncudw="); 
    // make sure we have at most one command
    if (commandChar != check) {return INVALID;}
    bool hasCommandChar = (commandChar != string::npos);
    if (hasCommandChar) {
        ct = (CommandType) commandBuffer[commandChar];
    }
    else {
        ct = print;
    }
    //AddressRange range;
// conditions special, num, comma || special, comma || num, comma 
// || comma || special || comma
// Check that comma appears once, check that there's only one special before or after the comma
    if (hasDigits && hasSpecial && hasComma) {
        // special, comma, digit -- makes no sense
        // digits, comma, special
        if(((firstSpecial < comma) || (lastNum > comma))
                || (hasCommandChar && commandChar < lastSpecial)) {
            return INVALID;
        }                
        string sNum = 
            commandBuffer.substr(firstNum, lastNum-firstNum);
        stringstream sstream(sNum);
        sstream >> ar.start;
        ar.end = totalLines;
    }

    else if (hasSpecial && hasComma) {
        if (hasCommandChar && commandChar < lastSpecial) {
            return INVALID;
        }                
        else if (firstSpecial == (comma - 1)) {
            ar.start = totalLines;
        }
        ar.end = totalLines;
    }

    else if (hasDigits && hasComma) {
        if (hasCommandChar && commandChar < lastNum) {
            return INVALID;
        }
        
        else {
            bool hasFirst = false;
            bool hasSecond = false;
            if (firstNum < comma) {
                hasFirst = true;
                string sNum = 
                    commandBuffer.substr(firstNum, comma-firstNum);
                stringstream sstream(sNum);
                sstream >> ar.start;
            }
            if (lastNum > comma) {
                hasSecond = true; 
                string sNum = 
                    commandBuffer.substr(comma+1, lastNum-comma);
                stringstream sstream(sNum);
                sstream >> ar.end;
            }
            /*if (hasFirst, !hasSecond) {
                ar.end = ar.start;
            }*/
        }
    }
    else if (hasDigits && hasSpecial) {
        return INVALID;
    }
    else if (hasDigits) { 
        if (hasCommandChar && commandChar < lastNum) {
            return INVALID;
        }
        
        else {
            // has a first number 
            string sNum = 
                commandBuffer.substr(firstNum, lastNum-(firstNum-1));
            stringstream sstream(sNum);
            sstream >> ar.start;
            ar.end = ar.start;
        }
    }
    else if (hasSpecial) {
        if ((firstSpecial != lastSpecial) 
            || (hasCommandChar && (commandChar < lastSpecial))) {
            return INVALID;
        }
        
        else {
            ar.start = totalLines;
            ar.end = ar.start;
        }
    }
    else { // only has comma or has nothing for address
        // TODO figure out commands where addressing changes
        // special hasComma !hasCommandChar 1,$p
        // special !hasComma !hasCommandChar 1d
        if (hasComma && hasCommandChar && (commandChar < comma)) {
            return INVALID;
        }
        else {
            if (hasComma && !hasCommandChar) {
                ar.start = 1;
                ar.end = totalLines;
                ct = print;
            }
            else if (!hasComma && !hasCommandChar) {
                ar.start = 1;
                ar.end = 1;
                ct = down;
            }
            else if (!hasCommandChar) {
                ct = print;
            }            
        }
    } 
    // TODO remove debug
    cout << "Address range start: " << ar.start;
    cout << ", and end: " << ar.end;
    cout << ", with command: " << (char) ct << endl;
    // end
    // if we got this far, and the address range is valid, we're ok
    bool rangeIsValid = (ar.start > 0 &&  ar.start 
                <= ar.end && ar.end <= totalLines);
    if (!rangeIsValid) {
        return INVALID;
    }
    else {
        return VALID;
    }
}

const Command::AddressRange& Command::getAddressRange() const{
    return this->ar;
}

const Command::CommandType& Command::getCommandType() const{
    return this->ct;
}
