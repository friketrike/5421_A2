/*
 * Assignennt 2, COMP 5421, summer 2016  
 * Federico O'Reilly Regueiro 40012304
 * Concordia University
 * 
 * Command implementation file
 */

#include "Command.h"

const string Command::VALID_COMMAND_CHAR = "iarpncudwqoh=";
const string Command::VALID_ADDR_CHAR = "0123456789";
const string Command::SPECIAL_ADDR_CHAR = "$";
const string Command::SEPARATOR = ",";
const string Command::VALID_CHAR = Command::VALID_COMMAND_CHAR 
    + Command::VALID_ADDR_CHAR + Command::SPECIAL_ADDR_CHAR 
    + Command::SEPARATOR;

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
    while (foundSpace != string::npos) {
        s.erase(foundSpace, 1);
        foundSpace = s.find_first_of(" \t");
    }
}

bool Command::parse(string& commandBuffer, 
        const size_t& currentLine, const size_t& totalLines){
    removeWhiteSpace(commandBuffer);
    cs = validCommand;

    size_t invalidCharPos = commandBuffer.find_first_not_of(VALID_CHAR);
    if (invalidCharPos != string::npos) {
        cs = invalidChars;
        return INVALID;
    }
    
    ar = AddressRange(currentLine); // set default

    // find digits
    size_t firstNum = commandBuffer.find_first_of(VALID_ADDR_CHAR);
    size_t lastNum = commandBuffer.find_last_of(VALID_ADDR_CHAR);
    bool hasDigits = (firstNum != std::string::npos);
    
    // find special address characters
    size_t firstSpecial = commandBuffer.find_first_of(SPECIAL_ADDR_CHAR);
    size_t lastSpecial = commandBuffer.find_last_of(SPECIAL_ADDR_CHAR);
    // special find for $
    bool hasSpecial = (firstSpecial != string::npos);

    size_t separator = commandBuffer.find_first_of(SEPARATOR);
    size_t check = commandBuffer.find_last_of(SEPARATOR);
    // make sure we have at most one comma
    if (separator != check) {
        cs = invalidSyntax; 
        return INVALID;
    }
    bool hasSeparator = (separator != string::npos);
    // find the command now
    //TODO replace commandChar by commandCharPos
    size_t commandChar = commandBuffer.find_first_of(VALID_COMMAND_CHAR);
    check = commandBuffer.find_last_of(VALID_COMMAND_CHAR); 
    // make sure we have at most one command
    if (commandChar != check) {
        cs = invalidSyntax;
        return INVALID;
    }
    bool hasCommandChar = (commandChar != string::npos);
    
    // if the command is not at the end, leave
    if (hasCommandChar && (commandChar < (commandBuffer.length()-1)) ) { 
        cs = invalidSyntax;
        return INVALID;
    }

    if (hasCommandChar) {
        ct = (CommandType) commandBuffer[commandChar];
    }
    else {
        ct = print; // default command
    }
    // Default range and command are set, now evaluate for all combinations
    // of having or not  separator, special-end-char and digits
    if (!hasSeparator && !hasSpecial && !hasDigits) {
        if (!hasCommandChar) {
            ar.start = 1;
            ar.end = 1;
            ct = down;
        } // else, keep defaults and we're ok
    }
    else if (hasDigits && !hasSeparator && !hasSpecial) { 
        string sNum = 
            commandBuffer.substr(firstNum, lastNum-(firstNum-1));
        stringstream sstream(sNum);
        sstream >> ar.start;
        ar.end = ar.start;
    }
    else if (hasSpecial && !hasDigits && !hasSeparator) {
        if (firstSpecial != lastSpecial) { 
            cs = invalidSyntax;
            return INVALID;
        }
        else {
            ar.start = totalLines;
            ar.end = totalLines;
        }
    }
    else if (hasSeparator && !hasDigits && !hasSpecial) {
        if (!hasCommandChar) {
            // special case expanding to 1,$p, p is already set 
            // from first condition in the decission tree        
            ar.start = 1;
            ar.end = totalLines;
        } // else keep default of curr,curr
    }
    else if(hasDigits && hasSpecial && !hasSeparator) {
        cs = invalidSyntax;
        return INVALID;
    }
    else if (hasSpecial && hasSeparator && !hasDigits) {
        if ((lastSpecial - firstSpecial) > 2) {
            cs = invalidSyntax;
            return INVALID;
        } else { // at least one of these must be true
            if (firstSpecial == (separator - 1)) {
                ar.start = totalLines;
            }
            if (lastSpecial == separator+1) {
                ar.end = totalLines;
            }
        }
    }
    else if (hasDigits && hasSeparator && !hasSpecial) { 
        // at least one of the following must be true
        if (firstNum < separator) {
            string sNum = 
                commandBuffer.substr(firstNum, separator-firstNum);
            stringstream sstream(sNum);
            sstream >> ar.start;
        }
        if (lastNum > separator) {
            string sNum = 
                commandBuffer.substr(separator+1, lastNum-separator);
            stringstream sstream(sNum);
            sstream >> ar.end;
        }
    }
    else if (hasDigits && hasSpecial && hasSeparator) {
        if((firstNum < separator) && (lastNum > separator)){ 
            cs = invalidSyntax;
            return INVALID;
        } else {               
            string sNum = 
                commandBuffer.substr(firstNum, lastNum-(firstNum-1));
            stringstream sstream(sNum);
            sstream >> ar.start;
            if (firstNum < separator) {
                ar.end = totalLines;
                sstream >> ar.start;
            } else { // allow super strange $,n syntax, maybe not a good idea
                ar.start = totalLines;
                sstream >> ar.end;
                if (ar.end < totalLines) {
                    cs = invalidRange;
                    return INVALID;
                }
            }
        }
    }
    // if we got this far, and the address range is valid, we're ok
     bool rangeIsValid = true;
    // u d w q = and o don't need the address range, otherwise validate
    if (ct != up && ct != down && ct != write && ct != open
            && ct != quit && ct != printCurrLine && ct != help) {
        rangeIsValid = (ar.start > 0 &&  ar.start 
                <= ar.end && ar.end <= totalLines);
    }
    if (!rangeIsValid) {
        cs = invalidRange;
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

const Command::CommandStatus& Command::getCommandStatus() const{
    return this->cs;
}
