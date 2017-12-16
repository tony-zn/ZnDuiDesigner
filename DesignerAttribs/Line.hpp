#pragma once
#ifndef __LINE_HPP_2015_11_09
#define __LINE_HPP_2015_11_09

class CAttr_Line : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Line(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
	{
		SetFixedHeight(20);
	}
	void DoInit()
	{
		if (__m_strName.GetLength())
		{
			DuiLib::CControlUI* pLeft = new DuiLib::CControlUI;
			pLeft->ApplyAttributeList(_T("height=\"1\" bkcolor=\"FF3F3F46\" padding=\"0,10\" "));
			DuiLib::CLabelUI* pText = new DuiLib::CLabelUI;
			pText->ApplyAttributeList(_T("align=\"center\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
			SIZE sz = get_text_size(GetManager()->GetPaintDC(), GetManager()->GetDefaultFontInfo()->hFont, __m_strName);
			pText->SetFixedWidth(sz.cx + 10);
			pText->SetText(__m_strName);
			DuiLib::CControlUI* pRight = new DuiLib::CControlUI;
			pRight->ApplyAttributeList(_T("height=\"1\" bkcolor=\"FF3F3F46\" padding=\"0,10\" "));
			Add(pLeft);
			Add(pText);
			Add(pRight);
		}
		else
		{
			DuiLib::CControlUI* pControl = new DuiLib::CControlUI;
			pControl->ApplyAttributeList(_T("height=\"1\" bkcolor=\"FF3F3F46\" padding=\"0,10\" "));
			Add(pControl);
		}
		if (__m_pControl)
		{
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetDefaultFontInfo();
			if (pFI) GetManager()->SetDefaultFont(pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic, true);
		}
		__super::DoInit();
	}
	void DoPaint(HDC hDC, const RECT& rcPaint)
	{
		__super::DoPaint(hDC, rcPaint);
	}
private:
	DuiLib::CControlUI*		__m_pControl;
	DuiLib::CDuiString		__m_strAttrib;
	DuiLib::CDuiString		__m_strName;
};

#endif