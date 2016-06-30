/*
 * Assignennt 2, COMP 5421, summer 2016  
 * Federico O'Reilly Regueiro 40012304
 * Concordia University
 * 
 * Line Editor implementation file
 */

#include "LineEditor.h"

LineEditor::LineEditor (const string& filename) {
    this->filename = filename;
    openHelper();
}

void LineEditor::run () {
    displayEnteringCommand();
    while (cin.good() && !quitLed) {
        displayPrompt();
        string commandBuffer;
        getline(cin, commandBuffer);
        bool valid = com.parse(commandBuffer, currentLine, buffer.size());
        const Command::AddressRange ar = com.getAddressRange();
        const Command::CommandType ct = com.getCommandType();
        if (valid) {
            routeCommand(ct, ar);
        } else {
            if (com.getCommandStatus() == Command::invalidRange) {
                if (buffer.size() == 0) {
                    bool validInit = ((ct == Command::insert) 
                            || (ct == Command::append));
                    if (validInit) {
                        routeCommand(ct, Command::AddressRange(0,0));
                    } else {
                        string e1 = "error:  file empty ";
                        string e2 = "- enter 'q' to quit, 'a' to append, ";
                        string e3 = "or 'i' to insert";
                        cerr << e1 << e2 << e3 << endl;
                } 
                } else {
                    cerr << "error:  invalid range " << ar.start << " through "
                       << ar.end << endl; 
                }
            } else {
                cerr << "error:  invalid command." << endl;
            }
        }
    }
}

void LineEditor::insert (const size_t& line) {
    insertAppendHelper (line, true);
}

void LineEditor::append (const size_t& line) {
    insertAppendHelper (line, false);
}

void LineEditor::remove (const Command::AddressRange& ar) {
    auto it1 = buffer.begin();
    auto it2 = buffer.begin();
    advance(it1, ar.start-1);
    advance(it2, ar.end);
    buffer.erase(it1, it2);
    currentLine = ar.start - 1;
    dirty = true;
}
 
void LineEditor::print (const Command::AddressRange& ar
            , const bool& numbered /*= false*/) {
    auto it1 = buffer.begin();
    auto it2 = buffer.begin();
    advance(it1, ar.start-1);
    advance(it2, ar.end);
    currentLine = ar.start;
    while (it1 != it2) {
        string prepend = numbered ? "(" + to_string(currentLine) + ") " : "";
        cout << prepend << *it1 << endl;
        ++it1;
        ++currentLine;
    }
    currentLine = ar.end;
}
 
void LineEditor::printCurrLine () {
    cout << currentLine << endl;
}

void LineEditor::change (const Command::AddressRange& ar) {
    size_t found;
    string toBeReplaced;
    string replacement;
    cout << "change what? ";
    getline (cin, toBeReplaced);
    cout << "by what? ";
    getline (cin, replacement);
    // now make the required changes
    auto start = buffer.begin();
    advance(start, ar.start -1);
    auto end = buffer.begin();
    advance(end, ar.end);
    while(start != end) {
        found = start->find(toBeReplaced); 
        while (found != string::npos) {
            start->replace(found, toBeReplaced.length(), replacement);
            ++found;
            found = start->find(toBeReplaced, found);
        }
        ++start;
    }
    currentLine = ar.end;
    dirty = true;
}
 
void LineEditor::move (const size_t& lines, const bool& isUp /* = false*/) {
    if (buffer.size() == 0) {
        currentLine = 0;
    }
    else if (isUp) {
        if (lines < currentLine) {
            currentLine -= lines;
        } else {
            currentLine = 1;
            cerr << "BOF reached" << endl;
        }
    } else {
        if (lines <= (buffer.size() - currentLine)) {
            currentLine += lines;
        } else {
            currentLine = buffer.size();
            cerr << "EOF reached-debug" << endl; //TODO remove
        }
    }
}
 
void LineEditor::write () {
    if (filename.empty()) {
        cout << "Name of the file to be saved? ";
        // TODO some form of input validation for this
        cin >> filename;
        // TODO check if it exists and prompt to verwrite
    }
    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::trunc);
    if (ofs.is_open()) {
        auto it = buffer.begin();
        while (it != buffer.end()) {
            ofs << *it << endl;
            ++it;
        }
        ofs.close();
        cout << "\"" << filename << "\" " << to_string(buffer.size()) 
            << " lines" << endl;
        dirty = false;
    } else {
        cerr << "error:  couldn't open " << filename << " for writing" << endl;
    }
}

void LineEditor::quit () {
    if (dirty) {
        saveChanges();
    }
    quitLed = true;
}       

void LineEditor::open () {
    string newFilename;
    if (dirty) {
        saveChanges();
    }
    cout << "open file, filename? ";
    if (cin.good()) {
        cin >> newFilename;
    } // TODO some form of validation
    filename = newFilename;
    buffer.clear(); // clear will call destructors on each string, nice!
    currentLine = 0;
    openHelper();
    dirty = false;
}

void LineEditor::openHelper () {
    if (!filename.empty()) {
        ifstream file(filename);
        string fileLine;
        if (file.is_open()) {
            while (getline(file, fileLine)) {
                buffer.push_back(fileLine);
                ++currentLine;
            }
            file.close();
            cout << "\"" << filename << "\" " << to_string(buffer.size()) 
                << " lines" << endl;
        } else {
            cerr << "Unable to open file " << filename << endl;
            cout << "\"" << filename << "\" [New File]" << endl;
        }
    } else {
        cout << "[New File]" << endl;
    }
}

void LineEditor::insertAppendHelper (const size_t& line, const bool& isInsert) {
    string inputLine; 
    auto it = buffer.begin();
    getline (cin , inputLine);
    if (!buffer.empty()) {
        size_t linesToAdvance = (isInsert) ? line-1 : line;
        advance(it, linesToAdvance);
        currentLine = linesToAdvance;
    }
    while (cin.good () && inputLine != ".")
    {
        buffer.insert(it, inputLine);
        currentLine++;
        dirty = true;
        getline (cin , inputLine);
    }
    displayEnteringCommand();
}

void LineEditor::saveChanges () {
    string name = (filename.empty()) ? "[New File]" : filename;
    cout << "Save changes to " << name << " (y/n)?" << endl;
    char yn;
    cin >> yn;
    while (cin.good() && yn != 'y' && yn != 'n' && yn != 'Y' && yn != 'N') {
        cin >> yn;
    }
    if (yn == 'Y' || yn == 'y') {
        write();
    }
}

void LineEditor::displayEnteringCommand() {
    cout << "Entering command mode." << endl;
}
void LineEditor::displayPrompt() {
    cout << ((dirty) ? "*:" : " :");
}

void LineEditor::routeCommand (const Command::CommandType& ct, 
        const Command::AddressRange& ar) {
        switch (ct) {
            case Command::insert:
                insert(ar.start);
                break;
            case Command::append:
                append(ar.end);
                break;
            case Command::remove:
                remove(ar);
                break;
            case Command::print:
                print(ar);
                break;
            case Command::numberPrint:
                print(ar, true);
                break;
            case Command::change:
                change(ar);
                break;
            case Command::up:
                move(ar.start, true);
                break;
            case Command::down:
                move(ar.end);
                break;
            case Command::write:
                write();
                break;
            case Command::printCurrLine:
                printCurrLine();
                break;
            case Command::quit:
                quit();
                break;
            case Command::open:
                open();
                break;
            case Command::notRecognized:
                cerr << "error:  command not recognized by the parser" << endl;
                break;
            default:
                cerr << "error:  being here defies all logic..." << endl;
        }
}
