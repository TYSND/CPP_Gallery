#pragma once
#include<iostream>
class CFindMostFrequentSubstring
{
public:
	CFindMostFrequentSubstring();
	CFindMostFrequentSubstring(unsigned char* _pucMain, int _nMainStrLen);
	// 后缀数组
	int GetSubstring_SA(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen);
	// 暴力方法
	int GetSubstring_BruteForce(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen);

private:
	double CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow);

private:
	unsigned char* m_pucMain = nullptr;
	int m_nMainStrLen = 0;

	// m_pnSuffixArray[i]表示：字典序排名第i的后缀的开始下标
	int* m_pnSuffixArray = nullptr;
	// m_pnRank[i]表示：开始下标为i的后缀的字典序排名
	int* m_pnRank = nullptr;
};