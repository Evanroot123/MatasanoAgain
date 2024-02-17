#pragma once

#include <iostream>
using namespace std;

string hexToB64(string& hex);
string hexor(string& hex1, string& hex2);
void normalizeFrequenciesBySpace();
string singleByteXor(string& cipher);
void utilityTest();