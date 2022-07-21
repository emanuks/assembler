#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool findSymbolOnTable(string, string [], int);

void printSymbolTable(string [], int [], int);

string instructionsTable[] = {"ADD", "SUB", "MUL", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};

string directivesTable[] = {"SPACE", "CONST", "BEGIN", "END", "EXTERN", "PUBLIC", "EQU", "IF"};

int main(int argc, char **argv) {
    ifstream file;
    string line, label, operation, operators, comments;
    string symbolsTable[100];
    int symbolsPositionTable[100];
    size_t labelAux, operationAux;
    int lineCounter = 0, positionCounter = 0, symbolsTableIndex = 0;

    file.open(argv[2]);

    if (file.is_open()) {
        while (file) {
            getline(file, line);
            lineCounter++;

            if (line.empty()) 
                continue;

            labelAux = line.find_first_of(':');
            if (labelAux != string::npos) {
                label = line.substr(0, labelAux);
                if (findSymbolOnTable(label, symbolsTable, 100)) {
                    cout << "ERROR: Symbol already defined -> " << label << endl;
                    break;
                } else {
                    symbolsTable[symbolsTableIndex] = label;
                    symbolsPositionTable[symbolsTableIndex] = positionCounter;
                    symbolsTableIndex++;

                    line = line.substr(labelAux+2);
                    operationAux = line.find_first_of(' ');
                    if (line == "SPACE") {
                        positionCounter++;
                        continue;
                    }

                    if (operationAux != string::npos) {
                        operation = line.substr(0, operationAux);

                        if (operation == "SECAO") 
                            continue;

                        if (findSymbolOnTable(operation, instructionsTable, 14)) {
                            positionCounter++;
                            if (operation != "STOP") {
                                positionCounter++;
                            }
                        } else if (findSymbolOnTable(operation, directivesTable, 8)) {
                            if (operation == "CONST") {
                                positionCounter++;
                            }
                        } else {
                            cout << "ERROR: Unknown operation -> " << operation << endl;
                            break;
                        }
                    }
                }
            } else {
                operationAux = line.find_first_of(' ');

                if (line == "STOP") {
                    positionCounter++;
                    continue;
                }

                if (operationAux != string::npos) {
                    operation = line.substr(0, operationAux);

                    if (operation == "SECAO") 
                        continue;

                    if (findSymbolOnTable(operation, instructionsTable, 14)) {
                        positionCounter += 2;
                    } else if (findSymbolOnTable(operation, directivesTable, 8)) {
                    } else {
                        cout << "ERROR: Unknown operation -> " << operation << endl;
                        break;
                    }
                }
            }
        }
    }

    printSymbolTable(symbolsTable, symbolsPositionTable, symbolsTableIndex);

    return 0;
}

bool findSymbolOnTable(string symbol, string table[], int quantityPositions) {
    for(int i = 0; i < quantityPositions; i++) {
        if (table[i] == symbol)
            return true;
    }
    return false;
}

void printSymbolTable(string symbolsTable[], int symbolsPositionTable[], int index) {
    for(int i = 0; i < index; i++)
        cout << symbolsTable[i] << " " << symbolsPositionTable[i] << endl;
}