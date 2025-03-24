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

//this function will be used to parse the different commands we add
//these conditionals can lead to other functions depending on how much logic the command includes
//any example of this is the conditional for processes, if met this executes the function to show all processes
void parseCommand(string command) {
    if (command == "processes") {
        getProcesses();
    }
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

