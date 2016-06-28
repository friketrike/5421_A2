
#include <iostream>
#include "Command.h"
#include "LineEditor.h"

int main (){
    LineEditor led("somefile.txt");
    led.run();
}
