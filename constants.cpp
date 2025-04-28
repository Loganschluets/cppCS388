#include "constants.hpp"

/*
stores the references to all commands we make for our program.
this way we can use Constants::<WHATEVER_STRING_YOU_NEED>
to get the exact string or constant variable we need every time 
without worrying about typos or bugs
*/
namespace Constants {
    const std::string TERMINAL_PROMPT = "> ";
    const std::string WELCOME_MESSAGE = "Welcome to the custom shell!";
    const std::string ERROR_MESSAGE = "An error occurred.";
    const int SUCCESS = 0;
    const int ERROR = 1;
    const int NO_COMMAND = -1;

    //commands
    const std::string EXIT_COMMAND = "exit";
    const std::string HELP_COMMAND = "help";
    const std::string PROCESSES = "processes";
    const std::string USER = "user";
    const std::string PROCESS_MEMORY = "pmem";
    const std::string TCP = "tcp";
    const std::string UDP = "udp";
    const std::string MEMORY = "mem";
    const std::string CPU = "cpu";
    

}