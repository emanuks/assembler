#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

string getSymbol(string);

string getValue(string);

int getSymbolValue(string, string [], string [], string [], string [], int , int , int);

bool changeIndex(string [], int, int);

int main (int argc, char **argv) {
    ifstream file1, file2;
    ofstream objectFile("linked_file.o");
    string lineFile1, lineFile2, originalLine, value, object = "";
    string useTableSymbol[2][100];
    string useTableValue[2][100];
    string defTableSymbol[2][100];
    string defTableValue[2][100];
    string instructions[2];
    int useTableIndex[] = {0, 0};
    int defTableIndex[] = {0, 0};
    int correctionFactor = 0, actualPosition = 0, actualIndex = 0, remainingOperators = 0;
    bool isTable = false, isRelative = false;
    size_t positionAux;

    file1.open(argv[1]);
    file2.open(argv[2]);

    if (objectFile.is_open()) {
        if (file1.is_open() && file2.is_open()) {
            while (file1) {
                getline(file1, lineFile1);

                if (lineFile1.empty()) continue;

                if (lineFile1 == "TABELA USO") {
                    isTable = true;
                    while (isTable) {
                        getline(file1, lineFile1);
                        if (lineFile1.empty()) {
                            isTable = false;
                        } else {
                            useTableSymbol[0][useTableIndex[0]] = getSymbol(lineFile1);
                            useTableValue[0][useTableIndex[0]] = getValue(lineFile1);
                            useTableIndex[0]++;
                        }
                    }
                } else if (lineFile1 == "TABELA DEF") {
                    isTable = true;
                    while (isTable) {
                        getline(file1, lineFile1);
                        if (lineFile1.empty()) {
                            isTable = false;
                        } else {
                            defTableSymbol[0][defTableIndex[0]] = getSymbol(lineFile1);
                            defTableValue[0][defTableIndex[0]] = getValue(lineFile1);
                            defTableIndex[0]++;
                        }
                    }
                } else {
                    instructions[0] = lineFile1;
                }
            }

            while (file2) {
                getline(file2, lineFile2);

                if (lineFile2.empty()) continue;

                if (lineFile2 == "TABELA USO") {
                    isTable = true;
                    while (isTable) {
                        getline(file2, lineFile2);
                        if (lineFile2.empty()) {
                            isTable = false;
                        } else {
                            useTableSymbol[1][useTableIndex[1]] = getSymbol(lineFile2);
                            useTableValue[1][useTableIndex[1]] = getValue(lineFile2);
                            useTableIndex[1]++;
                        }
                    }
                } else if (lineFile2 == "TABELA DEF") {
                    isTable = true;
                    while (isTable) {
                        getline(file2, lineFile2);
                        if (lineFile2.empty()) {
                            isTable = false;
                        } else {
                            defTableSymbol[1][defTableIndex[1]] = getSymbol(lineFile2);
                            defTableValue[1][defTableIndex[1]] = getValue(lineFile2);
                            defTableIndex[1]++;
                        }
                    }
                } else {
                    instructions[1] = lineFile2;
                }
            }

            originalLine = instructions[0];
            while (true) {
                positionAux = originalLine.find_first_of(' ');
                correctionFactor++;

                if (positionAux == string::npos)
                    break;

                originalLine = originalLine.substr(positionAux+1);
            }

            for (int i = 0; i < defTableIndex[1]; i++)
                defTableValue[1][i] = to_string(stoi(defTableValue[1][i]) + correctionFactor);

            originalLine = instructions[0];
            while (true) {
                positionAux = originalLine.find_first_of(' ');

                if (positionAux == string::npos) {
                    if (remainingOperators > 0) {
                        object += to_string(getSymbolValue(originalLine, useTableSymbol[0], useTableValue[0], defTableSymbol[1], defTableValue[1], 0, actualPosition, actualIndex));
                        if (changeIndex(useTableValue[0], actualPosition, actualIndex)) {
                            actualIndex++;
                        }
                    } else {
                        object += originalLine;
                    }
                    break;
                }

                value = originalLine.substr(0, positionAux);
                originalLine = originalLine.substr(positionAux+1);

                if (remainingOperators > 0) {
                    object += to_string(getSymbolValue(value, useTableSymbol[0], useTableValue[0], defTableSymbol[1], defTableValue[1], 0, actualPosition, actualIndex));
                    if (changeIndex(useTableValue[0], actualPosition, actualIndex)) {
                        actualIndex++;
                    }
                } else {
                    object += value;
                }

                if (remainingOperators == 0) {
                    if (value == "9" || value == "09") {
                        remainingOperators = 2;
                    } else {
                        remainingOperators = 1;
                    }
                } else {
                    remainingOperators--;
                }

                object += " ";

                actualPosition++;
            }

            originalLine = instructions[1];
            actualPosition = 0;
            actualIndex = 0;
            object += " ";
            while (true) {
                positionAux = originalLine.find_first_of(' ');

                if (positionAux == string::npos) {
                    if (remainingOperators > 0) {
                        object += to_string(getSymbolValue(originalLine, useTableSymbol[1], useTableValue[1], defTableSymbol[0], defTableValue[0], correctionFactor, actualPosition, actualIndex));
                        if (changeIndex(useTableValue[1], actualPosition, actualIndex)) {
                            actualIndex++;
                        }
                    } else {
                        object += originalLine;
                    }
                    break;
                }

                value = originalLine.substr(0, positionAux);
                originalLine = originalLine.substr(positionAux+1);

                if (remainingOperators > 0) {
                    object += to_string(getSymbolValue(value, useTableSymbol[1], useTableValue[1], defTableSymbol[0], defTableValue[0], correctionFactor, actualPosition, actualIndex));
                    if (changeIndex(useTableValue[1], actualPosition, actualIndex)) {
                        actualIndex++;
                    }
                } else {
                    object += value;
                }

                if (remainingOperators == 0) {
                    if (value == "9" || value == "09") {
                        remainingOperators = 2;
                    } else {
                        remainingOperators = 1;
                    }
                } else {
                    remainingOperators--;
                }

                object += " ";

                actualPosition++;
            }
        }
    }

    objectFile << object << endl;
    objectFile.close();

    return 0;
}

string getSymbol(string s) {
    return s.substr(0, s.find_first_of(' '));
}

string getValue(string s) {
    return s.substr(s.find_first_of(' ')+1);
}

int getSymbolValue(string line, string useTableSymbol[100], string useTableValue[100], string defTableSymbol[100], string defTableValue[100], int correctionFactor, int actualPosition, int actualIndex) {
    if (useTableValue[actualIndex].empty())
        return stoi(line) + correctionFactor;

    int useValue = stoi(useTableValue[actualIndex]);
    string symbol;
    if (useValue == actualPosition) {
        symbol = useTableSymbol[actualIndex];
        for (int i = 0; i < 100; i++)
            if (symbol == defTableSymbol[i])
                return stoi(defTableValue[i]) + stoi(line);
    } else {
        return stoi(line) + correctionFactor;
    }

    return 0;
}

bool changeIndex(string useTableValue[100], int actualPosition, int actualIndex) {
    if (useTableValue[actualIndex].empty()) return false;

    int useValue = stoi(useTableValue[actualIndex]);
    if (useValue == actualPosition)
        return true;
    return false;
}