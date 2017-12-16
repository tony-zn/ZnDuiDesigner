#pragma once
#ifndef __RECT_HPP_2015_11_12
#define __RECT_HPP_2015_11_12

class CAttr_Rect : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Rect(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich_Left(nullptr)
		, __m_pRich_Top(nullptr)
		, __m_pRich_Right(nullptr)
		, __m_pRich_Bottom(nullptr)
	{
		SetFixedHeight(45);
	}
	void DoInit()
	{
		ApplyAttributeList(_T("inset=\"5,0,5,0\" "));
		DuiLib::CLabelUI* pAttr = new DuiLib::CLabelUI;
		pAttr->ApplyAttributeList(_T("height=\"20\" width=\"80\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		pAttr->SetText(__m_strName);
		pAttr->SetToolTip(__m_strAttrib);
		DuiLib::CVerticalLayoutUI* pV = new DuiLib::CVerticalLayoutUI;
		DuiLib::CHorizontalLayoutUI* pTop = new DuiLib::CHorizontalLayoutUI;
		pTop->ApplyAttributeList(_T("height=\"20\" "));
		DuiLib::CHorizontalLayoutUI* pBottom = new DuiLib::CHorizontalLayoutUI;
		pBottom->ApplyAttributeList(_T("padding=\"0,5\" height=\"20\" "));
		DuiLib::CLabelUI* pName1 = new DuiLib::CLabelUI;
		pName1->ApplyAttributeList(_T("text=\"вС\" width=\"25\" height=\"20\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Left = new DuiLib::CRichEditUI;
		__m_pRich_Left->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pName2 = new DuiLib::CLabelUI;
		pName2->ApplyAttributeList(_T("text=\"ио\" width=\"25\" height=\"20\" padding=\"10\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Top = new DuiLib::CRichEditUI;
		__m_pRich_Top->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		pTop->Add(pName1);
		pTop->Add(__m_pRich_Left);
		pTop->Add(pName2);
		pTop->Add(__m_pRich_Top);

		DuiLib::CLabelUI* pName3 = new DuiLib::CLabelUI;
		pName3->ApplyAttributeList(_T("text=\"ср\" width=\"25\" height=\"20\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Right = new DuiLib::CRichEditUI;
		__m_pRich_Right->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pName4 = new DuiLib::CLabelUI;
		pName4->ApplyAttributeList(_T("text=\"об\" width=\"25\" height=\"20\" padding=\"10\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Bottom = new DuiLib::CRichEditUI;
		__m_pRich_Bottom->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		pBottom->Add(pName3);
		pBottom->Add(__m_pRich_Right);
		pBottom->Add(pName4);
		pBottom->Add(__m_pRich_Bottom);

		pV->Add(pTop);
		pV->Add(pBottom);

		Add(pAttr);
		Add(pV);
		if (__m_pControl) 
		{
			__m_pRich_Left->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Rect::__EditNotify);
			__m_pRich_Top->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Rect::__EditNotify);
			__m_pRich_Right->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Rect::__EditNotify);
			__m_pRich_Bottom->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Rect::__EditNotify);
			
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
				RECT rc = {0};
				rc.left = _tcstol(strValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
				rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);   
				DuiLib::CDuiString strRect[4];
				strRect[0].Format(_T("%d"), rc.left);
				strRect[1].Format(_T("%d"), rc.top);
				strRect[2].Format(_T("%d"), rc.right);
				strRect[3].Format(_T("%d"), rc.bottom);
				__m_pRich_Left->SetText(strRect[0]);
				__m_pRich_Top->SetText(strRect[1]);
				__m_pRich_Right->SetText(strRect[2]);
				__m_pRich_Bottom->SetText(strRect[3]);
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
		if ( (pNotify->pSender == __m_pRich_Left || pNotify->pSender == __m_pRich_Top || pNotify->pSender == __m_pRich_Right || pNotify->pSender == __m_pRich_Bottom )
			&& ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN ))
		{
			DuiLib::CDuiString strValue;
			strValue.Format(_T("%d,%d,%d,%d")
				, _tstoi(__m_pRich_Left->GetText())
				, _tstoi(__m_pRich_Top->GetText())
				, _tstoi(__m_pRich_Right->GetText())
				, _tstoi(__m_pRich_Bottom->GetText()));
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
	DuiLib::CRichEditUI*	__m_pRich_Left;
	DuiLib::CRichEditUI*	__m_pRich_Top;
	DuiLib::CRichEditUI*	__m_pRich_Right;
	DuiLib::CRichEditUI*	__m_pRich_Bottom;
	DuiLib::CDuiString		__m_strValue;
};

#endif