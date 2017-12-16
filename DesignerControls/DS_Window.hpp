#pragma once
#ifndef __DS_WINDOW_HPP_2015_11_11
#define __DS_WINDOW_HPP_2015_11_11

#include <map>

class CDS_Window : public DuiLib::CContainerUI, public CControlPlugin_ContainerUI, public IWindowSharedData
{
public:
	CDS_Window()
	{}
	LPCTSTR					GetClass() const
	{
		return _T("Window");
	}
	LPVOID					GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(_T("Window"), pstrName) == 0)
			return this;
		else if (_tcscmp(DUI_CTR_CONTROL_PLUGIN_BASE, pstrName) == 0)
			return static_cast<IControlPlugin*>(this);
		else if (_tcscmp(DUI_CTR_WINDOW_SHARED_DATA, pstrName) == 0)
			return static_cast<IWindowSharedData*>(this);
		return DuiLib::CContainerUI::GetInterface(pstrName);
	}
	void					DoInit()
	{
		__m_mapAttr[_T("opacity")] = _T("255");
		__m_mapAttr[_T("layeredopacity")] = _T("255");
	}
	void					SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(_T("size"), pstrName) == 0)
		{
			SIZE cxyFixed = { 0 };
			LPTSTR pstr = NULL;
			cxyFixed.cx = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);    
			cxyFixed.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			RECT rcPos = GetPos();
			int nWidth = rcPos.right - rcPos.left;
			int nHeight = rcPos.bottom - rcPos.top;
			if (cxyFixed.cx != nWidth || cxyFixed.cy != nHeight) 
			{
				rcPos.right = rcPos.left + cxyFixed.cx;
				rcPos.bottom = rcPos.top + cxyFixed.cy;
				SetFixedWidth(cxyFixed.cx);
				SetFixedHeight(cxyFixed.cy);
				NeedUpdate();
			}
		}
		else if (_tcscmp(_T("sizebox"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("caption"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("roundcorner"), pstrName) == 0)
		{
			SIZE cxyRound = { 0 };
			LPTSTR pstr = NULL;
			cxyRound.cx = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);    
			cxyRound.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			if (m_cxyBorderRound.cx != cxyRound.cx || m_cxyBorderRound.cy != cxyRound.cy) SetBorderRound(cxyRound);
		}
		else if (_tcscmp(_T("mininfo"), pstrName) == 0)
		{
			SIZE cxyInfo = { 0 };
			LPTSTR pstr = NULL;
			cxyInfo.cx = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);    
			cxyInfo.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			if (cxyInfo.cx != GetMinWidth()) SetMinWidth(cxyInfo.cx);
			if (cxyInfo.cy != GetMinHeight()) SetMinHeight(cxyInfo.cy);
		}
		else if (_tcscmp(_T("maxinfo"), pstrName) == 0)
		{
			SIZE cxyInfo = { 0 };
			LPTSTR pstr = NULL;
			cxyInfo.cx = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);    
			cxyInfo.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			if (cxyInfo.cx != GetMaxWidth()) SetMaxWidth(cxyInfo.cx);
			if (cxyInfo.cy != GetMaxHeight()) SetMaxHeight(cxyInfo.cy);
		}
		else if (_tcscmp(_T("opacity"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("layeredopacity"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("layeredimage"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("mininfo"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("disabledfontcolor"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("defaultfontcolor"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("linkfontcolor"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("linkhoverfontcolor"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("selectedcolor"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("showdirty"), pstrName) == 0)
			__m_mapAttr[pstrName] = pstrValue;
		else if (_tcscmp(_T("resousepath"), pstrName) == 0)
			__m_strResourcePath = pstrValue;
		else if (_tcscmp(_T("fontid"), pstrName) == 0)
			__m_strFontID = pstrValue;
		else if (_tcscmp(_T("defaultfont"), pstrName) == 0)
			__m_strDefaultFontID = pstrValue;
		else
			DuiLib::CContainerUI::SetAttribute(pstrName, pstrValue);
	}
	DuiLib::CDuiString		GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CDuiString str;
		if (_tcscmp(_T("size"), pstrName) == 0)
		{
			str.Format(_T("%d,%d"), m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
			return str;
		}
		else if (_tcscmp(_T("maxinfo"), pstrName) == 0)
		{
			str.Format(_T("%d,%d"), GetMaxWidth(), GetMaxHeight());
			return str;
		}
		else if (_tcscmp(_T("mininfo"), pstrName) == 0)
		{
			str.Format(_T("%d,%d"), GetMinWidth(), GetMinHeight());
			return str;
		}
		else if (_tcscmp(_T("roundcorner"), pstrName) == 0)
		{
			str.Format(_T("%d,%d"), m_cxyBorderRound.cx, m_cxyBorderRound.cy);
			return str;
		}
		else if (_tcscmp(_T("resousepath"), pstrName) == 0)
			return __m_strResourcePath;
		else if (_tcscmp(_T("fontid"), pstrName) == 0)
			return __m_strFontID;
		else if (_tcscmp(_T("defaultfont"), pstrName) == 0)
			return __m_strDefaultFontID;
		auto it = __m_mapAttr.find(pstrName);
		if (it == __m_mapAttr.end()) return str;
		return it->second;
	}
private:
	std::map<DuiLib::CDuiString, DuiLib::CDuiString>	__m_mapAttr;
	DuiLib::CDuiString									__m_strResourcePath;
	DuiLib::CDuiString									__m_strFontID;
	DuiLib::CDuiString									__m_strDefaultFontID;
};

#endif