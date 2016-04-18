//
// Created by Chiara La Licata on 18/04/2016.
//

#include "../DataParser.h"


int main() {
    DataParser *parser = new DataParser("doeTable.dat");
    parser->read();
}