#include "CFindMostFrequentSubstring.h"

// 向左扩展的字符用[0, MAX_CHARACTER_COUNT - 1]存储，向右扩展的字符用[MAX_CHARACTER_COUNT, 2 * MAX_CHARACTER_COUNT - 1]存储
#define GET_CHARACTER_DOUBLESIDE_HASH(_Character, _bLeft) (_bLeft ? _Character : _Character + MAX_CHARACTER_COUNT)

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
		m_pucMain = new unsigned char[_nMainStrLen];
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

CFindMostFrequentSubstring::~CFindMostFrequentSubstring()
{
	if (m_pucMain != nullptr) delete[] m_pucMain;
	m_nMainStrLen = 0;
}

int CFindMostFrequentSubstring::GetSubString(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen)
{
	if (_ppucRet == nullptr)
	{
		return -1;
	}
	if (m_pucMain == nullptr)
	{
		return 0;
	}


	/* 需要的数据结构
	* 数组，存储当前长度所有最频繁子串的开始下标
	* 数组，arr[i]表示以m_pucMain[i]开始的子串，下一个相同子串的开始下标
	* 两个一维数组，分别存储当前子串向左和向右的一个字符的数量（用于寻找长度+1的最频繁子串）
	*/
		
	// 存储相同子串链表的头指针
	int** ppnFrequentSubStrBeginIndexs = nullptr;
	// 下一个相同子串的开始下标。若没有，则是-1
	int** ppnNextSameSubStrBeginIndexs = nullptr;
	// 存储迭代时子串扩展出去的字符数量
	int** ppnExpendCharacterCounts = nullptr;
	// 结果集。pnRetSection[i]表示长度为i的最频繁子串的开始下标。暂时每个长度只存1个子串
	int* pnRetSection = nullptr;


	// ppnFrequentSubStrBeginIndexs和ppnNextSameSubStrBeginIndexs已用的下标
	int nUsageIndex = -1;

	try
	{
		ppnFrequentSubStrBeginIndexs = new int*[m_nMainStrLen + 1];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnFrequentSubStrBeginIndexs[nIndex] = new int[MAX_CHARACTER_DOUBLECOUNT];
			std::fill(ppnFrequentSubStrBeginIndexs[nIndex], ppnFrequentSubStrBeginIndexs[nIndex] + MAX_CHARACTER_DOUBLECOUNT, -1);
			
		}

		ppnNextSameSubStrBeginIndexs = new int*[m_nMainStrLen];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnNextSameSubStrBeginIndexs[nIndex] = new int[m_nMainStrLen];
			std::fill(ppnNextSameSubStrBeginIndexs[nIndex], ppnNextSameSubStrBeginIndexs[nIndex] + m_nMainStrLen, -1);
		}

		ppnExpendCharacterCounts = new int*[m_nMainStrLen];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnExpendCharacterCounts[nIndex] = new int[MAX_CHARACTER_DOUBLECOUNT];
			std::fill(ppnExpendCharacterCounts[nIndex], ppnExpendCharacterCounts[nIndex] + MAX_CHARACTER_DOUBLECOUNT, 0);

		}

		pnRetSection = new int[m_nMainStrLen + 1];

	}
	catch (...)
	{
		if (ppnFrequentSubStrBeginIndexs != nullptr) 
		{
			for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
			{
				delete[] ppnFrequentSubStrBeginIndexs[nIndex];
				ppnFrequentSubStrBeginIndexs[nIndex] = nullptr;
			}
			delete[] ppnFrequentSubStrBeginIndexs;
			ppnFrequentSubStrBeginIndexs = nullptr;
		}
		if (ppnNextSameSubStrBeginIndexs != nullptr)
		{
			for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
			{
				delete[] ppnNextSameSubStrBeginIndexs[nIndex];
				ppnNextSameSubStrBeginIndexs[nIndex] = nullptr;
			}
			delete[] ppnNextSameSubStrBeginIndexs;
			ppnNextSameSubStrBeginIndexs = nullptr;
		}
		if (ppnExpendCharacterCounts != nullptr)
		{
			for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
			{
				delete[] ppnExpendCharacterCounts[nIndex];
				ppnExpendCharacterCounts[nIndex] = nullptr;
			}
			delete[] ppnExpendCharacterCounts;
			ppnExpendCharacterCounts = nullptr;
		}
		if (pnRetSection != nullptr)
		{
			delete[] pnRetSection;
			pnRetSection = nullptr;
		}
		
		return -1;
	}
	int nCurrentSubStrLen = 1;
	int nMaxCharacterCount = 0;
	for (int nIndex = 0; nIndex < m_nMainStrLen; nIndex++)
	{
		int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nIndex], false); // 长度为1时默认用右边
		ppnNextSameSubStrBeginIndexs[nCurrentSubStrLen][nIndex] = ppnFrequentSubStrBeginIndexs[nCurrentSubStrLen][nCurrCharacter];
		ppnFrequentSubStrBeginIndexs[nCurrentSubStrLen][nCurrCharacter] = nIndex;
		ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter]++;
		if (ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter] > nMaxCharacterCount)
		{
			nMaxCharacterCount = ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter];
			pnRetSection[nCurrentSubStrLen] = nIndex;
		}
	}

	// 当前迭代到的最频繁子串的长度，初始为单个字符，即1
	int64_t nMaxWeight = CalcWeight(nMaxCharacterCount, 1, _dFrequencyPow, _dLengthPow);

	Recur(ppnFrequentSubStrBeginIndexs, ppnNextSameSubStrBeginIndexs, ppnExpendCharacterCounts, pnRetSection, nMaxCharacterCount, nCurrentSubStrLen);

	for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
	{
		for (int i = pnRetSection[nIndex]; i < pnRetSection[nIndex] + nIndex; i++)
		{
			std::cout << m_pucMain[i];
		}
		std::cout << std::endl;
	}


	return 0;
}

int CFindMostFrequentSubstring::Recur(int** _ppnFrequentSubStrBeginIndexs, int** _ppnNextSameSubStrBeginIndexs, int** _ppnExpendCharacterCounts, int* _pnRetSection, int _nMaxCharacterCount, int _nCurrentSubStrLen)
{
	if (_nCurrentSubStrLen > m_nMainStrLen) return 0;
	else if (_nCurrentSubStrLen == m_nMainStrLen)
	{
		_pnRetSection[_nCurrentSubStrLen] = 0;
		return 0;
	}

	bool pnVisited[MAX_CHARACTER_DOUBLECOUNT];
	memset(pnVisited, false, sizeof(bool) * MAX_CHARACTER_DOUBLECOUNT);

	for (int nHead = 0; nHead < MAX_CHARACTER_DOUBLECOUNT; nHead++)	// 遍历所有子串链表的头指针
	{
		if (pnVisited[nHead] || _ppnExpendCharacterCounts[_nCurrentSubStrLen][nHead] < _nMaxCharacterCount) continue;

		pnVisited[nHead] = true;

	
		int nNextSubStrLen = _nCurrentSubStrLen + 1;

		std::fill(_ppnFrequentSubStrBeginIndexs[nNextSubStrLen], _ppnFrequentSubStrBeginIndexs[nNextSubStrLen] + MAX_CHARACTER_DOUBLECOUNT, -1);
		std::fill(_ppnNextSameSubStrBeginIndexs[nNextSubStrLen], _ppnNextSameSubStrBeginIndexs[nNextSubStrLen] + m_nMainStrLen, -1);
		std::fill(_ppnExpendCharacterCounts[nNextSubStrLen], _ppnExpendCharacterCounts[nNextSubStrLen] + MAX_CHARACTER_DOUBLECOUNT, 0);

		int nMaxCharacterCount = 0;

		int nSubStrBeginIndex;
		int nSubStrEndIndex;
		if (nHead >= MAX_CHARACTER_COUNT)
		{
			nSubStrEndIndex = _ppnFrequentSubStrBeginIndexs[_nCurrentSubStrLen][nHead];
			nSubStrBeginIndex = nSubStrEndIndex - _nCurrentSubStrLen + 1;
		}
		else
		{
			nSubStrBeginIndex = _ppnFrequentSubStrBeginIndexs[_nCurrentSubStrLen][nHead];
			nSubStrEndIndex = nSubStrBeginIndex + _nCurrentSubStrLen - 1;
		}

		while (true)
		{			
			// 向左扩一个字符
			if (nSubStrBeginIndex - 1 >= 0)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nSubStrBeginIndex - 1], true);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nSubStrBeginIndex - 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nSubStrBeginIndex - 1;  // 向左扩展时存储子串的开始位置
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex - 1;
				}
			}

			// 向右扩一个字符
			if (nSubStrEndIndex + 1 <= m_nMainStrLen - 1)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nSubStrEndIndex + 1], false);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nSubStrEndIndex + 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nSubStrEndIndex + 1; // 向右扩展时存储子串的结束位置
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex;
				}
			}

			if (nHead >= MAX_CHARACTER_COUNT)
			{
				nSubStrEndIndex = _ppnNextSameSubStrBeginIndexs[_nCurrentSubStrLen][nSubStrEndIndex];
				if (nSubStrEndIndex < 0) break;
				nSubStrBeginIndex = nSubStrEndIndex - _nCurrentSubStrLen + 1;
			}
			else
			{
				nSubStrBeginIndex = _ppnNextSameSubStrBeginIndexs[_nCurrentSubStrLen][nSubStrBeginIndex];
				if (nSubStrBeginIndex < 0) break;
				nSubStrEndIndex = nSubStrBeginIndex + _nCurrentSubStrLen - 1;
			}
		}

		Recur(_ppnFrequentSubStrBeginIndexs, _ppnNextSameSubStrBeginIndexs, _ppnExpendCharacterCounts, _pnRetSection, nMaxCharacterCount, nNextSubStrLen);

	}
}

int CFindMostFrequentSubstring::GetSubString1(double _dFrequencyPow, double _dLengthPow, unsigned char** _ppucRet, int& _nRetLen)
{
	if (_ppucRet == nullptr)
	{
		return -1;
	}
	if (m_pucMain == nullptr)
	{
		return 0;
	}

	int pnCharacterCount[MAX_CHARACTER_COUNT]; // 记录字符的数量
	memset(pnCharacterCount, 0, sizeof(pnCharacterCount));
	for (int nCharacterIndex = 0; nCharacterIndex < m_nMainStrLen; ++nCharacterIndex)
	{
		pnCharacterCount[m_pucMain[nCharacterIndex]]++;
	}

	// 存储字符串中每个字符的位置。此处若字符串很长，则为了节省内存，使用了不等长的二维数组。若不需要考虑内存，则可以改为链表数组减少一次遍历
	int** ppnCharacterPosition = nullptr;
	// 存储当前迭代刀的长度的子串的开始下标
	int* pnCurrSubStringBeginIndex = nullptr;
	// pnRetSection[i]表示长度为i的最频繁子串，其中一个的开始下标
	int* pnRetSection = nullptr; 
	try
	{
		pnCurrSubStringBeginIndex = new int[m_nMainStrLen]; // 当前最频繁子串的开始位置。向左扩展为负数，向右扩展为正数。子串长度为1时默认为正数
		pnRetSection = new int[m_nMainStrLen];
		ppnCharacterPosition = new int*[MAX_CHARACTER_COUNT];
		for (int nCharacterIndex = 0; nCharacterIndex < MAX_CHARACTER_COUNT; nCharacterIndex++)
		{
			// ppnCharacterPosition[nCharacterIndex]第0个位置存储该字符的数量，后续存储该字符的下标
			ppnCharacterPosition[nCharacterIndex] = new int[pnCharacterCount[nCharacterIndex] + 1];
			memset(ppnCharacterPosition[nCharacterIndex], 0, sizeof(int) * (pnCharacterCount[nCharacterIndex] + 1));
		}
	}
	catch (...)
	{
		if (pnRetSection != nullptr)
		{
			delete[] pnRetSection;
		}
		if (pnCurrSubStringBeginIndex != nullptr)
		{
			delete[] pnCurrSubStringBeginIndex;
		}
		if (ppnCharacterPosition != nullptr)
		{
			for (int nCharacterIndex = 0; nCharacterIndex < MAX_CHARACTER_COUNT; nCharacterIndex++)
			{
				if (ppnCharacterPosition[nCharacterIndex] != nullptr)
				{
					delete[] ppnCharacterPosition[nCharacterIndex];
				}
			}
			delete[] ppnCharacterPosition;
		}
		return -1;
	}

	int nCurrentSubStringLen = 1;
	int nCurrentSubStrCount = 0;
	int nMaxAppearCount = -1; // 当前最频繁子串的出现次数

	// 初始化哈希数组，存储长度为1的子串（即单个字符）的数量和所有下标
	for (int nIndex = 0; nIndex < m_nMainStrLen; nIndex++)
	{
		unsigned char ucCurrCharacter = m_pucMain[nIndex];
		ppnCharacterPosition[ucCurrCharacter][ppnCharacterPosition[ucCurrCharacter][0]++] = nIndex;
	}

	for (int nIndex = 0; nIndex < MAX_CHARACTER_COUNT; nIndex++)
	{
		unsigned char ucCurrCharacter = m_pucMain[nIndex];
		nMaxAppearCount = std::max(ppnCharacterPosition[ucCurrCharacter][0], nMaxAppearCount);
	}
	// 存储最频繁子串的开始下标
	memset(pnCurrSubStringBeginIndex, 0, sizeof(int) * m_nMainStrLen);

	while (nCurrentSubStringLen <= m_nMainStrLen)
	{
		for (int nIndex = 0; nIndex < MAX_CHARACTER_COUNT; nIndex++)
		{
			
		}
	}


	return 0;
}



double CFindMostFrequentSubstring::CalcWeight(int _nRepeatCount, int _nLength, double _dFrequencyPow, double _dLengthPow)
{
	return pow(_nRepeatCount, _dFrequencyPow) * pow(_nLength, _dLengthPow);
}