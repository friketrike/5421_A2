/*
 * Assignennt 2, COMP 5421, summer 2016  
 * Federico O'Reilly Regueiro 40012304
 * Concordia University
 * 
 * Line Editor implementation file
 */

#ifndef LINE_EDITOR
#define LINE_EDITOR
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "Command.h"

using namespace std;

/**
 * The LineEditor class.
 * Holds and manipulates a buffer of lines which can be read and written to a 
 * text file.
 */
class LineEditor{

  private:
    list<string> buffer; /**<A buffer contains lines of text. */
    Command com; /**<A Command object to help parse commands. See \ref Command*/
    size_t currentLine{0}; /**<Marker indicating the current line number. 
                             Empty buffers are initialized at line 0. Indexing 
                             of non-empty buffers starts at 1*/
    string filename; /**<The name of the file from which we read and to which 
                       we write. If left empty on initialization, the user will 
                       be prompted to choose a filename before saving the file*/
    bool fileExists; /**<Keep track if we're creating a file or editing an 
                       existing one.*/
    bool dirty{false}; /**<True when changes have not been written.*/
    bool quitLed{false}; /**<True when the user has chosen to quit.*/
    
    // led-command methods
    /**
     * Insert a line prior to the chosen line.
     * @param line is the line number before which another line will be added.
     */
    void insert(const size_t& line);
    /**
     * Appends a line after the speified line.
     * @param line is the line number after which another line will be added.
     * If line number is unspecified, insertion happens after the current line.
     */
    void append(const size_t& line);
    /**
     * Removes a range of lines from the buffer.
     * The range of lins to be removed must be a valid range.
     * If the line 
     */
    void remove(const Command::AddressRange& ar);
    /**
     * Prints the specified range of lines.
     * @param numbered, when true, lines are printed with a prepended
     * line number. numbered defaults to false.
     */ 
    void print(const Command::AddressRange& ar,
            const bool& numbered = false);
    /**
     * Prints the line number for the last line which was manipulated.
     */ 
    void printCurrLine();
    /**
     * Replace matching portions of lines within the given range.
     * All substrings matching the string given by the user will be replaced
     * by another string, also input by the user.
     * @param ar defines the address range where replacements will take place.
     */
    void change(const Command::AddressRange& ar);
    /**
     * Move the current position up or down by however many lines are indicated.
     * @param lines defines how many lines to move up or down.
     * @param isUp is a boolean signaling upwards movement, 
     * it defaults to false (downwards movement);
     */
    void move(const size_t& lines, const bool& isUp = false);
    /**
     * Writes the contents of the buffer to a file.
     * If a filename has been specified, write() attempts to write
     * the contents of the buffer into said file. If no filename has
     * been specified, it prompts the user for a filename.
     */ 
    void write();
    /**
     * Exits the line editor.
     * If the buffer contains unsaved changes, it prompts the user 
     * to save the buffer's contents onto a file.
     */ 
    void quit();
    /**
     * Opens a new file.
     * If the current buffer contains unsaved changes, it prompts 
     * the user to save the buffer's contents
     */
    void open();
    /**
     * Displays usage information for the user.
     * Defeats the purpose of a line editor in that it's rather verbose
     * but if help is needed, what can you do?
     */
    void help(); 

    // helper methods
    /**
     * Helper function for opening files.
     * Called by the constructor as well as the open function.
     */
    void openHelper();
    /**
     * Unified method for insert and append.
     * @param line is the line after/before which contents will be added.
     * @param isInsert indicates if insertion (T) or appending take place. 
     */
    void insertAppendHelper (const size_t& line, const bool& isInsert);
    /**
     * Helper function prompting the user to write unsaved changes.
     * If the user chooses to do so, saveChanges also writes the buffer's 
     * contents to a file.
     */
    void saveChanges();
    /**
     * Inform the user that command mode is starting.
     */
    void displayEnteringCommand();
    /**
     * Display a prompt informing the user if the buffer is dirty or not.
     * Feedback in order to quickly know if the buffer contains unsaved changes.
     */
    void displayPrompt();
    /**
     * Helper function that chooses the appropriate command to perform.
     * @param ct is an \ref Command#CommandType enum returned by the Command
     * object after parsing the user input.
     * @param ar is an \ref Command#AddressRange with a start and end line
     * address returned by the Command member after parsing a user command.
     */
    void routeCommand (const Command::CommandType& ct, 
        const Command::AddressRange& ar);

  public:
    /**
     * Default constructor taking 1 optional parameter.
     * @param filename gives the name of a file to be read into the buffer
     * and onto which the buffer's contents can be saved. It defaults to
     * an empty string, creating a new buffer with no association to a file.
     */
    LineEditor(const string& filename = "");
    /**
     * Explicit default destructor
     */
    virtual ~LineEditor()=default;
    /**
     * Causes the initialized LineEditor to run (interactively).
     */
    void run();
};

#endif
