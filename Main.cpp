#include<iostream>
#include<string>
#include"CFindMostFrequentSubstring.h"

int main()
{
	std::string str = "ababababcadcadcadcad";

	CFindMostFrequentSubstring stCFMFS((unsigned char*)str.c_str(), str.size(), 1);

	unsigned char* pnRet = nullptr;
	int nRetLen = 0;
	stCFMFS.GetSubString(1, 1, &pnRet, nRetLen);

	return 0;
}