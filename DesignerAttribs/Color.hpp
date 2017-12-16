#pragma once
#ifndef __COLOR_HPP_2015_12_01
#define __COLOR_HPP_2015_12_01

class CColorSetUI : public DuiLib::CControlUI
{
public:
	CColorSetUI() : __m_dHue(0.0), __m_hBitmap(NULL), __m_pBits(nullptr), __m_dwColor(0), __m_S(0), __m_B(0){}
	LPCTSTR		GetClass() const{ return _T("ColorSetUI");}
	LPVOID		GetInterface(LPCTSTR pstrName){ if (_tcscmp(pstrName, _T("ColorSet")) == 0) return this;	return __super::GetInterface(pstrName);	}
	void		SetHue(double _dHue)
	{
		if (fabs(_dHue - __m_dHue) > 0.001) 
		{
			__m_dHue = _dHue; 
			if (__m_dHue >= 360.0) __m_dHue = __m_dHue - 360.0;
			__ResetColor(); 
			NeedUpdate();
		}
	}
	void		SetSaturation(double _dSaturation)
	{
		if (fabs(_dSaturation - __m_S) > 0.001)
		{
			__m_S = _dSaturation;
			NeedUpdate();
		}
	}
	void		SetBrightness(double _dBrightness)
	{
		if (fabs(_dBrightness - __m_B) > 0.001)
		{
			__m_B = _dBrightness;
			NeedUpdate();
		}
	}
	void		SetColorPt(int x, int y)
	{
		if (__m_szBitmap.cx == 0 || __m_szBitmap.cy == 0) return;
		if (x < m_rcItem.left)
			x = 0;
		else if (x > m_rcItem.right)
			x = m_rcItem.right - m_rcItem.left;
		else
			x = x - m_rcItem.left;
		if (y < m_rcItem.top)
			y = 0;
		else if (y > m_rcItem.bottom)
			y = m_rcItem.bottom - m_rcItem.top;
		else
			y = y - m_rcItem.top;
		__m_S = static_cast<double>(x) / __m_szBitmap.cx;
		__m_B = static_cast<double>(y) / __m_szBitmap.cy;
		NeedUpdate();
	}
	void		DoPaint(HDC hDC, const RECT& rcPaint)
	{
		__super::DoPaint(hDC, rcPaint);
		__ResetBitmap();
		HDC hMemDc = ::CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDc, __m_hBitmap);
		// »­Ñ¡È¡µÄÔ²È¦
		{
			int x = static_cast<int>(__m_S * __m_szBitmap.cx);
			int y = static_cast<int>(__m_B * __m_szBitmap.cy);
			Gdiplus::Graphics graphic(hMemDc);
			graphic.SetPageUnit(Gdiplus::UnitPixel);
			graphic.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			double dB = 1 - __m_B;
			Gdiplus::Pen penW(Gdiplus::Color(0xFFFFFFFF), 1);
			Gdiplus::Pen penB(Gdiplus::Color(0xFF000000), 1);
			const int r = 5;
			RECT rcEllipse = {x - r, y - r, x + r, y + r};
			graphic.DrawEllipse(&penW, rcEllipse.left, rcEllipse.top, rcEllipse.right - rcEllipse.left, rcEllipse.bottom - rcEllipse.top);
			::InflateRect(&rcEllipse, -1, -1);
			graphic.DrawEllipse(&penB, rcEllipse.left, rcEllipse.top, rcEllipse.right - rcEllipse.left, rcEllipse.bottom - rcEllipse.top);
			::InflateRect(&rcEllipse, -1, -1);
			graphic.DrawEllipse(&penW, rcEllipse.left, rcEllipse.top, rcEllipse.right - rcEllipse.left, rcEllipse.bottom - rcEllipse.top);
		}
		::BitBlt(hDC, m_rcItem.left, m_rcItem.top, __m_szBitmap.cx, __m_szBitmap.cy, hMemDc, 0, 0, SRCCOPY);
		::SelectObject(hMemDc, hOldBitmap);
		::DeleteObject(hMemDc);
	}
	void		DoEvent(DuiLib::TEventUI& event)
	{
		if (event.Type == DuiLib::UIEVENT_BUTTONDOWN)
		{
			RECT rcPos = GetPos();
			int x = event.ptMouse.x - rcPos.left;
			int y = event.ptMouse.y - rcPos.top;

		}
	}
	DWORD		GetColor(){	return hsb_to_rgb(__m_dHue, __m_S, __m_B, 0x00); }
private:
	inline HBITMAP	__CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
	{
		LPBITMAPINFO lpbiSrc = NULL;
		lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiSrc == NULL) return NULL;
		lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiSrc->bmiHeader.biWidth = cx;
		lpbiSrc->bmiHeader.biHeight = cy;
		lpbiSrc->bmiHeader.biPlanes = 1;
		lpbiSrc->bmiHeader.biBitCount = 32;
		lpbiSrc->bmiHeader.biCompression = BI_RGB;
		lpbiSrc->bmiHeader.biSizeImage = cx * cy;
		lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biClrUsed = 0;
		lpbiSrc->bmiHeader.biClrImportant = 0;
		HBITMAP hBitmap = CreateDIBSection (hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
		delete [] lpbiSrc;
		return hBitmap;
	}
	void			__ResetBitmap()
	{
		RECT rcPos =  GetPos();
		int nWidth = rcPos.right - rcPos.left;
		int nHeight = rcPos.bottom - rcPos.top;
// 		if (__m_szBitmap.cx == nWidth && __m_szBitmap.cy == nHeight)
// 			return;
		__m_szBitmap.cx = nWidth;
		__m_szBitmap.cy = nHeight;
		if (__m_hBitmap)
		{
			__m_pBits = nullptr;
			::DeleteObject(__m_hBitmap);
			__m_hBitmap = nullptr;
		}
		__m_hBitmap = __CreateMyBitmap(GetManager()->GetPaintDC(), __m_szBitmap.cx, __m_szBitmap.cy, &__m_pBits);
		__ResetColor();
	}
	void			__ResetColor()
	{
		if (!__m_pBits) return;
		for (int i = 0; i < __m_szBitmap.cx; ++i)
		{
			double s = static_cast<double>(i) / __m_szBitmap.cx;
			for (int j = 0; j < __m_szBitmap.cy; ++j)
			{
				int nPos = (__m_szBitmap.cy - 1 - j) * __m_szBitmap.cx + i;
				double b = static_cast<double>(j) / __m_szBitmap.cy;
				__m_pBits[nPos] = static_cast<COLORREF>(hsb_to_rgb(__m_dHue, s, b, 0xFF));
			}
		}
	}
private:
	double		__m_dHue;
	HBITMAP		__m_hBitmap;
	COLORREF*	__m_pBits;
	SIZE		__m_szBitmap;
	DWORD		__m_dwColor;
	double		__m_S;
	double		__m_B;
};

class CAttr_Color : public DuiLib::CVerticalLayoutUI
{
public:
	CAttr_Color(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich(nullptr)
		, __m_bSlideHueLBtnDown(false)
		, __m_bSlideAlphaLBtnDown(false)
		, __m_bColorSetLBtnDown(false)
	{
		SetFixedHeight(20);
	}
	void DoInit()
	{
		ApplyAttributeList(_T("inset=\"5,0,5,0\" childpadding=\"5\" "));
		DuiLib::CHorizontalLayoutUI* pHor = new DuiLib::CHorizontalLayoutUI;
		pHor->ApplyAttributeList(_T("height=\"20\" childpadding=\"5\""));
		DuiLib::CLabelUI* pAttr = new DuiLib::CLabelUI;
		pAttr->ApplyAttributeList(_T("height=\"20\" width=\"80\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		pAttr->SetText(__m_strName);
		pAttr->SetToolTip(__m_strAttrib);
		__m_pBtnColor = new DuiLib::CButtonUI;
		__m_pBtnColor->ApplyAttributeList(_T("width=\"20\" height=\"20\" bordersize=\"1\" bordercolor=\"#FF3F3F46\""));
		__m_pRich = new DuiLib::CRichEditUI;
		__m_pRich->ApplyAttributeList(_T("height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		pHor->Add(pAttr);
		pHor->Add(__m_pBtnColor);
		pHor->Add(__m_pRich);
		Add(pHor);
		__m_pColorSetH = new DuiLib::CHorizontalLayoutUI;
		__m_pColorSetH->ApplyAttributeList(_T("height=\"132\" visible=\"false\" inset=\"80\" childpadding=\"5\""));
		__m_pColorSet = new CColorSetUI;
		__m_pColorSet->ApplyAttributeList(_T("width=\"132\" height=\"132\""));
		__m_pSlideHue = new DuiLib::CSliderUI;
		__m_pSlideHue->ApplyAttributeList(_T("width=\"20\" padding=\"0,-5\" height=\"142\" max=\"142\" bkimage=\"file='Attribs/hue.png' dest='0,5,20,137'\" thumbimage=\"Attribs/thumb_hue.png\" thumbsize=\"20,10\" hor=\"false\" "));
		__m_pSlideAlpha = new DuiLib::CSliderUI;
		__m_pSlideAlpha->ApplyAttributeList(_T("width=\"20\" padding=\"0,-5\" height=\"142\" max=\"142\" bkimage=\"file='Attribs/alpha.png' dest='0,5,20,137'\" thumbimage=\"Attribs/thumb_hue.png\" thumbsize=\"20,10\" hor=\"false\" "));
		__m_pColorSetH->Add(__m_pColorSet);
		__m_pColorSetH->Add(__m_pSlideHue);
		__m_pColorSetH->Add(__m_pSlideAlpha);
		Add(__m_pColorSetH);
		if (__m_pControl)
		{
			__m_pRich->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Color::__EditNotify);
			__m_pBtnColor->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Color::__BtnColorNotify);
			__m_pSlideHue->OnEvent += DuiLib::MakeDelegate(this, &CAttr_Color::__SlideHueEvent);
			__m_pColorSet->OnEvent += DuiLib::MakeDelegate(this, &CAttr_Color::__ColorSetEvent);
			__m_pSlideAlpha->OnEvent += DuiLib::MakeDelegate(this, &CAttr_Color::__SlideAlphaEvent);
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetDefaultFontInfo();
			if (pFI) GetManager()->SetDefaultFont(pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic, true);
		}
		__super::DoInit();
	}
	void DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if (__m_pControl)
		{
			IControlPlugin* pCP = static_cast<IControlPlugin*>(__m_pControl->GetInterface(DUI_CTR_CONTROL_PLUGIN_BASE));
			CDefaultControlAttrib dca;
			dca.SetControl(__m_pControl);
			if (!pCP) pCP = &dca;
			__ReadColor(pCP->GetAttribute(__m_strAttrib));
		}
		__super::DoPaint(hDC, rcPaint);
	}
private:
	bool __EditNotify(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (pNotify->pSender == __m_pRich && ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN ))
		{
			DuiLib::CDuiString strValue = __m_pRich->GetText();
			if (__m_strValue != strValue)
			{
				__ReadColor(strValue);
				__m_strValue = strValue;
				__m_pControl->SetAttribute(__m_strAttrib, strValue);
				__m_pControl->NeedUpdate();
				GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
			}
		}
		return true;
	}
	bool __BtnColorNotify(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (pNotify->pSender == __m_pBtnColor && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			if (__m_pColorSetH->IsVisible())
			{
				SetFixedHeight(20);
				GetParent()->SetFixedHeight(20);
				GetParent()->GetParent()->SetFixedHeight(20);
				__m_pColorSetH->SetVisible(false);
			}
			else
			{
				__m_pColorSetH->SetVisible(true);
				DWORD dwFixHeight = __m_pColorSetH->GetFixedHeight() + 25;
				SetFixedHeight(dwFixHeight);
				GetParent()->SetFixedHeight(dwFixHeight);
				GetParent()->GetParent()->SetFixedHeight(dwFixHeight);
			}
			return false;
		}
		return true;
	}
	bool __SlideHueEvent(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TEventUI* pEvent = static_cast<DuiLib::TEventUI*>(_lpParam);
		if (!pEvent) return true;
		if (pEvent->pSender == __m_pSlideHue && pEvent->Type == DuiLib::UIEVENT_MOUSEMOVE && __m_bSlideHueLBtnDown)
		{
			__SetHue(pEvent->ptMouse.y);
			__SetColor();
		}
		else if (pEvent->pSender == __m_pSlideHue && pEvent->Type == DuiLib::UIEVENT_BUTTONDOWN)
		{
			__SetHue(pEvent->ptMouse.y);
			__SetColor();
			__m_bSlideHueLBtnDown = true;
		}
		else if (pEvent->pSender == __m_pSlideHue && pEvent->Type == DuiLib::UIEVENT_BUTTONUP)
		{
			__m_bSlideHueLBtnDown = false;
			GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
		}
		return true;
	}
	bool __ColorSetEvent(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TEventUI* pEvent = static_cast<DuiLib::TEventUI*>(_lpParam);
		if (!pEvent) return true;
		if (pEvent->pSender == __m_pColorSet && pEvent->Type == DuiLib::UIEVENT_MOUSEMOVE && __m_bColorSetLBtnDown)
		{
			__m_pColorSet->SetColorPt(pEvent->ptMouse.x, pEvent->ptMouse.y);
			__SetColor();
		}
		else if (pEvent->pSender == __m_pColorSet && pEvent->Type == DuiLib::UIEVENT_BUTTONDOWN)
		{
			__m_pColorSet->SetColorPt(pEvent->ptMouse.x, pEvent->ptMouse.y);
			__m_bColorSetLBtnDown = true;
			__SetColor();
		}
		else if (pEvent->pSender == __m_pColorSet && pEvent->Type == DuiLib::UIEVENT_BUTTONUP)
		{
			__m_bColorSetLBtnDown = false;
			GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
		}
		return true;
	}
	bool __SlideAlphaEvent(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TEventUI* pEvent = static_cast<DuiLib::TEventUI*>(_lpParam);
		if (!pEvent) return true;
		if (pEvent->pSender == __m_pSlideAlpha && pEvent->Type == DuiLib::UIEVENT_MOUSEMOVE && __m_bSlideAlphaLBtnDown)
		{
			__SetAlpha(pEvent->ptMouse.y);
			__SetColor();
		}
		else if (pEvent->pSender == __m_pSlideAlpha && pEvent->Type == DuiLib::UIEVENT_BUTTONDOWN)
		{
			__SetAlpha(pEvent->ptMouse.y);
			__SetColor();
			__m_bSlideAlphaLBtnDown = true;
		}
		else if (pEvent->pSender == __m_pSlideAlpha && pEvent->Type == DuiLib::UIEVENT_BUTTONUP)
		{
			__m_bSlideAlphaLBtnDown = false;
			GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
		}
		return true;
	}
	void __SetHue(int _nPos)
	{
		int nPos = _nPos;
		RECT rcPos = __m_pSlideHue->GetPos();
		if (nPos < rcPos.top) nPos = rcPos.top;
		if (nPos > rcPos.bottom) nPos = rcPos.bottom;
		__m_pSlideHue->SetValue(rcPos.bottom - nPos);
		double dHue = static_cast<double>(__m_pSlideHue->GetValue()) / (__m_pSlideHue->GetMaxValue() - __m_pSlideHue->GetMinValue());
		__m_pColorSet->SetHue(dHue * 360);
	}
	void __SetAlpha(int _nPos)
	{
		int nPos = _nPos;
		RECT rcPos = __m_pSlideAlpha->GetPos();
		if (nPos < rcPos.top) nPos = rcPos.top;
		if (nPos > rcPos.bottom) nPos = rcPos.bottom;
		__m_pSlideAlpha->SetValue(rcPos.bottom - nPos);
	}
	void __SetColor()
	{
		double dAlpha = static_cast<double>(__m_pSlideAlpha->GetValue()) / (__m_pSlideAlpha->GetMaxValue() - __m_pSlideAlpha->GetMinValue());
		DuiLib::CDuiString strColor;
		strColor.Format(_T("#%02X%06X"), zn_round(dAlpha * 255), __m_pColorSet->GetColor());
		if (strColor != __m_strValue)
		{
			__m_strValue = strColor;
			__m_pRich->SetText(__m_strValue);
			__m_pBtnColor->SetAttribute(_T("bkcolor"), __m_strValue);
			__m_pControl->SetAttribute(__m_strAttrib, __m_strValue);
			__m_pControl->NeedUpdate();
		}
	}
	void __ReadColor(LPCTSTR _lpColor)
	{
		if (__m_strValue != _lpColor)
		{
			__m_strValue = _lpColor;
			__m_pRich->SetText(__m_strValue);
			while( *_lpColor > _T('\0') && *_lpColor <= _T(' ') ) _lpColor = ::CharNext(_lpColor);
			if( *_lpColor == _T('#')) _lpColor = ::CharNext(_lpColor);
			LPTSTR lpStr = nullptr;
			DWORD dwColor = _tcstoul(_lpColor, &lpStr, 16);
			__m_pBtnColor->SetBkColor(dwColor);
			int nAlpha;
			double h, s, b;
			rgb_to_hsb(dwColor, h, s, b, nAlpha);
			__m_pSlideAlpha->SetValue(static_cast<int>(nAlpha / 255.0 * __m_pSlideAlpha->GetMaxValue()));
			__m_pSlideHue->SetValue(static_cast<int>(h / 360.0 * __m_pSlideAlpha->GetMaxValue()));
			__m_pColorSet->SetHue(h);
			__m_pColorSet->SetSaturation(s);
			__m_pColorSet->SetBrightness(b);
		}
	}
private:
	DuiLib::CControlUI*				__m_pControl;
	DuiLib::CDuiString				__m_strAttrib;
	DuiLib::CDuiString				__m_strName;
	DuiLib::CDuiString				__m_strValue;
	DuiLib::CButtonUI*				__m_pBtnColor;
	DuiLib::CRichEditUI*			__m_pRich;
	DuiLib::CHorizontalLayoutUI*	__m_pColorSetH;
	CColorSetUI*					__m_pColorSet;
	DuiLib::CSliderUI*				__m_pSlideHue;
	DuiLib::CSliderUI*				__m_pSlideAlpha;
	bool							__m_bSlideHueLBtnDown;
	bool							__m_bSlideAlphaLBtnDown;
	bool							__m_bColorSetLBtnDown;
};

#endif