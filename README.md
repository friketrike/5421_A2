
LED is a line editor.
It is made up of two classes: LineEditor and Command.
LineEditor holds a buffer which is implemented via a list container holding a
string per line. It contains a Command object which parses user-input commands,
verifying correctness and parsing into an address range and a directive.
Command hosts an AddressRange struct in to return start-end pairs in a less 
verbose fashion. Command also defines an enum for commands as well as an enum
with a few valid/invalid states of parsed commands such as invalid range or
invalid syntax.
Commands are insert, append, remove, print, print with line numbers, change,
up, down, write, print current line number, quit, open and help.
Open and help have been added from the Assignment requirements; open allows the
user to open a new file, prompting to save changes if the buffer contains 
unsaved modifications. Help displays a brief list of commands.

