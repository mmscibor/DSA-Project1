/* Michael Scibor
 * This project creates Stack and Queue objects which can be used
 * to push() and pop() objects. Both of these classes were created
 * from scratch without utilizing C++ built in Collections. */

#include <iostream>
#include <fstream>
#include <list>
#include <string>

#define CREATE "create"
#define PUSH "push"
#define POP "pop"

// The abstract class SimpleList: this class contains 
template <class T>
class SimpleList {
    private:
        std::string listName;
        class Node {
            T element;
            Node * nextNode;
        } node;

    public:
        // Constructor: each SimpleList should have a unique name.
        SimpleList (std::string listName) : listName(listName) {}

        // Pure virtual functions for Stack & Queue to define.
        virtual void insertLast() = 0;
        virtual void removeLast() = 0;
        virtual void removeFirst() = 0;

        std::string getName() {
            return listName;
        }
};

// Global declarations of the lists, these keep track of our Stacks & Queues
std::list<SimpleList<int> *> intList;
std::list<SimpleList<double> *> doubleList;
std::list<SimpleList<std::string> *> stringList;
std::string arguments[4]; // Use this when parseing commands from .txt file

// Function prototypes
void parseString(std::string parseable);
void execute(std::string command);

template <class T>
bool listContains(std::list<SimpleList<T> *> searchable, std::string name);

int main() {
    // Initial variable declarations, as well as pointer lists
    std::string inputDirectory;
    std::string outputDirectory;
    std::string fileLine;

    // Receive user input, determine input file and name of output file
    std::cout << "Enter the name of the input file: ";
    std::cin >> inputDirectory;
    std::cout << "Enter the name of the output file: ";
    std::cin >> outputDirectory;

    // Open file stream for reading from file
    std::ifstream inputFile (inputDirectory.c_str());

    // Iterate through the lines of the file
    if (inputFile.is_open()) {
        while (getline(inputFile, fileLine)) {
            parseString(fileLine);
            execute(arguments[0]); // arguments[0] is the COMMAND to be executed. (see parseString())
            
        }
        inputFile.close(); // Close filestream after done reading
    }

    return 0;
}

void parseString(std::string parseable) {
    /* This method parses one line of input from the .txt file at a time.
     * arguments[0] is the COMMAND to be executed.
     * arguments[1] is the VARIABLE NAME to be created or utilized.
     * arguments[2] is either list type, value to be pushed, or empty string if pop. 
     * arguments[3] is the first letter of the VARIABLE NAME: i, d, or s.
     * Notice arguments[] is a Global Variable that is only updated here. */

    for (int i = 0, spaceHolder = 0; i < parseable.length(); i++) {
        if (isspace(parseable.at(i)) && spaceHolder == 0) {
            arguments[0] = parseable.substr(0, i);
            spaceHolder = i;
        }

        if (isspace(parseable.at(i)) && spaceHolder != 0) {
            if (arguments[0].compare(POP) == 0) {
                arguments[1] = parseable.substr(spaceHolder);
                arguments[2] = "";
            } else {
                arguments[1] = parseable.substr(spaceHolder, (i-spaceHolder));
                arguments[2] = parseable.substr(i);
            }
        }

        arguments[3] = arguments[1].at(0);
    }
}

void execute(std::string command) {
    if (command.compare(CREATE) == 0) {
        // usedName returns true is that variable name is already in use, false otherwise.
        bool usedName = listContains(intList, arguments[1]) || listContains(doubleList, arguments[1]) || listContains(stringList, arguments[1]); 
        if (usedName) {
            std::cout << "ERROR: This name already exists!" << std::endl; // TODO: Make this right to file instead
        } else {
        }
    }
}

template <class T>
bool listContains(std::list<SimpleList<T> *> searchable, std::string name) {
    // Searches one of the three lists of Stacks & Queues to see if the variable name is used yet.
    for (int i = 0; i < searchable.size(); i++) {
        if (name.compare(searchable.getName()) == 0) {
            return true;
        }
    }
    return false;
}   
