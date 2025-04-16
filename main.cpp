#include <iostream>
#include <string>
#include <sstream>
#include "constants.hpp"

using namespace std;

void parseCommand(const string& command);
void getProcesses();
void showCPU();
void showRAM();
void showNetwork();
void showConnections();
void killProcess(const string& pid);
void showHelp();

int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command;

    while (true) {
        cout << Constants::TERMINAL_PROMPT;
        getline(cin, command);
        if (command == Constants::EXIT_COMMAND) break;
        parseCommand(command);
    }

    return 0;
}

void parseCommand(const string& command) {
    if (command == "processes") {
        getProcesses();
    } else if (command == "cpu") {
        showCPU();
    } else if (command == "ram") {
        showRAM();
    } else if (command == "network") {
        showNetwork();
    } else if (command == "connections") {
        showConnections();
    } else if (command.rfind("kill", 0) == 0) {
        istringstream iss(command);
        string cmd, pid;
        iss >> cmd >> pid;
        if (!pid.empty()) killProcess(pid);
        else cout << "Please provide a PID to kill.\n";
    } else if (command == "clear") {
        system("clear");
    } else if (command == Constants::HELP_COMMAND) {
        showHelp();
    } else {
        cout << "Invalid command. Type 'help' for a list of commands.\n";
    }
}

void getProcesses() {
    system("ps -eo pid,ppid,cmd,%mem,%cpu --sort=-%cpu | head");
}

void showCPU() {
    system("top -bn1 | grep \"%Cpu\"");
}

void showRAM() {
    system("free -h");
}

void showNetwork() {
    system("ifconfig");
}

void showConnections() {
    system("netstat -tunapl");
}

void killProcess(const string& pid) {
    string cmd = "kill -9 " + pid;
    int result = system(cmd.c_str());
    if (result != 0) cout << "Failed to kill process " << pid << "\n";
}

void showHelp() {
    cout << "\nAvailable Commands:\n";
    cout << "  help          - Show this help message\n";
    cout << "  processes     - List all running processes\n";
    cout << "  cpu           - Show current CPU usage\n";
    cout << "  ram           - Show current RAM usage\n";
    cout << "  network       - Show network interface info\n";
    cout << "  connections   - Show active TCP/UDP connections\n";
    cout << "  kill <pid>    - Kill a process by its PID\n";
    cout << "  clear         - Clear the terminal screen\n";
    cout << "  exit          - Exit the program\n\n";
}
