#pragma once
#ifndef __FOLDER_HPP_2015_11_17
#define __FOLDER_HPP_2015_11_17
#include <ShlObj.h>
#include <Shlwapi.h>

class CAttr_Folder : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Folder(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich(nullptr)
		, __m_pButton(nullptr)
	{
		SetFixedHeight(20);
	}
	void DoInit()
	{
		ApplyAttributeList(_T("inset=\"5,0,5,0\" "));
		DuiLib::CLabelUI* pAttr = new DuiLib::CLabelUI;
		pAttr->ApplyAttributeList(_T("height=\"20\" width=\"80\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		pAttr->SetText(__m_strName);
		pAttr->SetToolTip(__m_strAttrib);
		__m_pRich = new DuiLib::CRichEditUI;
		__m_pRich->ApplyAttributeList(_T("readonly=\"true\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		__m_pButton = new DuiLib::CButtonUI;
		__m_pButton->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"ä¯ÀÀ...\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		Add(pAttr);
		Add(__m_pRich);
		Add(__m_pButton);
		if (__m_pControl) __m_pButton->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Folder::__EditNotify);
		if (__m_pControl)
		{
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetDefaultFontInfo();
			if (pFI) GetManager()->SetDefaultFont(pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic, true);
		}
		__super::DoInit();
	}
	void DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if (__m_pControl)
		{
			IControlPlugin* pCP = get_control_interface(__m_pControl);
			DuiLib::CDuiString strValue = pCP->GetAttribute(__m_strAttrib);
			if (__m_strValue != strValue)
			{
				__m_strValue = strValue;
				__m_pRich->SetText(strValue);
			}
		}
		__super::DoPaint(hDC, rcPaint);
	}
private:
	bool __EditNotify(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (pNotify->pSender == __m_pButton && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			static TCHAR szPath[MAX_PATH] = { 0 };
			BROWSEINFO bi = { 0 };
			bi.hwndOwner = GetManager()->GetPaintWindow();
			bi.pszDisplayName = szPath;
			bi.ulFlags = BIF_RETURNONLYFSDIRS;
			LPITEMIDLIST pIDList = ::SHBrowseForFolder(&bi);
			if (!pIDList) return nullptr;
			if (::SHGetPathFromIDList(pIDList, szPath))
			{
				if (szPath[_tcslen(szPath) - 1] != _T('\\'))
					_tcscat_s(szPath, _T("\\"));
				__m_pRich->SetText(szPath);
				if (__m_strValue != szPath)
				{
					__m_strValue = szPath;
					__m_pControl->SetAttribute(__m_strAttrib, __m_strValue);
					__m_pControl->NeedUpdate();
				}
			}
			GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
		}
		return true;
	}
private:
	DuiLib::CControlUI*		__m_pControl;
	DuiLib::CDuiString		__m_strAttrib;
	DuiLib::CDuiString		__m_strName;
	DuiLib::CRichEditUI*	__m_pRich;
	DuiLib::CButtonUI*		__m_pButton;
	DuiLib::CDuiString		__m_strValue;
};

#endif