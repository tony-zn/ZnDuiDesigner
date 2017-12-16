#pragma once
#ifndef __EDIT_HPP_2015_11_06
#define __EDIT_HPP_2015_11_06

class CAttr_Edit : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Edit(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich(nullptr)
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
		__m_pRich->ApplyAttributeList(_T("height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		Add(pAttr);
		Add(__m_pRich);
		if (__m_pControl) __m_pRich->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Edit::__EditNotify);
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
			IControlPlugin* pCP = static_cast<IControlPlugin*>(__m_pControl->GetInterface(DUI_CTR_CONTROL_PLUGIN_BASE));
			CDefaultControlAttrib dca;
			dca.SetControl(__m_pControl);
			if (!pCP) pCP = &dca;
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
		if (pNotify->pSender == __m_pRich && ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN ))
		{
			DuiLib::CDuiString strValue = __m_pRich->GetText();
			if (__m_strValue != strValue)
			{
				__m_strValue = strValue;
				__m_pControl->SetAttribute(__m_strAttrib, strValue);
				__m_pControl->NeedUpdate();
				GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
			}
		}
		return true;
	}
private:
	DuiLib::CControlUI*		__m_pControl;
	DuiLib::CDuiString		__m_strAttrib;
	DuiLib::CDuiString		__m_strName;
	DuiLib::CRichEditUI*	__m_pRich;
	DuiLib::CDuiString		__m_strValue;
};
#endif