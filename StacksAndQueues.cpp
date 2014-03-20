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

        std::string getName() {
            return listName;
        }

    protected:
        // Insert a new Node at the end of SimpleList.
        void insert(T element) {
            Node * updatedTail = new Node();
            (* tailNode).setValue(element);
            (* tailNode).setNextNode(updatedTail);
            (* updatedTail).setPreviousNode(tailNode);
            tailNode = updatedTail;
        }

        T removeLast() {
            Node * toRemove = (* tailNode).getPreviousNode();
            Node * newLast = (* toRemove).getPreviousNode();
            (* tailNode).setPreviousNode(newLast);
            (* newLast).setNextNode(tailNode);
            T removedValue = (* toRemove).getValue();
            delete toRemove;
            return removedValue;
        }

        T removeFirst() {
            Node * toRemove = (* headNode).getNextNode();
            Node * newFirst = (* toRemove).getNextNode();
            (* headNode).setNextNode(newFirst);
            (* newFirst).setPreviousNode(headNode);
            T removedValue = (* toRemove).getValue();
            delete toRemove;
            return removedValue;
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

        void push(T element) {
            insert(element);
        }

        void pop() {
            if (this->isEmpty()) {
                outputFile << "ERROR: This list is empty!\n";
            } else {
                returnValue = this->removeFirst();
                returnStoredValue();
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

        void push(T element) {
            insert(element);
        }

        void pop() {
            if (this->isEmpty()) {
                outputFile << "ERROR: This list is empty!\n";
            } else {
                returnValue = this->removeLast();
                returnStoredValue();
            }
        }
};

// Global declarations of the lists, these keep track of our Stacks & Queues
std::list<SimpleList<int> *> intList;
std::list<SimpleList<double> *> doubleList;
std::list<SimpleList<std::string> *> stringList;

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

    // Open file streams for reading / writing from / to file
    std::ifstream inputFile (inputDirectory.c_str());
    outputFile.open(outputDirectory.c_str());

    // Iterate through the lines of the file
    if (inputFile.is_open()) {
        while (getline(inputFile, fileLine)) {
            parseString(fileLine); 
        }
        inputFile.close(); // Close filestream after done reading / writing
        outputFile.close();
    }

    return 0;
}


void parseString(std::string fileLine) {
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
