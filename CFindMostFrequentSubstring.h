#pragma once
#include<iostream>
class CFindMostFrequentSubstring
{
public:
	CFindMostFrequentSubstring();
	CFindMostFrequentSubstring(unsigned char* _pucMain, int _nMainStrLen);
	// 分块前缀和方法
	int GetSubstring_DP(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet);
	// 暴力方法
	int GetSubstring_BruteForce(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet);

private:
	double CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow);

private:
	unsigned char* m_pucMain = nullptr;
	int m_nMainStrLen = 0;
};