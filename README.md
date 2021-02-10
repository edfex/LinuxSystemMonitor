# Linux-System-Monitor
This project displays linux filesystem information using the ncurses graphical library similar to the program htop.
Information includes system details, processor / memory utilization, and detailed information on all individual processes running on the system.
Processes can be sorted by PID, User, CPU Utilization, Memory Utilization, Time Active, and Command. Information updates every second.

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions
1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`
2. Build the project: `make build`
3. Run the resulting executable: `./build/monitor`