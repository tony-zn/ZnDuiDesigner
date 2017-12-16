#pragma once
#ifndef __SIZE_HPP_2015_11_11
#define __SIZE_HPP_2015_11_11

class CAttr_Size : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Size(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich_X(nullptr)
		, __m_pRich_Y(nullptr)
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
		DuiLib::CHorizontalLayoutUI* pH = new DuiLib::CHorizontalLayoutUI;
		DuiLib::CLabelUI* pName1 = new DuiLib::CLabelUI;
		pName1->ApplyAttributeList(_T("text=\"¿í\" width=\"25\" height=\"20\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_X = new DuiLib::CRichEditUI;
		__m_pRich_X->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pName2 = new DuiLib::CLabelUI;
		pName2->ApplyAttributeList(_T("text=\"¸ß\" width=\"25\" height=\"20\" padding=\"10\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Y = new DuiLib::CRichEditUI;
		__m_pRich_Y->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		pH->Add(pName1);
		pH->Add(__m_pRich_X);
		pH->Add(pName2);
		pH->Add(__m_pRich_Y);

		Add(pAttr);
		Add(pH);
		if (__m_pControl) 
		{
			__m_pRich_X->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Size::__EditNotify);
			__m_pRich_Y->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Size::__EditNotify);
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
				LPTSTR pstr = NULL;
				SIZE sz = {0};
				sz.cx = _tcstol(strValue, &pstr, 10);  ASSERT(pstr);    
				sz.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				DuiLib::CDuiString strRect[2];
				strRect[0].Format(_T("%d"), sz.cx);
				strRect[1].Format(_T("%d"), sz.cy);
				__m_pRich_X->SetText(strRect[0]);
				__m_pRich_Y->SetText(strRect[1]);
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
		if ( (pNotify->pSender == __m_pRich_X || pNotify->pSender == __m_pRich_Y )
			&& ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN ))
		{
			DuiLib::CDuiString strValue;
			strValue.Format(_T("%d,%d")
				, _tstoi(__m_pRich_X->GetText())
				, _tstoi(__m_pRich_Y->GetText()));
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
	DuiLib::CRichEditUI*	__m_pRich_X;
	DuiLib::CRichEditUI*	__m_pRich_Y;
	DuiLib::CDuiString		__m_strValue;
};

#endif