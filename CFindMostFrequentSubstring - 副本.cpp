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

int CFindMostFrequentSubstring::GetSubstring_SA(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen)
{
	int nCharacter = 255;
	int nNewMainStrLen = std::max(m_nMainStrLen + 1, nCharacter + 1);
	unsigned char* pcNewMain = nullptr;

	int* pnCnt = nullptr, *pnOldRank = nullptr, *pnID = nullptr;
	try
	{
		pcNewMain = new unsigned char[nNewMainStrLen];
		m_pnSuffixArray = new int[nNewMainStrLen];
		m_pnRank = new int[nNewMainStrLen];
		pnOldRank = new int[nNewMainStrLen];
		pnCnt = new int[nNewMainStrLen];
		pnID = new int[nNewMainStrLen];

		memcpy(pcNewMain + 1, m_pucMain, sizeof(unsigned char) * m_nMainStrLen);
		memset(m_pnSuffixArray, 0, sizeof(int) * nNewMainStrLen);
		memset(m_pnRank, 0, sizeof(int) * nNewMainStrLen);
		memset(pnOldRank, 0, sizeof(int) * nNewMainStrLen);
		memset(pnCnt, 0, sizeof(int) * nNewMainStrLen);
	}
	catch (...)
	{
		if (pcNewMain != nullptr) delete[] pcNewMain;
		if (m_pnSuffixArray != nullptr) delete[] m_pnSuffixArray;
		if (m_pnRank != nullptr) delete[] m_pnRank;
		if (pnOldRank != nullptr) delete[] pnOldRank;
		if (pnCnt != nullptr) delete[] pnCnt;
		if (pnID != nullptr) delete[] pnID;
		

		pcNewMain = nullptr;
		pnOldRank = m_pnSuffixArray = m_pnRank = pnCnt = pnID = nullptr;
		return -1;
	}

	for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
	{
		m_pnRank[nIndex] = pcNewMain[nIndex];
		++pnCnt[m_pnRank[nIndex]];
	}
	
	for (int nIndex = 1; nIndex <= nCharacter; nIndex++)
	{
		pnCnt[nIndex] += pnCnt[nIndex - 1];
	}

	for (int nIndex = m_nMainStrLen; nIndex >= 1; nIndex--)
	{
		m_pnSuffixArray[pnCnt[m_pnRank[nIndex]]--] = nIndex;
	}

	memcpy(pnOldRank + 1, m_pnRank + 1, m_nMainStrLen * sizeof(int));

	for (int nScore = 0, nIndex = 1; nIndex <= m_nMainStrLen; ++nIndex)
	{
		if (pnOldRank[m_pnSuffixArray[nIndex]] == pnOldRank[m_pnSuffixArray[nIndex - 1]]) m_pnRank[m_pnSuffixArray[nIndex]] = nScore;
		else m_pnRank[m_pnSuffixArray[nIndex]] = ++nScore;
	}

	// 基数排序
	for (int w = 1; w < m_nMainStrLen; w <<= 1, nCharacter = m_nMainStrLen)
	{
		// 第二关键字
		memset(pnCnt, 0, sizeof(int) * nNewMainStrLen);
		memcpy(pnID + 1, m_pnSuffixArray + 1, m_nMainStrLen * sizeof(int));
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			++pnCnt[m_pnRank[pnID[nIndex] + w]];
		}

		for (int nIndex = 1; nIndex <= nCharacter; nIndex++)
		{
			pnCnt[nIndex] += pnCnt[nIndex - 1];
		}

		for (int nIndex = m_nMainStrLen; nIndex >= 1; nIndex--)
		{
			m_pnSuffixArray[pnCnt[m_pnRank[pnID[nIndex] + w]]--] = pnID[nIndex];
		}

		// 第一关键字
		memset(pnCnt, 0, sizeof(int) * nNewMainStrLen);
		memcpy(pnID + 1, m_pnSuffixArray + 1, m_nMainStrLen * sizeof(int));
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			++pnCnt[m_pnRank[pnID[nIndex]]];
		}

		for (int nIndex = 1; nIndex <= nCharacter; nIndex++)
		{
			pnCnt[nIndex] += pnCnt[nIndex - 1];
		}

		for (int nIndex = m_nMainStrLen; nIndex >= 1; nIndex--)
		{
			m_pnSuffixArray[pnCnt[m_pnRank[pnID[nIndex]]]--] = pnID[nIndex];
		}


		memcpy(pnOldRank + 1, m_pnRank + 1, sizeof(int) * m_nMainStrLen);
		for (int nScore = 0, nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			if (pnOldRank[m_pnSuffixArray[nIndex]] == pnOldRank[m_pnSuffixArray[nIndex - 1]]
				&& pnOldRank[m_pnSuffixArray[nIndex] + w] == pnOldRank[m_pnSuffixArray[nIndex - 1] + w])
			{
				m_pnRank[m_pnSuffixArray[nIndex]] = nScore;
			}
			else
			{
				m_pnRank[m_pnSuffixArray[nIndex]] = ++nScore;
			}
		}

	}

	return 0;
}

string max_freq_substring(const string& s)
{
	build_sa(s);
	build_lcp(s);

	vector<pair<int, int>> lcp_pos(n);
	for (int i = 0; i < n; ++i)
		lcp_pos[i].second = i;

	for (int i = n - 1; i >= 0; --i)
	{
		int j = sa[i];
		lcp_pos[j].first = i;
	}

	vector<int> freq(n);
	for (int i = 0; i < n; ++i)
		freq[lcp_pos[i].second]++;

	int max_freq = 0;
	string max_freq_substring;

	for (int i = 0; i < n; ++i)
	{
		if (freq[i] > max_freq)
		{
			max_freq = freq[i];
			max_freq_substring = s.substr(lcp_pos[i].first, lcp_pos[i].second - lcp_pos[i].first);
		}
	}

	return max_freq_substring;
}


int CFindMostFrequentSubstring::GetSubstring_BruteForce(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen)
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
		_nRetLen = nRetLen;
		return 0;
	}
}




double CFindMostFrequentSubstring::CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow)
{
	return pow(_nRepeatCount, _dFrequencyPow) * pow(_nLength, _dLengthPow);
}