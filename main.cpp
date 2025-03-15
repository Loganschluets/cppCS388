#include <iostream>
#include <string>
#include "constants.hpp"

using namespace std; 

void parseCommand();



int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command = "";

    // terminal program driven by while loop and leaves on exit
    while (command != Constants::EXIT_COMMAND) {
        cout << Constants::TERMINAL_PROMPT;
        cout << "Type a number: ";
        cin >> command;
    }
    return 0;
}

void parseCommand(string command) {
}

