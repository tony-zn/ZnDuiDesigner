#pragma once
#ifndef __MEMORANDUM_H_2015_12_09
#define __MEMORANDUM_H_2015_12_09

template<typename T>
class CMemorandum
{
public:
	typedef T					value_type;
public:
	CMemorandum(UINT _unMax = 100);
	void Add(const value_type& _data);
	bool Get(value_type& _data);
	void MoveNext();
	void MovePre();
	UINT GetCurPos();
	UINT GetSize();
private:
	std::vector<value_type>		__m_vecMemorandum;		// 备忘录容器
	UINT						__m_unPos;				// 当前记录的偏移量
	const UINT					__m_unMax;				// 备记录最大记录个数
};

class CMemorandumControl
{
public:
	typedef _ZnString			value_type;
public:
	static CMemorandumControl&	Instance(){static CMemorandumControl mem; return mem;}
	void						Add(const int _nID, const value_type& _strData);
	bool						Get(const int _nID, value_type& _strData);
	void						MoveNext(const int _nID);
	void						MovePre(const int _nID);
	UINT						GetCurPos(const int _nID);
	UINT						GetSize(const int _nID);
	void						Delete(const int _nID);
private:
	CMemorandumControl(){}
	CMemorandumControl(const CMemorandumControl&){}
private:
	std::map<int, CMemorandum<value_type> >	__m_mapMem;
};

#define memorandum CMemorandumControl::Instance()

#endif