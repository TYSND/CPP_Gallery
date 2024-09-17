#pragma once
#include<iostream>

constexpr int MAX_CHARACTER_COUNT = 256;
constexpr int MAX_CHARACTER_DOUBLECOUNT = 512; // 两倍的最大字符数量，用于后续将向左向右扩展的子串分开

class CFindMostFrequentSubstring
{
public:
	CFindMostFrequentSubstring();
	CFindMostFrequentSubstring(unsigned char* _pucMain, int _nMainStrLen);
	~CFindMostFrequentSubstring();

	int GetSubString(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen);

	int Recur(int** _ppnFrequentSubStrBeginIndexs, int** _ppnNextSameSubStrBeginIndexs, int** _ppnExpendCharacterCounts, int* _pnRetSection, int _nMaxCharacterCount, int _nCurrentSubStrLen);

	int GetSubString1(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen);

private:
	inline double CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow);

private:
	unsigned char* m_pucMain = nullptr;
	int m_nMainStrLen = 0;
};