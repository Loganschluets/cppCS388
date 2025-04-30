#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

//constants used in constants.cpp need to be declared here in our .hpp or header file that 
//we can refernce at the top of other programs, ex at the top of main
namespace Constants {
    extern const std::string TERMINAL_PROMPT;
    extern const std::string WELCOME_MESSAGE;
    extern const std::string ERROR_MESSAGE;
    extern const int NO_COMMAND;
    extern const int SUCCESS;
    extern const int ERROR;
    
    //commands
    extern const std::string EXIT_COMMAND;
    extern const std::string HELP_COMMAND;
    extern const std::string PROCESSES;
    extern const std::string USER;
    extern const std::string PROCESS_MEMORY;
    extern const std::string TCP;
    extern const std::string UDP;
    extern const std::string MEMORY;
    extern const std::string CPU;
    extern const std::string TCP_CONNECTIONS;
    extern const std::string UDP_CONNECTIONS;
    extern const std::string PROCESS_DETAILS;
    extern const std::string THREAD_LIST;
}

#endif
