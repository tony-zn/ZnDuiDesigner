#pragma once
#ifndef __GLOBLE_HPP_2015_11_09
#define __GLOBLE_HPP_2015_11_09

#include <sstream>
#include <string>
#include <algorithm>

#ifdef _UNICODE
typedef std::wstringstream		_STREAM;
typedef std::wstring			_STRING;
#else
typedef std::stringstream		_STREAM;
typedef std::string				_STRING;
#endif

SIZE get_text_size(HDC _hDC, HFONT _hFont, LPCTSTR _lpStr)
{
	SIZE sz = {0};
	HFONT hOld = (HFONT)::SelectObject(_hDC, _hFont);
	::GetTextExtentPoint32(_hDC, _lpStr, _tcslen(_lpStr), &sz);
	::SelectObject(_hDC, hOld);
	return sz;
}


IControlPlugin* get_control_interface( DuiLib::CControlUI* _pControl )
{
	if (!_pControl) return nullptr;
	static CDefaultControlAttrib dca;
	IControlPlugin* pInterface = static_cast<IControlPlugin*>(_pControl->GetInterface(DUI_CTR_CONTROL_PLUGIN_BASE));
	if (!pInterface) 
	{
		dca.SetControl(_pControl);
		pInterface = &dca;
	}
	return pInterface;
}

DuiLib::CControlUI* get_parent(DuiLib::CControlUI* _pControl)
{
	if (!_pControl) return nullptr;
	DuiLib::CTreeNodeUI* pNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetInterface(DUI_CTR_TREENODE));
	if (pNode) 
	{
		DuiLib::CTreeNodeUI* pParentNode = pNode->GetParentNode();
		if (pParentNode)
			return pParentNode;
		else
			return pNode->GetTreeView();
	}
	return _pControl->GetParent();
}

DuiLib::CDuiString get_resource_path(DuiLib::CControlUI* _pControl)
{
	while( _tcscmp(_pControl->GetClass(), _T("Window")) != 0)
		_pControl = get_parent(_pControl);
	IControlPlugin* pInterface = get_control_interface(_pControl);
	if (pInterface)
		return pInterface->GetAttribute(_T("resousepath"));
	return DuiLib::CDuiString();
}

DuiLib::CControlUI* get_wnd_ptr(DuiLib::CControlUI* _pControl)
{
	while(_pControl &&  _tcscmp(_pControl->GetClass(), _T("Window")) != 0)
		_pControl = get_parent(_pControl);
	return _pControl;
}

DuiLib::CControlUI* get_root_ptr(DuiLib::CControlUI* _pControl)
{
	while(_pControl && _pControl->GetParent())
		_pControl = get_parent(_pControl);
	return _pControl;
}

IWindowSharedData* get_wnd_shared_data( DuiLib::CControlUI* _pControl )
{
	DuiLib::CControlUI* pWnd = get_wnd_ptr(_pControl);
	if (!pWnd) return nullptr;
	return static_cast<IWindowSharedData*>(pWnd->GetInterface(DUI_CTR_WINDOW_SHARED_DATA));
}

//Ã¶¾Ùº¯Êý
int CALLBACK EnumFontFamProc(LPENUMLOGFONT lpelf,LPNEWTEXTMETRIC lpntm,DWORD nFontType,long lparam)
{
	std::vector<DuiLib::CDuiString>* pVecFont = (std::vector<DuiLib::CDuiString>*)(lparam);
	if (std::find(pVecFont->begin(), pVecFont->end(), lpelf->elfLogFont.lfFaceName) == pVecFont->end())
		pVecFont->push_back(DuiLib::CDuiString(lpelf->elfLogFont.lfFaceName));
	return 1;
}

const std::vector<DuiLib::CDuiString>& get_sys_font_name()
{
	static std::vector<DuiLib::CDuiString> vecFont;
	if (vecFont.empty())
	{
		LOGFONT lf = {0};
		lf.lfCharSet = DEFAULT_CHARSET;
		HDC hDC = ::GetDC(NULL);
		::EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC) EnumFontFamProc, (LPARAM)&vecFont, 0);
		::ReleaseDC(NULL, hDC);
	}
	return vecFont;
}

#define zn_max(a,b)    (((a) > (b)) ? (a) : (b))
#define zn_min(a,b)    (((a) < (b)) ? (a) : (b))
#define zn_round(a)	   static_cast<int>((a) + 0.5)

void rgb_to_hsb(DWORD _dwRGB, double& _h, double& _s, double& _b, int& _a)
{
	_a = _dwRGB >> 24 & 0x000000FF;
	int R = _dwRGB >> 16 & 0x000000FF;
	int G = _dwRGB >> 8 & 0x000000FF;
	int B = _dwRGB & 0x000000FF;
	double dMax = zn_max(R, zn_max(G, B));
	double dMin = zn_min(R, zn_min(G, B));
	_b = dMax / 255.0;
	_s = dMax == 0 ? 0 : (dMax - dMin) / dMax;
	_h = 0;
	if (dMax == R && G >= B)
		_h = (G - B) * 60 / (dMax - dMin) + 0;  
	else if (dMax == R && G < B)
		_h = (G - B) * 60 / (dMax - dMin) + 360;  
	else if (dMax == G)   
		_h = (B - R) * 60 / (dMax - dMin) + 120;  
	else if (dMax == B)
		_h = (R - G) * 60 / (dMax - dMin) + 240;  
}

DWORD hsb_to_rgb(double _h, double _s, double _b, int _a)
{
	double R = 0.0, G = 0.0, B = 0.0;
	int i = static_cast<int>(_h / 60);
	double f = (_h / 60) - i;
	double p = _b * (1 - _s);
	double q = _b * (1 - f * _s);
	double t = _b * (1 - (1 - f) * _s);
	switch (i) 
	{  
	case 0:  
		R = _b;  
		G = t;  
		B = p;  
		break;  
	case 1:  
		R = q;  
		G = _b;  
		B = p;  
		break;  
	case 2:  
		R = p;  
		G = _b;  
		B = t;  
		break;  
	case 3:  
		R = p;  
		G = q;  
		B = _b;  
		break;  
	case 4:  
		R = t;  
		G = p;  
		B = _b;  
		break;  
	case 5:  
		R = _b;  
		G = p;  
		B = q;  
		break;  
	default: 
		break;  
	}
	int rgbR = zn_round(R * 255.0);
	int rgbG = zn_round(G * 255.0);
	int rgbB = zn_round(B * 255.0);
	return 0xFF000000 & _a << 24 | 0xFF0000 & rgbR << 16 | 0xFF00 & rgbG << 8 | 0xFF & rgbB;
}


#endif