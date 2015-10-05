#pragma once

#include <string>
#include <Windows.h>
using namespace std;



void log(string strlog){
	OutputDebugString(strlog.c_str());
}