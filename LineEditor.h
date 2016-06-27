#ifndef LINE_EDITOR

#include <line>
#include <string>
#include <iostream>
#include <functional> // try and then delete
#include "Command.h"

class LineEditor{
  using namespace std;
  friend class Command;
  private:
    line<string> buffer;
    Command com;
    function<void(Command::AddressRange& ar)> nextCommand; // try and then delete TODO then change signatures below
    void insert(Command::AddressRange& ar);
    void append(Command::AddressRange& ar);
    void remove(Command::AddressRange& ar);
    void print(Command::AddressRange& ar);
    void numberPrint(Command::AddressRange& ar);
    void change(Command::AddressRange& ar);
    void up(Command::AddressRange& ar);
    void down(Command::AddressRange& ar);
    void write(Command::AddressRange& ar);

  public:
    LineEditor(const string& filename);
    LineEdtitor()=delete;
    virtual ~LineEditor()=default;
    void run();
}

#endif
