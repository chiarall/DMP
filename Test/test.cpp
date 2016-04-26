//
// Created by Chiara La Licata on 18/04/2016.
//

#include "../DataParser.h"
#include <thread>
#include <sstream>

void assertEquals(const char *expected, const char *actual);

void assertEquals(int expected, int actual);

using namespace std;

void convertIntToCharStar(int value, char *dest_char) {
    string dest;
    stringstream convert;
    convert << value;
    dest = convert.str();
    strcpy(dest_char, (char *) dest.c_str());
};


int main() {
    DataParser *parser = new DataParser("doeTable.dat");
    vector<vector<string>> doe = parser->read();

    cout << "Parser Test 1: " << endl;
    assertEquals(3, doe.size());
    assertEquals("0", doe[0][0].c_str());
    assertEquals("DMtest", doe[0][1].c_str());
    assertEquals("1", doe[1][0].c_str());

    // Test 1
    char charValue[100000];
    convertIntToCharStar(-102222220, charValue);
    const char *expected = "-102222220";

    cout << "Convert Test 2: " << endl;
    assertEquals(expected, charValue);


    // Test 2
    convertIntToCharStar(1923451, charValue);
    const char *expected2 = "1923451";

    cout << "Convert Test 3: " << endl;
    assertEquals(expected2, charValue);
}

void assertEquals(const char *expected, const char *actual) {
    if (strcmp(expected, actual) == 0) {
        cout << "Test OK!" << endl;
    } else {
        cout << "Parser Test Expected = " << expected << " found = " << actual << endl;
    }
}

void assertEquals(int expected, int actual) {
    if (expected == actual) {
        cout << "Test OK!" << endl;
    } else {
        cout << "Parser Test Expected = " << expected << " found = " << actual << endl;
    }
}