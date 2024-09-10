#include "CFindMostFrequentSubstring.h"

CFindMostFrequentSubstring::CFindMostFrequentSubstring()
{
	m_pucMain = nullptr;
}

CFindMostFrequentSubstring::CFindMostFrequentSubstring(unsigned char* _pucMain, int _nMainStrLen)
{
	if (_pucMain == nullptr || _nMainStrLen == 0)
	{
		m_pucMain = nullptr;
		m_nMainStrLen = 0;
	}
	else
	{
		m_pucMain = (unsigned char*)malloc(sizeof(unsigned char) * _nMainStrLen);
		if (m_pucMain != nullptr)
		{
			memcpy(m_pucMain, _pucMain, sizeof(unsigned char) * _nMainStrLen);
			m_nMainStrLen = _nMainStrLen;
		}
		else
		{
			m_nMainStrLen = 0;
		}
	}
}

int CFindMostFrequentSubstring::GetSubstring_DP(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet)
{
	// 想法：dp[i][j] 以字符i开始，且长度为j的字符出现的次数

}

int CFindMostFrequentSubstring::GetSubstring_BruteForce(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet)
{
#define GET_FIRST_LARGEEQUAL_POW2(_OriginValue, _Pow2Value) (_OriginValue + (_Pow2Value - 1)) & ~(_Pow2Value - 1)
	if (_ppucRet == nullptr || m_pucMain == nullptr) return -1;

	// 暴力：枚举子串起点，枚举子串终点、遍历母串查找所有等长的子串，判断是否相等并计数，返回最大值

	int nRetBegin = -1;
	int nRetLen = 0;
	double dRetWeight = 0;

	int nSubLen = 0, nRepeatCount = 0;

	for (int nSubBegin = 0; nSubBegin < m_nMainStrLen; nSubBegin++)
	{
		for (int nSubEnd = nSubBegin; nSubEnd < m_nMainStrLen; nSubEnd++)
		{
			nSubLen = nSubEnd - nSubBegin + 1;
			nRepeatCount = 0;
			for (int nCmpBegin = 0; nCmpBegin <= m_nMainStrLen - nSubLen; nCmpBegin++)
			{
				if (memcmp(m_pucMain + nSubBegin, m_pucMain + nCmpBegin, sizeof(unsigned char) * nSubLen) == 0) // 当nSubLen较大时，两端可采用memcmp，中间可以强转uint64*进行循环比较
				{
					nRepeatCount++;
				}
			}
			double dCurrWeight = CalcWeight(nRepeatCount, nSubLen, _dFrequencyPow, _dLengthPow);
			if (dCurrWeight - dRetWeight > 0.000001)
			{
				nRetBegin = nSubBegin;
				nRetLen = nSubLen;
				dRetWeight = dCurrWeight;
			}
		}
	}
	if (nRetBegin >= 0 && nRetLen > 0)
	{
		*_ppucRet = (unsigned char*)malloc(nRetLen * sizeof(unsigned char));
		if (*_ppucRet == nullptr) return -1;
		
		memcpy(*_ppucRet, m_pucMain + nRetBegin, nRetLen * sizeof(unsigned char));
		return 0;
	}
}




double CFindMostFrequentSubstring::CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow)
{
	return pow(_nRepeatCount, _dFrequencyPow) * pow(_nLength, _dLengthPow);
}