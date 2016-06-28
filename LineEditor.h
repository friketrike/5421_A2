
#ifndef LINE_EDITOR
#define LINE_EDITOR
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "Command.h"

using namespace std;

class LineEditor{

  private:
    list<string> buffer;
    Command com;
    size_t currentLine{0};
    string filename;
    bool fileExists;
    bool dirty{false};
    bool quitLed{false};
    
    // led-command methods
    void insert(const size_t& line);
    void append(const size_t& line);
    void remove(const Command::AddressRange& ar);
    void print(const Command::AddressRange& ar
            , const bool& numbered = false);
    void printCurrLine();
    void change(const Command::AddressRange& ar);
    void move(const size_t& line, const bool& up = false);
    void write();
    void quit();

    // helper methods
    void displayEnteringCommand();
    void displayPrompt();
    void routeCommand (const Command::CommandType& ct, 
        const Command::AddressRange& ar);

  public:
    LineEditor(const string& filename = "");
    LineEditor()=delete;
    virtual ~LineEditor()=default;
    void run();
};

#endif
