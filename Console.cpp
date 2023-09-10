#include <iostream>
using namespace std;

class Console {        // The class
public:          // Access specifier
    string content;  // Attribute
    Console(string x) { // Constructor with parameters
        content = x;
        cout << x << endl;
    }
};





