#include<iostream>
#include "CFindMostFrequentSubstring.h"

// 向左扩展的字符用[0, MAX_CHARACTER_COUNT - 1]存储，向右扩展的字符用[MAX_CHARACTER_COUNT, 2 * MAX_CHARACTER_COUNT - 1]存储
#define GET_CHARACTER_DOUBLESIDE_HASH(_Character, _bLeft) (_bLeft ? _Character : _Character + MAX_CHARACTER_COUNT)
#define max std::max

CFindMostFrequentSubstring::CFindMostFrequentSubstring()
{
	m_pucMain = nullptr;
}

CFindMostFrequentSubstring::CFindMostFrequentSubstring(unsigned char* _pucMain, int _nMainStrLen, int _nFirstLayerSaveRank)
{
	m_nFirstLayerSaveRank = _nFirstLayerSaveRank;
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
	// 长度为i的子串的最多出现次数
	int* pnSubStrAppearCount = nullptr;
	// 结果集。ppnRetSection[i]的数组存储长度为i的最频繁子串的开始下标，其中ppnRetSection[i][0]存储结果个数，后续依次存储下标
	int** ppnRetSection = nullptr;


	// ppnFrequentSubStrBeginIndexs和ppnNextSameSubStrBeginIndexs已用的下标
	int nUsageIndex = -1;

	try
	{
		ppnFrequentSubStrBeginIndexs = new int* [m_nMainStrLen + 1];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnFrequentSubStrBeginIndexs[nIndex] = new int[MAX_CHARACTER_DOUBLECOUNT];
			std::fill(ppnFrequentSubStrBeginIndexs[nIndex], ppnFrequentSubStrBeginIndexs[nIndex] + MAX_CHARACTER_DOUBLECOUNT, -1);

		}

		ppnNextSameSubStrBeginIndexs = new int* [m_nMainStrLen + 1];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnNextSameSubStrBeginIndexs[nIndex] = new int[2 * m_nMainStrLen];
			std::fill(ppnNextSameSubStrBeginIndexs[nIndex], ppnNextSameSubStrBeginIndexs[nIndex] + 2 * m_nMainStrLen, -1);
		}

		ppnExpendCharacterCounts = new int* [m_nMainStrLen + 1];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			ppnExpendCharacterCounts[nIndex] = new int[MAX_CHARACTER_DOUBLECOUNT];
			std::fill(ppnExpendCharacterCounts[nIndex], ppnExpendCharacterCounts[nIndex] + MAX_CHARACTER_DOUBLECOUNT, 0);

		}

		pnSubStrAppearCount = new int[m_nMainStrLen + 1];
		memset(pnSubStrAppearCount, 0, sizeof(int) * (m_nMainStrLen + 1));

		ppnRetSection = new int* [m_nMainStrLen + 1];
		for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
		{
			// 长度为len的子串的结果，额外开辟一个，下标为0的元素存储结果个数
			ppnRetSection[nIndex] = new int[m_nMainStrLen + 1];
			memset(ppnRetSection[nIndex], 0, sizeof(int) * (m_nMainStrLen + 1));
		}

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
		if (pnSubStrAppearCount != nullptr)
		{
			delete[] pnSubStrAppearCount;
			pnSubStrAppearCount = nullptr;
		}
		if (ppnRetSection != nullptr)
		{
			for (int nIndex = 1; nIndex <= m_nMainStrLen; nIndex++)
			{
				delete ppnRetSection[nIndex];
				ppnRetSection[nIndex] = nullptr;
			}
			delete[] ppnRetSection;
			ppnRetSection = nullptr;
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
		}
	}

	// 当前迭代到的最频繁子串的长度，初始为单个字符，即1
	int64_t nMaxWeight = CalcWeight(nMaxCharacterCount, 1, _dFrequencyPow, _dLengthPow);

	Recur(ppnFrequentSubStrBeginIndexs, ppnNextSameSubStrBeginIndexs, ppnExpendCharacterCounts, pnSubStrAppearCount, ppnRetSection, nMaxCharacterCount, nCurrentSubStrLen);

	for (int nLen = 1; nLen <= m_nMainStrLen; nLen++)
	{
		std::cout << "长度为" << nLen << std::endl;
		for (int nRetIndex = 1; nRetIndex <= ppnRetSection[nLen][0]; nRetIndex++)
		{
			std::cout << "开始下标 " << ppnRetSection[nLen][nRetIndex] << " ";
			std::cout << "出现次数 " << pnSubStrAppearCount[nLen] << " ";
			for (int i = ppnRetSection[nLen][nRetIndex]; i < ppnRetSection[nLen][nRetIndex] + nLen; i++)
			{
				std::cout << m_pucMain[i];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}


	return 0;
}


int CFindMostFrequentSubstring::Recur(int** _ppnFrequentSubStrBeginIndexs, int** _ppnNextSameSubStrBeginIndexs, int** _ppnExpendCharacterCounts, int* _pnSubStrAppearCount, int** _ppnRetSection, int _nMaxCharacterCount, int _nCurrentSubStrLen)
{
	if (_nCurrentSubStrLen > m_nMainStrLen) return 0;
	else if (_nCurrentSubStrLen == m_nMainStrLen)
	{
		_ppnRetSection[_nCurrentSubStrLen][1] = 0;
		_pnSubStrAppearCount[_nCurrentSubStrLen] = 1;
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
		nSubStrBeginIndex = _ppnFrequentSubStrBeginIndexs[_nCurrentSubStrLen][nHead];

		// 排掉相同的结果子串。例如ab和bc的数量相同，则ab向右扩成abc，bc向左扩成abc，这两个是同一个串，不能重复搜索
		bool bRepeat = false;
		for (int nRetIndex = 1; nRetIndex <= _ppnRetSection[_nCurrentSubStrLen][0]; nRetIndex++)
		{
			unsigned char ucRetBeginChar = m_pucMain[_ppnRetSection[_nCurrentSubStrLen][nRetIndex]];
			if (ucRetBeginChar == m_pucMain[nSubStrBeginIndex % m_nMainStrLen])
			{
				bRepeat = true;
			}
		}
		if (bRepeat) continue;

		if (_nMaxCharacterCount > _pnSubStrAppearCount[_nCurrentSubStrLen])
		{
			_pnSubStrAppearCount[_nCurrentSubStrLen] = _nMaxCharacterCount;
			_ppnRetSection[_nCurrentSubStrLen][0] = 0;
			_ppnRetSection[_nCurrentSubStrLen][++(_ppnRetSection[_nCurrentSubStrLen][0])] = (nSubStrBeginIndex % m_nMainStrLen);
		}
		else if (_nMaxCharacterCount == _pnSubStrAppearCount[_nCurrentSubStrLen])
		{
			_ppnRetSection[_nCurrentSubStrLen][++(_ppnRetSection[_nCurrentSubStrLen][0])] = (nSubStrBeginIndex % m_nMainStrLen);
		}
		if (_nMaxCharacterCount == 1) continue;


		while (true)
		{
			int nRealBeginIndex = nSubStrBeginIndex % m_nMainStrLen;
			int nRealEndIndex = nRealBeginIndex + _nCurrentSubStrLen - 1;

			// 向左扩一个字符
			if (nSubStrBeginIndex - 1 >= 0)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nRealBeginIndex - 1], true);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nRealBeginIndex - 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nRealBeginIndex - 1;  // 向左扩展时存储子串的开始位置
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex - 1;
				}
			}

			// 向右扩一个字符
			if (nRealEndIndex + 1 <= m_nMainStrLen - 1)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nRealEndIndex + 1], false);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nRealBeginIndex + m_nMainStrLen] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nRealBeginIndex + m_nMainStrLen; // 向右扩展时，将开始下标加上母串长度，方便和向左扩展的排开
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex;
				}
			}

			nSubStrBeginIndex = _ppnNextSameSubStrBeginIndexs[_nCurrentSubStrLen][nSubStrBeginIndex];
			if (nSubStrBeginIndex < 0) break;
		}

		Recur(_ppnFrequentSubStrBeginIndexs, _ppnNextSameSubStrBeginIndexs, _ppnExpendCharacterCounts, _pnSubStrAppearCount, _ppnRetSection, nMaxCharacterCount, nNextSubStrLen);

	}
}


/*
int CFindMostFrequentSubstring::Recur(int** _ppnFrequentSubStrBeginIndexs, int** _ppnNextSameSubStrBeginIndexs, int** _ppnExpendCharacterCounts, int* _pnSubStrAppearCount, int** _ppnRetSection, int _nMaxCharacterCount, int _nCurrentSubStrLen)
{
	if (_nCurrentSubStrLen > m_nMainStrLen) return 0;
	else if (_nCurrentSubStrLen == m_nMainStrLen)
	{
		_ppnRetSection[_nCurrentSubStrLen][1] = 0;
		_pnSubStrAppearCount[_nCurrentSubStrLen] = 1;
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

		if (_nMaxCharacterCount > _pnSubStrAppearCount[_nCurrentSubStrLen])
		{
			_pnSubStrAppearCount[_nCurrentSubStrLen] = _nMaxCharacterCount;
			_ppnRetSection[_nCurrentSubStrLen][0] = 0;
			_ppnRetSection[_nCurrentSubStrLen][++(_ppnRetSection[_nCurrentSubStrLen][0])] = nSubStrBeginIndex;
		}
		else if (_nMaxCharacterCount == _pnSubStrAppearCount[_nCurrentSubStrLen])
		{
			_ppnRetSection[_nCurrentSubStrLen][++(_ppnRetSection[_nCurrentSubStrLen][0])] = nSubStrBeginIndex;
		}
		if (_nMaxCharacterCount == 1) continue;
		

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
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex - 1;
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
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex;
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

		Recur(_ppnFrequentSubStrBeginIndexs, _ppnNextSameSubStrBeginIndexs, _ppnExpendCharacterCounts, _pnSubStrAppearCount, _ppnRetSection, nMaxCharacterCount, nNextSubStrLen);

	}
}
*/
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
		ppnCharacterPosition = new int* [MAX_CHARACTER_COUNT];
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
		nMaxAppearCount = max(ppnCharacterPosition[ucCurrCharacter][0], nMaxAppearCount);
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

inline int CFindMostFrequentSubstring::GetLayerSaveRank(int _nLayer)
{
	return max(m_nFirstLayerSaveRank - _nLayer + 1, 1);
}