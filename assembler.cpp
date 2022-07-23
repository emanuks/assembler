#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool findSymbolOnTable(string, string [], int);

void printSymbolTable(string [], int [], int);

string getEquVal(string , string [], string []);

bool isNumber(string);

bool isHex(string);

string getOpcode(string, string[], string[]);

int getValue(string, string[], int []);

string instructionsTable[] = {"ADD", "SUB", "MUL", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};

string directivesTable[] = {"SPACE", "CONST", "BEGIN", "END", "EXTERN", "PUBLIC", "EQU", "IF"};

string opcodes[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14"};

int main(int argc, char **argv) {
    ifstream file;
    ofstream objectFile(argv[3]);
    string line, label, operation, operators, operator1, operator2, comments, object, originalLine;
    string symbolsTable[100];
    string equTable[100];
    int symbolsPositionTable[100];
    size_t labelAux, operationAux, operatorsAux, commentsAux;
    int lineCounter = 0, positionCounter = 0, symbolsTableIndex = 0;

    file.open(argv[2]);

    if (file.is_open()) {
        if (argv[1][1] == 'p' && objectFile.is_open()) {
            while (file) {
                getline(file, line);
                originalLine = line;

                if(line.empty()) continue;

                labelAux = line.find_first_of(':');
                if (labelAux != string::npos) {
                    label = line.substr(0, labelAux);
                    line = line.substr(labelAux+2);
                    operationAux = line.find_first_of(' ');
                    if (operationAux != string::npos) {
                        operation = line.substr(0, operationAux);
                        if (operation == "EQU") {
                            line = line.substr(operationAux+1);
                            operatorsAux = line.find_first_of(' ');
                            operator1 = line.substr(0, operatorsAux);
                            symbolsTable[symbolsTableIndex] = label;
                            equTable[symbolsTableIndex] = operator1;
                            symbolsTableIndex++;
                            continue;
                        } else if (operation == "IF") {
                            line = line.substr(operationAux+1);
                            operatorsAux = line.find_first_of(' ');
                            operator1 = line.substr(0, operatorsAux);
                            if (findSymbolOnTable(operator1, symbolsTable, 100)) {
                                string definition = getEquVal(operator1, symbolsTable, equTable);
                                getline(file, line);
                                if (definition != "0") {
                                    objectFile << line << endl;
                                }
                                continue;
                            }
                        }
                    }
                } else {
                    operationAux = line.find_first_of(' ');
                    if (operationAux != string::npos) {
                        operation = line.substr(0, operationAux);
                        if (operation == "IF") {
                            line = line.substr(operationAux+1);
                            operatorsAux = line.find_first_of(' ');
                            operator1 = line.substr(0, operatorsAux);
                            if (findSymbolOnTable(operator1, symbolsTable, 100)) {
                                string definition = getEquVal(operator1, symbolsTable, equTable);
                                getline(file, line);
                                if (definition != "0") {
                                    objectFile << line << endl;
                                }
                                continue;
                            }
                        }
                    }
                }
                objectFile << originalLine << endl;
            }

            objectFile.close();
        } else if (argv[1][1] == 'o') {
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

                        line = line.substr(labelAux + 2);
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
                                    if (operation == "COPY") {
                                        positionCounter++;
                                    }
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
                            if (operation == "COPY") {
                                positionCounter++;
                            }
                        } else if (findSymbolOnTable(operation, directivesTable, 8)) {
                        } else {
                            cout << "ERROR: Unknown operation -> " << operation << endl;
                            break;
                        }
                    }
                }
            }

            file.clear();
            file.seekg(0);

            if (objectFile.is_open()) {
                lineCounter = 0;
                positionCounter = 0;
                while (file) {
                    getline(file, line);
                    lineCounter++;

                    if (line.empty())
                        continue;

                    labelAux = line.find_first_of(':');
                    if (labelAux != string::npos) {
                        line = line.substr(labelAux + 2);

                        operationAux = line.find_first_of(' ');

                        if (operationAux != string::npos) {
                            operation = line.substr(0, operationAux);
                            line = line.substr(operationAux + 1);
                            commentsAux = line.find_first_of(';');
                            if (commentsAux != string::npos) {
                                operatorsAux = line.find_first_of(',');
                                if (operatorsAux != string::npos) {
                                    operator1 = line.substr(0, operatorsAux);
                                    operator2 = line.substr(operatorsAux + 2, commentsAux - operatorsAux - 2);
                                } else {
                                    operator1 = line;
                                    operator2 = "";
                                }
                            } else {
                                operatorsAux = line.find_first_of(',');
                                if (operatorsAux != string::npos) {
                                    operator1 = line.substr(0, operatorsAux);
                                    operator2 = line.substr(operatorsAux + 2);
                                } else {
                                    operator1 = line;
                                    operator2 = "";
                                }
                            }
                        } else {
                            operation = line;
                            operator1 = "";
                            operator2 = "";
                        }
                    } else {
                        operationAux = line.find_first_of(' ');
                        if (operationAux != string::npos) {
                            operation = line.substr(0, operationAux);
                            line = line.substr(operationAux + 1);
                            commentsAux = line.find_first_of(';');
                            if (commentsAux != string::npos) {
                                operatorsAux = line.find_first_of(',');
                                if (operatorsAux != string::npos) {
                                    operator1 = line.substr(0, operatorsAux);
                                    operator2 = line.substr(operatorsAux + 2, commentsAux - operatorsAux - 3);
                                } else {
                                    operator1 = line.substr(0, line.find_first_of(';')-1);
                                    operator2 = "";
                                }
                            } else {
                                operatorsAux = line.find_first_of(',');
                                if (operatorsAux != string::npos) {
                                    operator1 = line.substr(0, operatorsAux);
                                    operator2 = line.substr(operatorsAux + 2);
                                } else {
                                    operator1 = line.substr(0, line.find_first_of(';')-1);
                                    operator2 = "";
                                }
                            }
                        } else {
                            operation = line.substr(0, line.find_first_of(';')-1);
                            operator1 = "";
                            operator2 = "";
                        }
                    }

                    if (operation == "SECAO")
                        continue;

                    if (!isNumber(operator1) && !isHex(operator1) && !findSymbolOnTable(operator1, symbolsTable, 100)) {
                        cout << "ERROR: Symbol undefined -> " << operator1 << endl;
                        break;
                    }


                    if (!isNumber(operator2) && !findSymbolOnTable(operator2, symbolsTable, 100)) {
                        cout << operator2;
                        cout << "ERROR: Symbol undefined -> " << operator2 << endl;
                        break;
                    }

                    if (findSymbolOnTable(operation, instructionsTable, 14)) {
                        positionCounter++;
                        if (operation != "STOP") {
                            positionCounter++;
                            if (operation == "COPY") {
                                positionCounter++;
                            }
                        }


                        if (operation == "STOP") {
                            if (operator1 != "" || operator2 != "") {
                                cout << "ERROR: Invalid operands -> " << operator1 << " " << operator2 << endl;
                                break;
                            }

                            object += getOpcode(operation, instructionsTable, opcodes);
                        }

                        if (operation == "COPY") {
                            if (operator1 == "" || operator2 == "") {
                                cout << "ERROR: Invalid operands -> " << operator1 << " " << operator2 << endl;
                                break;
                            }

                            object += getOpcode(operation, instructionsTable, opcodes) + " " +
                                      to_string(getValue(operator1, symbolsTable, symbolsPositionTable)) + " " +
                                      to_string(getValue(operator2, symbolsTable, symbolsPositionTable));
                        }

                        if (operation != "COPY" && operation != "STOP") {
                            if (operator1 == "" || operator2 != "") {
                                cout << "ERROR: Invalid operands -> " << operator1 << " " << operator2 << endl;
                                break;
                            }

                            object + getOpcode(operation, instructionsTable, opcodes) + " " +
                            to_string(getValue(operator1, symbolsTable, symbolsPositionTable));
                        }
                    } else if (findSymbolOnTable(operation, directivesTable, 8)) {
                        positionCounter++;
                        if (operation == "SPACE") {
                            object += '0';
                        } else if (operation == "CONST") {
                            object += operator1;
                        }
                    } else {
                        cout << "ERROR: Unknown operation -> " << operation << endl;
                        break;
                    }

                    if (object.length() > 0 && object[object.length() - 1] != ' ')
                        object += " ";
                }
            }
        }
        object.pop_back();
        objectFile << object << endl;
        objectFile.close();
    }

    return 0;
}

bool findSymbolOnTable(string symbol, string table[], int quantityPositions) {
    for(int i = 0; i < quantityPositions; i++) {
        if (table[i] == symbol)
            return true;
    }
    return false;
}

string getOpcode(string instruction, string instructionsTable[], string opcodes[]) {
    for(int i = 0; i < 14; i++) {
        if (instructionsTable[i] == instruction)
            return opcodes[i];
    }
    return "";
}

int getValue(string symbol, string symbolsTable[], int values[]) {
    for(int i = 0; i < 100; i++) {
        if (symbolsTable[i] == symbol)
            return values[i];
    }
    return 0;
}

void printSymbolTable(string symbolsTable[], int symbolsPositionTable[], int index) {
    for(int i = 0; i < index; i++)
        cout << symbolsTable[i] << " " << symbolsPositionTable[i] << endl;
}

bool isNumber(string s) {
    if (s[0] != '-' && s[0] != '+') return false;
    s = s.substr(1);
    for (char const &c : s) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

bool isHex(string s) {
    if (s[0] != '0' || s[1] != 'x') return false;
    s = s.substr(2);
    for (char const &c : s) {
        if (c < '0' || (c > '9' && c < 'A') || (c > 'F' && c < 'a') || c > 'f') return false;
    }
    return true;
}

string getEquVal(string operator1, string symbolsTable[], string equTable[]) {
    for (int i = 0; i < 100; i++)
        if (operator1 == symbolsTable[i]) return equTable[i];
    return "";
}