#ifndef LED_COMMAND
#include <iostream>
#include <string>
#include <cstddef>
#include <sstream>   
//#include <algorithm> // std::remove
using namespace std;

class Command{

  public:
    // TODO probably can do without this, but maybe it's actually useful
    enum CommandType {
        insert = 'i', 
        append = 'a', 
        remove = 'r', 
        print = 'p', 
        numberPrint = 'n', 
        change = 'c',
        up = 'u', 
        down = 'd', 
        write = 'w', 
        printCurrLine = '=', 
        notRecognized
    };

    struct AddressRange {
        size_t start{0};
        size_t end{0};
        AddressRange()=default;
        AddressRange(const size_t& s, const size_t& e)
            : start(s)
            , end(e)
        { }
        AddressRange(const size_t& s)
            : start(s)
            , end(s)
        { }
        AddressRange& operator=(const AddressRange& ar); 
        virtual ~AddressRange()=default;
    };

  private:
    static const bool VALID = true;
    static const bool INVALID = false;
    CommandType ct{notRecognized};
    AddressRange ar;
    // string commandBuffer;

    void removeWhiteSpace(string& s);

  public:
    Command()=default;
    virtual ~Command()=default;
    bool parse(string& commandBuffer, const size_t& currentLine, 
            const size_t& totalLines);
    const AddressRange& getAddressRange() const;
    const CommandType& getCommandType() const;
};

#endif 
