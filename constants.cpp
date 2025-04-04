#include "constants.hpp"

/*
stores the references to all commands we make for our program.
this way we can use Constants::<WHATEVER_STRING_YOU_NEED>
to get the exact string or constant variable we need every time 
without worrying about typos or bugs
*/
namespace Constants {
    const std::string TERMINAL_PROMPT = "> ";
    const std::string WELCOME_MESSAGE = "Welcome to the System Process Manager!! CLI\nType 'help' for available commands.\n";
    const std::string EXIT_COMMAND = "exit";
    const std::string HELP_COMMAND = "help";
    const std::string ERROR_MESSAGE = "An error occurred.";
    const int SUCCESS = 0;
    const int ERROR = 1;
    const int NO_COMMAND = -1;
} 

