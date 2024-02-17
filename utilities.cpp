#include "utilities.h"
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

map<char, unsigned int> charToHex = { {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, 
{'8', 8}, {'9', 9}, {'a', 10}, {'b', 11} , {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15} };
map<unsigned int, char> hexToChar = { {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
{8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'} , {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'} };
map<unsigned int, char> intToB64 = { {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'},
{8, 'I'}, {9, 'J'}, {10, 'K'}, {11, 'L'} , {12, 'M'}, {13, 'N'}, {14, 'O'}, {15, 'P'}, {16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'},
{20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'} , {24, 'Y'}, {25, 'Z'}, {26, 'a'}, {27, 'b'} , {28, 'c'}, {29, 'd'}, {30, 'e'}, {31, 'f'} ,
{32, 'g'}, {33, 'h'}, {34, 'i'}, {35, 'j'} , {36, 'k'}, {37, 'l'}, {38, 'm'} , {39, 'n'}, {40, 'o'}, {41, 'p'}, {42, 'q'} , {43, 'r'},
{44, 's'}, {45, 't'}, {46, 'u'} , {47, 'v'}, {48, 'w'}, {49, 'x'}, {50, 'y'} , {51, 'z'}, {52, '0'}, {53, '1'}, {54, '2'} , {55, '3'},
{56, '4'}, {57, '5'}, {58, '6'} , {59, '7'}, {60, '8'}, {61, '9'}, {62, '+'} , {63, '/'}};
map<char, float> characterFrequencies = { {'a', 8.2} , {'b', 1.5} , {'c', 2.8} , {'d', 4.3} , {'e', 12.7} , {'f', 2.2} , {'g', 2.0} , {'h', 6.1} ,
{'i', 7.0} , {'j', 0.15} , {'k', 0.77} , {'l', 4.0} , {'m', 2.4} , {'n', 6.7} , {'o', 7.5} , {'p', 1.9} ,
{'q', 0.095} , {'r', 6.0} , {'s', 6.3} , {'t', 9.1} , {'u', 2.8} , {'v', 0.98} , {'w', 2.4} , {'x', 0.15} ,
{'y', 2.0} , {'z', 0.074} , {' ', 18.0} };

string hexToB64(string& hex)
{
	// for now we'll just assume an even number of characters
	if (hex.length() % 2 != 0)
	{
		cout << "hex string contains odd amount of characters" << endl;
		return "";
	}

	string b64 = "";
	int index = 0;
	int numCharacters = hex.length() - index;
	while (numCharacters > 0)
	{
		switch (numCharacters)
		{
		case 2:
			b64.push_back(charToHex[hex[index]] << 2 | charToHex[hex[index + 1]] >> 2);
			b64.push_back((charToHex[hex[index + 1]] & 0x0003) << 4 | 0);
			b64.push_back('=');
			b64.push_back('=');
			break;
		case 4:
			b64.push_back(intToB64[charToHex[hex[index]] << 2 | charToHex[hex[index + 1]] >> 2]);
			b64.push_back(intToB64[(charToHex[hex[index + 1]] & 0x0003) << 4 | charToHex[hex[index + 2]]]);
			b64.push_back(intToB64[charToHex[hex[index + 3]] << 2 | 0]);
			b64.push_back('=');
			break;
		default:
			b64.push_back(intToB64[charToHex[hex[index]] << 2 | charToHex[hex[index + 1]] >> 2]);
			b64.push_back(intToB64[(charToHex[hex[index + 1]] & 0x0003) << 4 | charToHex[hex[index + 2]]]);
			b64.push_back(intToB64[charToHex[hex[index + 3]] << 2 | charToHex[hex[index + 4]] >> 2]);
			b64.push_back(intToB64[(charToHex[hex[index + 4]] & 0x0003) << 4 | charToHex[hex[index + 5]]]);
			break;
		}

		index += 6;
		numCharacters = hex.length() - index;
	}

	return b64;
}

string hexor(string& hex1, string& hex2)
{
	if (hex1.length() != hex2.length())
	{
		cout << "the two hex strings are not equal length" << endl;
		return "";
	}

	string hex = "";
	for (int i = 0; i < hex1.length(); i++)
	{
		hex.push_back(hexToChar[charToHex[hex1[i]] ^ charToHex[hex2[i]]]);
	}

	return hex;
}


// soooo the frequencies i found didn't include the space character and im too lazy to include it
// so im just making up a value for space and then adjusting everything else by it
void normalizeFrequenciesBySpace()
{
	float goink = 100.0 - characterFrequencies[' '];

	for (auto& x : characterFrequencies)
	{
		if (x.first == ' ')
			continue;
		else
		{
			float ratio = x.second / 100.0;
			x.second = ratio * goink;
		}
	}
}

string singleByteXor(string& cipher)
{
	string message = "";
	int c = 0;
	vector<pair<float, int>> scores;

	while (c < 256)
	{
		float score = 0.0;
		for (int i = 0; i < cipher.length(); i+=2)
		{
			unsigned char letter;
			letter = charToHex[cipher[i]] << 4 | charToHex[cipher[i+1]];
			letter ^= c;

			score += characterFrequencies[letter];
		}

		scores.push_back({ score, c });
		c++;
	}

	sort(scores.begin(), scores.end());

	for (int i = 0; i < 10; i++)
	{
		int scoreIndex = scores.size() - 1 - i;
		for (int j = 0; j < cipher.length(); j+=2)
		{
			unsigned char letter;
			letter = charToHex[cipher[j]] << 4 | charToHex[cipher[j + 1]];
			letter ^= scores[scoreIndex].second;
			message.push_back(letter);
		}

		auto& x = scores[scoreIndex];
		cout << x.first << ", " << x.second << endl;
		cout << message << endl;
		message.clear();
	}

	return message;
}

void utilityTest()
{
	normalizeFrequenciesBySpace();
	string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string comparison = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	string b64 = hexToB64(hex);

	string hex1 = "1c0111001f010100061a024b53535009181c";
	string hex2 = "686974207468652062756c6c277320657965";
	string comparison2 = "746865206b696420646f6e277420706c6179";
	string xorString = hexor(hex1, hex2);

	string cipher = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	string message = singleByteXor(cipher);

	cout << message << endl;

	if (b64 != comparison)
		cout << "test not passed" << endl;
	if (xorString != comparison2)
		cout << "test not passed" << endl;

	cout << "test passed if not not passed" << endl;
}