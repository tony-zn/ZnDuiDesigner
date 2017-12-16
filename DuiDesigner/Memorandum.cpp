#include "StdAfx.h"
#include "Memorandum.h"

 template<typename T>
 CMemorandum<T>::CMemorandum( UINT _unMax /*= 100*/ ) : __m_unPos(0)
 	, __m_unMax(_unMax)
 {
 
 }

template<typename T>
void CMemorandum<T>::Add( const value_type& _data )
{
	if (!__m_vecMemorandum.empty() && __m_unPos < __m_vecMemorandum.size() - 1)
		__m_vecMemorandum.erase(__m_vecMemorandum.begin() + __m_unPos + 1, __m_vecMemorandum.end());
	if (!__m_vecMemorandum.empty() && __m_vecMemorandum.size() > __m_unMax)
		__m_vecMemorandum.erase(__m_vecMemorandum.begin(), __m_vecMemorandum.begin() + (__m_vecMemorandum.size() - __m_unMax));
	__m_vecMemorandum.push_back(_data);
	__m_unPos = __m_vecMemorandum.size() - 1;
}

template<typename T>
bool CMemorandum<T>::Get( value_type& _data )
{
	if (__m_unPos >= 0 && __m_unPos < __m_vecMemorandum.size())
	{
		_data = __m_vecMemorandum[__m_unPos];
		return true;
	}
	return false;
}

template<typename T>
void CMemorandum<T>::MoveNext()
{
	if (__m_vecMemorandum.empty()) return;
	if (__m_unPos < __m_vecMemorandum.size() - 1)
		++__m_unPos;
}

template<typename T>
void CMemorandum<T>::MovePre()
{
	if (__m_vecMemorandum.empty()) return;
	if (__m_unPos > 0)
		--__m_unPos;
}

template<typename T>
UINT CMemorandum<T>::GetCurPos()
{
	return __m_unPos;
}

template<typename T>
UINT CMemorandum<T>::GetSize()
{
	return __m_vecMemorandum.size();
}

void CMemorandumControl::Add( const int _nID, const value_type& _strData )
{
	__m_mapMem[_nID].Add(_strData);
}

bool CMemorandumControl::Get( const int _nID, value_type& _strData )
{
	return __m_mapMem[_nID].Get(_strData);
}

void CMemorandumControl::MoveNext( const int _nID )
{
	__m_mapMem[_nID].MoveNext();
}

void CMemorandumControl::MovePre( const int _nID )
{
	__m_mapMem[_nID].MovePre();
}

UINT CMemorandumControl::GetCurPos( const int _nID )
{
	return __m_mapMem[_nID].GetCurPos();
}

UINT CMemorandumControl::GetSize( const int _nID )
{
	return __m_mapMem[_nID].GetSize();
}

void CMemorandumControl::Delete( const int _nID )
{
	auto it = __m_mapMem.find(_nID);
	if (it != __m_mapMem.end()) __m_mapMem.erase(it);
}
