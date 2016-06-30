/*
 * Assignennt 2, COMP 5421, summer 2016  
 * Federico O'Reilly Regueiro 40012304
 * Concordia University
 * 
 * Command header file
 */


#ifndef LED_COMMAND
#define LED_COMMAND
#include <iostream>
#include <string>
#include <cstddef>
#include <sstream>   

using namespace std;
/**
 * Command class used by LED for parsing user input.
 * Contains members and methods for parsing and validating user input.
 */
class Command{

  public:
    /**
     * enum listing all valid commands accepted by the line editor.
     * Used to pass the command type (in more of a human-readable fashion) 
     * to the caller.
     */
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
        quit = 'q',
        open = 'o',
        help = 'h',
        notRecognized = 'z'
    };
    /** 
     * enum listing the validity status of parsed commands.
     * Used to pass the important information to the caller which can, in turn,
     * either execute a valid command or inform the user regarding errors.
     */
    enum CommandStatus {
        validCommand = 0,
        invalidChars,
        invalidSyntax,
        invalidRange
    };
    /**
     * An address range structure containing a start end end line address.
     * Used to pass command-addressing information to the caller.
     */
    struct AddressRange {
        size_t start; /**<range-start*/
        size_t end; /**<range-end*/
        /**
         * Default constructor with an optional parameter.
         * @param s is both the starting and ending line-addresses.
         * s defaults to 0
         */
        AddressRange(const size_t& s = 0)
            : start(s)
            , end(s)
        { }
        /**
         * Constructor with both a start and end parameters.
         * @param s is the start line-address of the specified range.
         * @param e is the end line-address of the specified range.
         */
        AddressRange(const size_t& s, const size_t& e)
            : start(s)
            , end(e)
        { }
        /**
         * Overloaded assignment operator.
         * Useful when address range is not specified in the command as
         * the received currentLine is assigned to both start and end.
         */
        AddressRange& operator=(const AddressRange& ar); 
        /**
         * Explicit request for a default destructor
         */
        virtual ~AddressRange()=default;
    };

  private:
    static const bool VALID = true; /**<makes reading exit points clearer*/
    static const bool INVALID = false; /**<makes reading exit points clearer*/


    // static const strings need to be defined outside of the class
    // see implementation file
    // The definition of these strings makes changing commands a bit easier
    // as well as reading the code a bit clearer (I hope)
    static const string VALID_COMMAND_CHAR; /**<Chars used in valid commands*/
    static const string VALID_ADDR_CHAR;/**<Chars used for address ranges*/
    static const string SPECIAL_ADDR_CHAR;/**<Chars that expand to addresses*/
    static const string SEPARATOR;/**<The character used as a s,e separator*/
    static const string VALID_CHAR;/**<All of the characters that can be used*/

    CommandType ct{notRecognized}; /**<used for passing CommandType to caller*/

    AddressRange ar; /**<Used for passing the range for the next command*/
    // until shown otherwise, initialize as valid
    CommandStatus cs{validCommand}; /**<Command validation information*/ 
    
    /**
     * Function for stripping commands from any whitespace.
     * @param s is a reference to the string to be pruned from ws.
     */
    void removeWhiteSpace(string& s);

  public:
    /**
     * Explicit request for a default constructor.
     */
    Command()=default;
    /**
     * Explicit request for a default destructor
     */
    virtual ~Command()=default;
    /**
     * Parse and validate command strings given by the caller.
     * @param commandBuffer is a string containing the command to be parsed.
     * @param currentLine is a reference to the current line in the buffer.
     * It is used as a default address if the address is not specified.
     * @param totalLines indicates the size of the buffer (in lines).
     */
    bool parse(string& commandBuffer, const size_t& currentLine, 
            const size_t& totalLines);
    /**
     * Returns an address range extracted from the last parsed command.
     */
    const AddressRange& getAddressRange() const;
    /**
     * Returns the type of command, extracted from the last parsed command.
     */
    const CommandType& getCommandType() const;
    /**
     * Returns validation information resulting from the last parsed command.
     */
    const CommandStatus& getCommandStatus() const;
};

#endif 
