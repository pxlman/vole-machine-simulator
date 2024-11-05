#ifndef VOLE_MACHINE_ALU_H
#define VOLE_MACHINE_ALU_H
#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <iostream>
#include <deque>
#include "machine/Register.h"
using namespace std;

class ALU {
private:
    float binToDec(const string& binary) {
        float decimal = 0;
        int length = binary.length();

        for (int i = 0; i < length; ++i) {
            if (binary[length - 1 - i] == '1') {
                decimal += (pow(2, i));
            }
        }

        return decimal;
    }

    string decimalToBinary(float number) {
        // Split the number into integer and fractional parts
        string bin = to_string(number);
        bool sign = bin[0] == '-' ? true : false;
        int integerPart = static_cast<int>(number);
        float fractionalPart = number - integerPart;
        fractionalPart = abs(fractionalPart);
        // Convert the integer part to binary
        string binary = "";
        if (integerPart == 0) {
            binary = "0";
        } else {
            while (integerPart > 0) {
                binary = to_string(integerPart % 2) + binary;
                integerPart /= 2;
            }
        }

        // Append the radix point for the fractional part
        binary += ".";

        // Convert the fractional part to binary
        int precision = 8; // Number of bits for fractional precision
        while (precision > 0 && abs(fractionalPart) > 0) {
            fractionalPart *= 2;
            if (fractionalPart >= 1) {
                binary += "1";
                fractionalPart -= 1;
            } else {
                binary += "0";
            }
            precision--;
        }

        if(sign)
            binary = '-' + binary;

        return binary;
    }

    string decimalToBinary(int dec) {
        string binary = "";
        while(dec != 0) {
            binary = to_string(dec % 2) + binary;
            dec = dec / 2;
        }
        while(binary.size() != 8) {
            binary = '0' + binary;
        }
        return binary;
    }

    string binToHex(string &convertedbin) {
        string firstNibble = convertedbin.substr(0, 4);
        string secondNibble = convertedbin.substr(4, 4);

        int decimalValue1 = stoi(firstNibble, nullptr, 2);
        int decimalValue2 = stoi(secondNibble, nullptr, 2);

        string hex = "";
        hex += (decimalValue1 < 10) ? ('0' + decimalValue1) : ('A' + decimalValue1 - 10);
        hex += (decimalValue2 < 10) ? ('0' + decimalValue2) : ('A' + decimalValue2 - 10);

        return hex;
    }

    string convertedBinToDec(std::string number) {
        // Check if the number is negative and remove the sign if it exists
        bool isNegative = (number[0] == '-');
        if (isNegative) {
            number = number.substr(1);
        }

        // Find the position of the radix point
        size_t pointPosition = number.find('.');
        if (pointPosition == std::string::npos) {
            pointPosition = number.length();
        }

        // Locate the first '1' to normalize the number
        size_t firstOne = number.find('1');
        if (firstOne == std::string::npos) {
            return "0"; // If there are no '1's, the number is 0
        }

        // Calculate the exponent based on the position of the first '1'
        int exponent = pointPosition - firstOne;

        // Generate the mantissa from the first '1', skipping the leading bit (implicit representation)
        std::string mantissa = number.substr(firstOne + 1); // Skip the first '1'

        // Remove the radix point from the mantissa, if present
        size_t newPointPosition = mantissa.find('.');
        if (newPointPosition != std::string::npos) {
            mantissa.erase(newPointPosition, 1);
        }

        // Ensure the mantissa is limited to a certain number of bits (e.g., 4 bits for simplicity)
        if (mantissa.length() > 4) {
            mantissa = mantissa.substr(0, 4);
        } else {
            while (mantissa.length() < 4) {
                mantissa += '0'; // Pad with zeros if needed
            }
        }

        // Calculate the biased exponent
        int bias = 4; // Assuming a bias of 4 for a 3-bit exponent
        int exponentValue = exponent + bias;

        // Convert the exponent to a 3-bit binary string
        std::string exponentBinary = "";
        for (int i = 2; i >= 0; --i) {
            exponentBinary += ((exponentValue >> i) & 1) ? '1' : '0';
        }

        // Combine sign, exponent, and mantissa into the final binary representation
        std::string normalizedBinary = (isNegative ? "1" : "0") + exponentBinary + mantissa;
        return normalizedBinary;
    }


public:
    ALU(){}
    int hextodec(string hex) {
        int dec = 0;
        int base = 1;
        for (int i = hex.length() - 1; i >= 0; i--) {
            if (hex[i] >= '0' && hex[i] <= '9') {
                dec += (hex[i] - '0') * base;
            }
            else if (hex[i] >= 'A' && hex[i] <= 'F') {
                dec += (hex[i] - 'A' + 10) * base;
            }
            base *= 16;
        }
        return dec;
    }

    string decToHex(int dec){
        string hex;
        int temp = 0;
        map<int, char> hexmap{
            {10, 'A'}, {11, 'B'}, {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}
        };
        while(dec != 0) {
            temp = dec % 16;
            dec = dec / 16;
            if(temp < 10) hex = char(temp + '0') + hex;
            else {
                hex = hexmap[temp] + hex;
            }
        }
        return hex.empty() ? "0" : hex;
    }
    void addFloat(int id1, int id2, int idx, Register &reg) {
        int num1 = hextodec(reg.getReg(id1));
        int num2 = hextodec(reg.getReg(id2));
        float decimal1 = stof(FloatRep(num1));
        float decimal2 = stof(FloatRep(num2));
        float res = decimal1 + decimal2;
        string ans = decimalToBinary(res);
        string result = convertedBinToDec(ans);
        string hex = binToHex(result);
        reg.setReg(idx,hex);
    }
    void addTwosC(int id1, int id2, int idx, Register &reg) {
        int num1 = hextodec(reg.getReg(id1));
        int num2 = hextodec(reg.getReg(id2));
        if(num1 > 127) {
            num1 -= 256;
        }
        if(num2 > 127) {
            num2 -= 256;
        }
        int res = num1 + num2;
        if(res > 127) {
            res -= 256;
        }
        else if(res < -128) {
            res += 256;
        }
        string twosHex = decToHex(res);
        reg.setReg(idx, (twosHex.length() == 2)? twosHex : "0" + twosHex);
    }
// Function to convert from hex to binary
string Hextobin(string Hex){
    int dec = hextodec(Hex);
    return decimalToBinary(dec);
}

void reverse(int idx, int& deg, Register &r) {
    string hex = r.getReg(idx);
    string bin = Hextobin(hex); // Converting hex to binary

    deque<char> dq; // Creating a deque

    // Pushing elements in the deque
    for(int i = 0; i < bin.length(); i++) {
        dq.push_back(bin[i]);
    }
    // Initializing an empty string
    string reversed = "";

    // Looping until degree equals 0 or dq is empty
    while(deg != 0 && !dq.empty()) {
        // Creating temporary var
        char temp = dq.back();

        // Adding the temp to the front of the string
        reversed = temp + reversed;

        // Popping the last elements in the dq
        dq.pop_back();

        // Decrementing deg
        deg--;
    }

    // Appending the remaining elements in the dq to the string
    while(!dq.empty()) {
        reversed += dq.front();
        dq.pop_front();
    }

    // Setting the register
    r.setReg(idx, binToHex(reversed));
}

string binToHex(const string & string);

void ORing(int id1, int id2,int idx,Register &r) { // Bit wise ORing function string hex1 = alu // Converting the numbers to binary
    string hex1 = r.getReg(id1);
    string hex2 = r.getReg(id2);
    string bin1 = Hextobin(hex1);
    string bin2 = Hextobin(hex2);

    // ORing the bits of the two binary's together
    int ans = stoi(bin1,nullptr,2) | stoi(bin2, nullptr,2);

    // Setting the register
    r.setReg(idx, binToHex(decimalToBinary(ans)));
}

void Anding(int id1, int id2, int idx, Register &r) {
    string hex1 = r.getReg(id1);
    string hex2 = r.getReg(id2);
    // Converting the numbers to binary
    string bin1 = Hextobin(hex1);
    string bin2 = Hextobin(hex2);

    // ANDing the bits of the two binary's together
    int ans = stoi(bin1,nullptr,2) & stoi(bin2, nullptr, 2);

    // Setting the register
    r.setReg(idx, binToHex(to_string(ans)));
}

void Xoring(int id1, int id2, int idx, Register &r) {
    string hex1 = r.getReg(id1);
    string hex2 = r.getReg(id2);
    // Converting the numbers to binary
    string bin1 = Hextobin(hex1);
    string bin2 = Hextobin(hex2);

    // XORing the bits of the two binary's together
    int ans = stoi(bin1,nullptr,2) ^ stoi(bin2, nullptr, 2);

    // Setting the register
    r.setReg(idx, binToHex(to_string(ans)));
}
    string FloatRep(int dec) {
        string exponent;
        string dectobin = decimalToBinary(dec);
        int bias = 4; // Bias for the exponent
        int sign = dectobin[0] == '1' ? 1 : 0;

        // Extract the exponent (3 bits)
        for (int i = 1; i <= 3; i++) {
            exponent += dectobin[i];
        }

        // Extract the mantissa (4 bits)
        string mantisa = "";
        for (int i = 4; i < 8; i++) {
            mantisa += dectobin[i];
        }

        // Calculate the actual mantissa value as a decimal
        float mantissaValue = 0.0; // Start with explicit leading 0
        for (int i = 0; i < mantisa.length(); i++) {
            mantissaValue += (mantisa[i] - '0') * pow(2, -(i + 1)); // Binary fraction
        }

        // Convert exponent from binary to decimal
        int exponentValue = stoi(exponent, nullptr, 2) - bias;

        // Calculate the floating-point result
        string result = to_string(pow(-1, sign) * mantissaValue * pow(2, exponentValue));

        if(stof(result) > 7.5) {
            result = to_string(7.5);
        }
        if(stof(result) < -7.5) {
            result = to_string(-7.5);
        }
        // Convert result to string
        return result;
    }

};

#endif //VOLE_MACHINE_ALU_H
