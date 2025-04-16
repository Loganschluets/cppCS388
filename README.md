# cppCS388
**System Process Manager**
Designed in C++, the command-line based system tool System Process Manager lets users track system performance, control running processes, and show resource consumption. It mainly Focused on Linux environments, the project supports necessary maintenance commands including CPU/RAM status checking, killing processes, and network activity viewing.

**Authors:**
Logan Schhueter
Nithin Reddy Jillella

**Project Commentary**
This effort seeks to automate and streamline Linux system monitoring chores. 
It is Time-consuming and error-prone manual inspection of processes, CPU, and memory can be This CLI tool was created to provide real-time feedback via basic terminal commands.
The tool shows process, memory, and network information in a readable format and links several Linux utilities. 
It also lets users interact with the system by commanding actions like clearing the terminal or killing procedures.

**Principal Characteristics**
Show a list of every CPU and memory-using active process.

Show system-wide CPU and RAM usage now.

See network interface data and active socket connections.

Kill processes by supplying their PID.

Simple, minimal command-line interface.

Designed in-built assistance to direct users.

Loop of constant commands for user interaction.

Supported commands support Display a list of every commanded that you support.

Show all running processes together with their CPU and memory consumption.

Display current CPU use here.

**ram**: show present RAM consumption.

Show simple network interface details.

Show all currently active **TCP/UPD** connections.

**kill** — By PID, end a particular process.

**clear**— wipe the terminal screen.


**Programming language:** C++; Technology Stack

**Environment of Development**: Linux tested on Poseidon and Ubuntu servers

Create tools with g++ and bash.

Ps, top, free, ifconfig, netstat, kill: external commands used

IDE: Remote SSH extension visual studio code

Create using g++ main.cpp constants.cpp -osysman

Run the program under./sysman.
run using commands: 
run once:
chmod +x run.sh

run every time after:
./run.sh

