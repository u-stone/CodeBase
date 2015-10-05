
#include <iostream>
using namespace std;

#include <string>
#include <algorithm>

std::string multiply(const std::string& operand1, const std::string& operand2);

int main()
{
	std::string operand1 = "22";
	std::string operand2 = "39";
	
	std::string ret = multiply(operand1, operand2);
	cout << operand1 + " * " + operand2 + " = " + ret << endl;
}

inline bool isnum(char c){
	return '0' <= c && c <= '9';
}
inline int _atoi(char c){
	if (isnum(c)){
		return c-'0';
	} else {
		throw "invalid parameter";
	}
}

inline char _itoa_(unsigned char c){
	if (0 <= c && c <= 9)
		return c + '0';
	else
		throw "invalid parameter";
}

std::string digit2str(unsigned char* digits, int count){
	std::string strRet;
	strRet.resize(count);
	for (int i = 0; i < count; ++i){
		strRet[i] = _itoa_(digits[i]);
	}
	return strRet;
}

#define UNIT 10

std::string multiply(const std::string& operand1, const std::string& operand2)
{
	std::string ret;
	if (operand1.length() * operand2.length() == 0)
		return "0";
	
	int len = operand1.length();
	for (int i = 0; i < len; ++i){
		if (!(isnum(operand1[i]) || operand1[i] == '.'))
			throw "operand1 is not a valid number";
	}
	len = operand2.length();
	for (int i = 0; i < len; ++i){
		if (!(isnum(operand2[i]) || operand2[i] == '.'))
			throw "operand2 is not a valid number";
	}

	if (operand1.length() < operand2.length()){
		
	}

	char** draftBuf = (char**)malloc(operand1.length() * sizeof(void*));
	memset(draftBuf, 0, operand1.length() * sizeof(void*));
	for (int i = 0; i < operand1.length(); ++i){
		draftBuf[i] = (char*)malloc(operand1.length() + operand2.length()+1);
		memset(draftBuf[i], 0, operand1.length() + operand2.length()+1);
	}
	
	typedef unsigned char ubyte;
	ubyte carrydigit = 0, tmp = 0;

	//convert char str to byte str
	ubyte *oper1 = (ubyte*)malloc(operand1.length());
	ubyte *oper2 = (ubyte*)malloc(operand2.length());
	memset(oper1, 0, operand1.length());
	memset(oper2, 0, operand2.length());

	for (int i = operand1.length() - 1; i >= 0; --i)
		oper1[i] = _atoi(operand1[i]);
	for (int i = operand2.length() - 1; i >= 0; --i)
		oper2[i] = _atoi(operand2[i]);

	//multiply
	for (int i = 0; i < operand1.length(); ++i){
		for (int j = 0; j < operand2.length(); ++j){
			tmp = oper1[i] * oper2[j] + carrydigit;
			draftBuf[i][i+j+1] += tmp % UNIT;
			carrydigit = tmp / UNIT;
		}
		if (carrydigit){
			draftBuf[i][i] = carrydigit;
			carrydigit = 0;
		}
	}
	
	//add 
	unsigned int iTmp = 0, carrydigit_added = 0;//avoid of overflow by using int iTmp,not tmp;
	ubyte* retbyte = (ubyte*)malloc(operand1.length() + operand2.length());
	memset(retbyte, 0, operand1.length() + operand2.length());
	for (int i = operand1.length() + operand2.length() - 1; i >= 0; --i){
		iTmp = carrydigit_added;
		for (int j = 0; j < operand1.length(); ++j){
			iTmp += draftBuf[j][i];
		}
		retbyte[i] = iTmp % UNIT;
		carrydigit_added = iTmp / UNIT;
	}
	if (carrydigit_added){
		retbyte[0] += carrydigit_added;
		if (retbyte[0] / UNIT){
			throw "最高位居然有进位!!!";
		}
	}
	ret = digit2str(retbyte, operand1.length() + operand2.length());
	free(oper1);
	free(oper2);
	for (int i = 0; i < operand1.length(); ++i){
		free(draftBuf[i]);
		draftBuf[i] = 0;
	}
	free(draftBuf);
	free(retbyte);
	
	return ret;
}