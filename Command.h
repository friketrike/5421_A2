#ifndef LED_COMMAND
#include <iostream>
#include <string>
#include <cstddef>

class Command{
  public:
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
    CommandType mCommandType{notRecognized};

    struct AddressRange {
        std::size_t start{1};
        std::size_t end{1};
        AddressRange()=default;
        AddressRange(std::size_t s, std::size_t e)
            : start(s)
            , end(e)
        { }
        virtual ~AddressRange()=default;
    };
    AddressRange mAddressRange;

  private:
    std::string commandBuffer;

  public:
    Command()=default;
    virtual ~Command()=default;
    void setCommandBuffer(std::string &); 
    void evaluateAddress();
    void evaluateCommand();
};

#endif 
