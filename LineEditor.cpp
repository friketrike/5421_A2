
#include "LineEditor.h"

LineEditor::LineEditor (const string& filename) {
    this->filename = filename;
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

void LineEditor::displayEnteringCommand() {
    cout << "Entering command mode." << endl;
}
void LineEditor::displayPrompt() {
    cout << ((dirty) ? "*:" : ":");
}

void LineEditor::run () {
    string fileStatus;
    string prompt;
    displayEnteringCommand();
    while (!quitLed) {
        displayPrompt();
        string commandBuffer;
        getline(cin, commandBuffer);
        bool valid = com.parse(commandBuffer, currentLine, buffer.size());
        const Command::AddressRange ar = com.getAddressRange();
        const Command::CommandType ct = com.getCommandType();
        if (valid) {
            routeCommand(ct, ar);
        } else {
            // TODO change other error messages to cerr...
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
                        cerr << e1 << e2 << e3;
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
    string inputLine;
    auto it = buffer.begin();
    getline (cin , inputLine);
    if (!buffer.empty()) {
        advance(it, line-1);
    }
    while (cin.good () && inputLine != ".")
    {
        buffer.insert(it, inputLine);
        currentLine++;
        getline (cin , inputLine);
    }
    dirty = true;
    displayEnteringCommand();
}

void LineEditor::append (const size_t& line) {
    string inputLine;
    auto it = buffer.begin();
    getline (cin , inputLine);
    if (!buffer.empty()) {
        advance(it, line);
    }
    while (cin.good () && inputLine != ".")
    {
        buffer.insert(it, inputLine);
        currentLine++;
        getline (cin , inputLine);
    }
    dirty = true;
    displayEnteringCommand();
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
 
void LineEditor::move (const size_t& line, const bool& up /* = false*/) {
    if (buffer.size() == 0) {
        currentLine = 0;
    }
    else if (up) {
        if (line < currentLine) {
            currentLine -= line;
        } else {
            currentLine = 1;
            cerr << "BOF reached" << endl;
        }
    } else {
        if (line <= (buffer.size() - currentLine)) {
            currentLine += line;
        } else {
            currentLine = buffer.size();
            cerr << "EOF reached" << endl;
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
        string name = (filename.empty()) ? "[New File]" : filename;
        cout << "Save changes to " << name << " (y/n)?" << endl;
        char yn;
        cin >> yn;
        while (yn != 'y' && yn != 'n' && yn != 'Y' && yn != 'N') {
            cin >> yn;
        }
        if (yn == 'Y' || yn == 'y') {
            write();
        }
    }
    quitLed = true;
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
            case Command::notRecognized:
                cerr << "error:  command not recognized by the parser" << endl;
                break;
            default:
                cerr << "error:  being here defies all logic..." << endl;
        }
}
