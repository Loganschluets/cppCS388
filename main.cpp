#include <iostream>
#include <string>
#include "constants.hpp"

using namespace std; 

void parseCommand(string command);
void getProcesses();



int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command = "";

    // terminal program driven by while loop and leaves on exit
    while (command != Constants::EXIT_COMMAND) {
        cout << Constants::TERMINAL_PROMPT;
        cout << "Type a number: ";
        cin >> command;
        parseCommand(command);
    }
    return 0;
}

void getProcesses() {
    FILE* pipe = popen("ps -e", "r");
    if (!pipe) {
        std::cerr << "Error opening pipe" << std::endl;
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;
    }

    pclose(pipe);
}

void parseCommand(string command) {
    if (command == "processes") {
        getProcesses();
    }
}

