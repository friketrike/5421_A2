/*
 * Assignennt 2, COMP 5421, summer 2016  
 * Federico O'Reilly Regueiro 40012304
 * Concordia University
 * 
 * LED driver file
 */

#include "LineEditor.h"

int main(int argc, char * argv[])
{
    string filename;
    switch (argc) {
        case 1: // no file name
            break;
        case 2: // read from argument string
            filename = argv[1];
            break;
        default:
            cout << ("too many arguments - all discarded") << endl;
            filename.clear();
            break;
    }
    LineEditor led(filename); // create an editor object
    led.run(); // run our editor
    return 0;
}
