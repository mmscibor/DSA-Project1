/* Michael Scibor
 * This project creates Stack and Queue objects which can be used
 * to push() and pop() objects. Both of these classes were created
 * from scratch without utilizing C++ built in Collections. */

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <stdlib.h>

#define CREATE "create"
#define PUSH "push"
#define POP "pop"

// The abstract class SimpleList: this class contains a Node object, and 
// contains methods for adding or removing elements from the front or end
// of the list ONLY. Because SimpleList is to be a superclass for Stacks
// and Queues, additional implementation to remove elements from the middle
// of the list is not necessary.

std::string arguments[4]; // Use this when parseing commands from .txt file
std::ofstream outputFile; // Use this stream to write to file throughout the code

template <class T>
class SimpleList {
    private:
        class Node {
            // This inner class is used to create Nodes: the methods defined here
            // are used by SimpleList to add or remove linked elements within the
            // SimpleList.
            private:
                T element;
                Node * nextNode;
                Node * previousNode;

            public:
                // Setter methods
                void setNextNode (Node * nextNode) {
                    this->nextNode = nextNode;
                }
                void setPreviousNode (Node * previousNode) {
                    this->previousNode = previousNode;
                }
                void setValue (T element) {
                    this->element = element;
                }

                // Getter methods
                Node * getNextNode () {return nextNode;}
                Node * getPreviousNode () {return previousNode;}
                T getValue () {return element;}
        };

        //std::vector<Node> listNodes;
        std::string listName;
        Node * headNode; // Points to first Node in the SimpleList
        Node * tailNode; // Final Node in the SimpleList, regularly updated


    public:
        // Constructor: each SimpleList should have a unique name.
        SimpleList (std::string listName) : listName(listName) {
            tailNode = new Node();
            headNode = new Node();
            (* headNode).setNextNode(tailNode);
            (* tailNode).setPreviousNode(headNode);
        }

        virtual void pop() = 0;

        void push(T element) {
            Node * updatedTail = new Node();
            (* tailNode).setValue(element);
            (* tailNode).setNextNode(updatedTail);
            (* updatedTail).setPreviousNode(tailNode);
            tailNode = updatedTail;
        }

        std::string getName() {
            return listName;
        }

    protected:
        // Insert a new Node at the end of SimpleList.

        void removeLast() {
            Node * toRemove = (* tailNode).getPreviousNode();
            Node * newLast = (* toRemove).getPreviousNode();
            (* tailNode).setPreviousNode(newLast);
            (* newLast).setNextNode(tailNode);
            T removedValue = (* toRemove).getValue();
            delete toRemove;
            outputFile << "Value popped: " << removedValue << std::endl;
        }

        void removeFirst() {
            Node * toRemove = (* headNode).getNextNode();
            Node * newFirst = (* toRemove).getNextNode();
            (* headNode).setNextNode(newFirst);
            (* newFirst).setPreviousNode(headNode);
            T removedValue = (* toRemove).getValue();
            delete toRemove;
            outputFile << "Value popped: " << removedValue << std::endl;
        }

        bool isEmpty() {
            if ((* headNode).getNextNode() == tailNode) {
                return true;
            }
            return false;
        }
};

template <class T>
class Queue : public SimpleList<T> {
    private:
        T returnValue;

    public:
        // Queue constructor, instantiate with name for queue.
        Queue (std::string queueName) : SimpleList<T>(queueName) {}

        T returnStoredValue() {
            return returnStoredValue;
        }

        void pop() {
            if (this->isEmpty()) {
                outputFile << "ERROR: This list is empty!\n";
            } else {
                this->removeFirst();
            }
        }
};

template <class T>
class Stack : public SimpleList<T> {
    private:
        T returnValue;

    public:
        // Queue constructor, instantiate with name for queue.
        Stack (std::string queueName) : SimpleList<T>(queueName) {}

        T returnStoredValue() {
            return returnStoredValue;
        }

        void pop() {
            if (this->isEmpty()) {
                outputFile << "ERROR: This list is empty!\n";
            } else {
                this->removeLast();
            }
        }
};

// Global declarations of the lists, these keep track of our Stacks & Queues
std::list<SimpleList<int> *> intList;
std::list<SimpleList<double> *> doubleList;
std::list<SimpleList<std::string> *> stringList;

// Function prototypes
void parseString(std::string parseable);
void execute();

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

    // Open file streams for reading / writing from / to file
    std::ifstream inputFile (inputDirectory.c_str());
    outputFile.open(outputDirectory.c_str());

    // Iterate through the lines of the file
    if (inputFile.is_open()) {
        while (getline(inputFile, fileLine)) {
            std::cout << "File line: " << fileLine << std::endl;
            parseString(fileLine);
            if (arguments[0].compare(POP) == 0) {
                outputFile << "PROCESSING COMMAND: " << arguments[0] << " " << arguments[1] << std::endl;
            } else {
                outputFile << "PROCESSING COMMAND: " << arguments[0] << " " << arguments[1] << " " << arguments[2] << std::endl;
            }
            execute();
        }
        inputFile.close(); // Close filestream after done reading / writing
        outputFile.close();
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

    int spaceHolder = 0;

    if (parseable.substr(0, 3).compare(POP) == 0) {
        arguments[0] = POP;
        spaceHolder = 4;
    } else if (parseable.substr(0, 4).compare(PUSH) == 0) {
        arguments[0] = PUSH;
        spaceHolder = 5;
    } else if (parseable.substr(0, 6).compare(CREATE) == 0) {
        arguments[0] = CREATE;
        spaceHolder = 7;
    }

    int secSpaceHolder = spaceHolder;
    if (!(arguments[0].compare(POP) == 0)) {
        while (parseable.at(secSpaceHolder) != ' ') {
            secSpaceHolder++;
        }
        arguments[1] = parseable.substr(spaceHolder, secSpaceHolder - spaceHolder);
        arguments[2] = parseable.substr(secSpaceHolder + 1);
    } else {
        arguments[1] = parseable.substr(spaceHolder);
    }

    std::cout << "Argument 0: " << arguments[0] << std::endl;
    std::cout << "Argument 1: " << arguments[1] << std::endl;
    std::cout << "Argument 2: " << arguments[2] << std::endl;

    arguments[3] = parseable.at(spaceHolder);
}

void execute() {
    bool listContains = false;

    if (arguments[3].compare("s") == 0) {
        std::list<SimpleList<std::string> *>::iterator iterator; 
        for (iterator = stringList.begin(); iterator != stringList.end(); iterator++) {
            if ((* iterator)->getName().compare(arguments[1]) == 0) {
                listContains = true;
                break;
            }
        }

        if (arguments[0].compare(CREATE) == 0) {
            if (!listContains) {
                SimpleList<std::string> * newSimpleList;
                if (arguments[2].compare("queue") == 0) {
                    newSimpleList = new Queue <std::string> (arguments[1]);
                } else {
                    newSimpleList = new Stack <std::string> (arguments[1]);
                }
                stringList.push_front(newSimpleList);
            } else {
                outputFile << "ERROR: This name already exists!" << std::endl;
            }
        } else if (arguments[0].compare(POP) == 0) {
            if (listContains) {
                (* iterator)->pop();
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        } else if (arguments[0].compare(PUSH) == 0) {
            if (listContains) {
                (* iterator)->push(arguments[2]);
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        }
    }
    
    if (arguments[3].compare("d") == 0) {
        std::list<SimpleList<double> *>::iterator iterator; 
        for (iterator = doubleList.begin(); iterator != doubleList.end(); iterator++) {
            if ((* iterator)->getName().compare(arguments[1]) == 0) {
                listContains = true;
                break;
            }
        }

        if (arguments[0].compare(CREATE) == 0) {
            if (!listContains) {
                SimpleList<double> * newSimpleList;
                if (arguments[2].compare("queue") == 0) {
                    newSimpleList = new Queue <double> (arguments[1]);
                } else {
                    newSimpleList = new Stack <double> (arguments[1]);
                }
                doubleList.push_front(newSimpleList);
            } else {
                outputFile << "ERROR: This name already exists!" << std::endl;
            }
        } else if (arguments[0].compare(POP) == 0) {
            if (listContains) {
                (* iterator)->pop();
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        } else if (arguments[0].compare(PUSH) == 0) {
            if (listContains) {
                (* iterator)->push(atof(arguments[2].c_str()));
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        }
    }
    
    if (arguments[3].compare("i") == 0) {
        std::list<SimpleList<int> *>::iterator iterator; 
        for (iterator = intList.begin(); iterator != intList.end(); iterator++) {
            if ((* iterator)->getName().compare(arguments[1]) == 0) {
                listContains = true;
                break;
            }
        }

        if (arguments[0].compare(CREATE) == 0) {
            if (!listContains) {
                SimpleList<int> * newSimpleList;
                if (arguments[2].compare("queue") == 0) {
                    newSimpleList = new Queue <int> (arguments[1]);
                } else {
                    newSimpleList = new Stack <int> (arguments[1]);
                }
                intList.push_front(newSimpleList);
            } else {
                outputFile << "ERROR: This name already exists!" << std::endl;
            }
        } else if (arguments[0].compare(POP) == 0) {
            if (listContains) {
                (* iterator)->pop();
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        } else if (arguments[0].compare(PUSH) == 0) {
            if (listContains) {
                (* iterator)->push(atoi(arguments[2].c_str()));
            } else {
                outputFile << "ERROR: This name does not exist!" << std::endl;
            }
        }
    }
}

template <class T>
bool listContains(std::list<SimpleList<T> *> searchable, std::string name) {
    for (typename std::list<SimpleList<T> *>::iterator iterator = searchable.begin(); iterator != searchable.end(); iterator++) {
        if ((* iterator)->getName().compare(name) == 0) {
            return true;
        }
    }
    return false;
}
