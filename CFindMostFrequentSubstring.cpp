#include<iostream>
#include "CFindMostFrequentSubstring.h"

// ������չ���ַ���[0, MAX_CHARACTER_COUNT - 1]�洢��������չ���ַ���[MAX_CHARACTER_COUNT, 2 * MAX_CHARACTER_COUNT - 1]�洢
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


	/* ��Ҫ�����ݽṹ
	* ���飬�洢��ǰ����������Ƶ���Ӵ��Ŀ�ʼ�±�
	* ���飬arr[i]��ʾ��m_pucMain[i]��ʼ���Ӵ�����һ����ͬ�Ӵ��Ŀ�ʼ�±�
	* ����һά���飬�ֱ�洢��ǰ�Ӵ���������ҵ�һ���ַ�������������Ѱ�ҳ���+1����Ƶ���Ӵ���
	*/

	// �洢��ͬ�Ӵ������ͷָ��
	int** ppnFrequentSubStrBeginIndexs = nullptr;
	// ��һ����ͬ�Ӵ��Ŀ�ʼ�±ꡣ��û�У�����-1
	int** ppnNextSameSubStrBeginIndexs = nullptr;
	// �洢����ʱ�Ӵ���չ��ȥ���ַ�����
	int** ppnExpendCharacterCounts = nullptr;
	// ����Ϊi���Ӵ��������ִ���
	int* pnSubStrAppearCount = nullptr;
	// �������ppnRetSection[i]������洢����Ϊi����Ƶ���Ӵ��Ŀ�ʼ�±꣬����ppnRetSection[i][0]�洢����������������δ洢�±�
	int** ppnRetSection = nullptr;


	// ppnFrequentSubStrBeginIndexs��ppnNextSameSubStrBeginIndexs���õ��±�
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
			// ����Ϊlen���Ӵ��Ľ�������⿪��һ�����±�Ϊ0��Ԫ�ش洢�������
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
		int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nIndex], false); // ����Ϊ1ʱĬ�����ұ�
		ppnNextSameSubStrBeginIndexs[nCurrentSubStrLen][nIndex] = ppnFrequentSubStrBeginIndexs[nCurrentSubStrLen][nCurrCharacter];
		ppnFrequentSubStrBeginIndexs[nCurrentSubStrLen][nCurrCharacter] = nIndex;
		ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter]++;
		if (ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter] > nMaxCharacterCount)
		{
			nMaxCharacterCount = ppnExpendCharacterCounts[nCurrentSubStrLen][nCurrCharacter];
		}
	}

	// ��ǰ����������Ƶ���Ӵ��ĳ��ȣ���ʼΪ�����ַ�����1
	int64_t nMaxWeight = CalcWeight(nMaxCharacterCount, 1, _dFrequencyPow, _dLengthPow);

	Recur(ppnFrequentSubStrBeginIndexs, ppnNextSameSubStrBeginIndexs, ppnExpendCharacterCounts, pnSubStrAppearCount, ppnRetSection, nMaxCharacterCount, nCurrentSubStrLen);

	for (int nLen = 1; nLen <= m_nMainStrLen; nLen++)
	{
		std::cout << "����Ϊ" << nLen << std::endl;
		for (int nRetIndex = 1; nRetIndex <= ppnRetSection[nLen][0]; nRetIndex++)
		{
			std::cout << "��ʼ�±� " << ppnRetSection[nLen][nRetIndex] << " ";
			std::cout << "���ִ��� " << pnSubStrAppearCount[nLen] << " ";
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

	for (int nHead = 0; nHead < MAX_CHARACTER_DOUBLECOUNT; nHead++)	// ���������Ӵ������ͷָ��
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

		// �ŵ���ͬ�Ľ���Ӵ�������ab��bc��������ͬ����ab��������abc��bc��������abc����������ͬһ�����������ظ�����
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

			// ������һ���ַ�
			if (nSubStrBeginIndex - 1 >= 0)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nRealBeginIndex - 1], true);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nRealBeginIndex - 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nRealBeginIndex - 1;  // ������չʱ�洢�Ӵ��Ŀ�ʼλ��
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex - 1;
				}
			}

			// ������һ���ַ�
			if (nRealEndIndex + 1 <= m_nMainStrLen - 1)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nRealEndIndex + 1], false);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nRealBeginIndex + m_nMainStrLen] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nRealBeginIndex + m_nMainStrLen; // ������չʱ������ʼ�±����ĸ�����ȣ������������չ���ſ�
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

	for (int nHead = 0; nHead < MAX_CHARACTER_DOUBLECOUNT; nHead++)	// ���������Ӵ������ͷָ��
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
			// ������һ���ַ�
			if (nSubStrBeginIndex - 1 >= 0)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nSubStrBeginIndex - 1], true);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nSubStrBeginIndex - 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nSubStrBeginIndex - 1;  // ������չʱ�洢�Ӵ��Ŀ�ʼλ��
				_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter]++;
				if (_ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter] > nMaxCharacterCount)
				{
					nMaxCharacterCount = _ppnExpendCharacterCounts[nNextSubStrLen][nCurrCharacter];
					//_pnRetSection[nNextSubStrLen] = nSubStrBeginIndex - 1;
				}
			}

			// ������һ���ַ�
			if (nSubStrEndIndex + 1 <= m_nMainStrLen - 1)
			{
				int nCurrCharacter = GET_CHARACTER_DOUBLESIDE_HASH(m_pucMain[nSubStrEndIndex + 1], false);
				_ppnNextSameSubStrBeginIndexs[nNextSubStrLen][nSubStrEndIndex + 1] = _ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter];
				_ppnFrequentSubStrBeginIndexs[nNextSubStrLen][nCurrCharacter] = nSubStrEndIndex + 1; // ������չʱ�洢�Ӵ��Ľ���λ��
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

	int pnCharacterCount[MAX_CHARACTER_COUNT]; // ��¼�ַ�������
	memset(pnCharacterCount, 0, sizeof(pnCharacterCount));
	for (int nCharacterIndex = 0; nCharacterIndex < m_nMainStrLen; ++nCharacterIndex)
	{
		pnCharacterCount[m_pucMain[nCharacterIndex]]++;
	}

	// �洢�ַ�����ÿ���ַ���λ�á��˴����ַ����ܳ�����Ϊ�˽�ʡ�ڴ棬ʹ���˲��ȳ��Ķ�ά���顣������Ҫ�����ڴ棬����Ը�Ϊ�����������һ�α���
	int** ppnCharacterPosition = nullptr;
	// �洢��ǰ�������ĳ��ȵ��Ӵ��Ŀ�ʼ�±�
	int* pnCurrSubStringBeginIndex = nullptr;
	// pnRetSection[i]��ʾ����Ϊi����Ƶ���Ӵ�������һ���Ŀ�ʼ�±�
	int* pnRetSection = nullptr;
	try
	{
		pnCurrSubStringBeginIndex = new int[m_nMainStrLen]; // ��ǰ��Ƶ���Ӵ��Ŀ�ʼλ�á�������չΪ������������չΪ�������Ӵ�����Ϊ1ʱĬ��Ϊ����
		pnRetSection = new int[m_nMainStrLen];
		ppnCharacterPosition = new int* [MAX_CHARACTER_COUNT];
		for (int nCharacterIndex = 0; nCharacterIndex < MAX_CHARACTER_COUNT; nCharacterIndex++)
		{
			// ppnCharacterPosition[nCharacterIndex]��0��λ�ô洢���ַ��������������洢���ַ����±�
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
	int nMaxAppearCount = -1; // ��ǰ��Ƶ���Ӵ��ĳ��ִ���

	// ��ʼ����ϣ���飬�洢����Ϊ1���Ӵ����������ַ����������������±�
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
	// �洢��Ƶ���Ӵ��Ŀ�ʼ�±�
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