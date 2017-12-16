#pragma once
#ifndef __DS_AUTOWIDTHLABEL_HPP_2015_11_16
#define __DS_AUTOWIDTHLABEL_HPP_2015_11_16

SIZE get_text_size(HDC _hDC, HFONT _hFont, LPCTSTR _lpStr)
{
	SIZE sz = {0};
	HFONT hOld = (HFONT)::SelectObject(_hDC, _hFont);
	::GetTextExtentPoint32(_hDC, _lpStr, _tcslen(_lpStr), &sz);
	::SelectObject(_hDC, hOld);
	return sz;
}

class CDS_AutoWidthOptionUI : public DuiLib::COptionUI, public CControlPlugin_LabelUI
{
public:
	CDS_AutoWidthOptionUI()
	{
		SetControl(this);
	}
	LPCTSTR		GetClass() const
	{
		return _T("AutoWidthOption");
	}
	LPVOID		GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(_T("AutoWidthOption"), pstrName) == 0)
			return this;
		else if (_tcscmp(DUI_CTR_CONTROL_PLUGIN_BASE, pstrName) == 0)
			return static_cast<IControlPlugin*>(this);
		return DuiLib::COptionUI::GetInterface(pstrName);
	}
	void		SetPos(RECT rc, bool bNeedInvalidate = true)
	{
		SIZE sz = get_text_size(GetManager()->GetPaintDC(), GetManager()->GetFont(GetFont()), GetText());
		sz.cx += m_rcTextPadding.left + m_rcTextPadding.right;
		if (m_cxyFixed.cx != sz.cx)
			SetFixedWidth(sz.cx);
		DuiLib::COptionUI::SetPos(rc);
	}
};

#endif