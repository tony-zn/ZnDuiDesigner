#pragma once
#ifndef __POS_HPP_2015_11_16
#define __POS_HPP_2015_11_16

class CAttr_Pos : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_Pos(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pRich_X(nullptr)
		, __m_pRich_Y(nullptr)
		, __m_pRich_Width(nullptr)
		, __m_pRich_Height(nullptr)
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
		pName1->ApplyAttributeList(_T("text=\"X\" width=\"25\" height=\"20\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_X = new DuiLib::CRichEditUI;
		__m_pRich_X->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pName2 = new DuiLib::CLabelUI;
		pName2->ApplyAttributeList(_T("text=\"Y\" width=\"25\" height=\"20\" padding=\"10\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Y = new DuiLib::CRichEditUI;
		__m_pRich_Y->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		pTop->Add(pName1);
		pTop->Add(__m_pRich_X);
		pTop->Add(pName2);
		pTop->Add(__m_pRich_Y);

		DuiLib::CLabelUI* pName3 = new DuiLib::CLabelUI;
		pName3->ApplyAttributeList(_T("text=\"¿í\" width=\"25\" height=\"20\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Width = new DuiLib::CRichEditUI;
		__m_pRich_Width->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pName4 = new DuiLib::CLabelUI;
		pName4->ApplyAttributeList(_T("text=\"¸ß\" width=\"25\" height=\"20\" padding=\"10\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pRich_Height = new DuiLib::CRichEditUI;
		__m_pRich_Height->ApplyAttributeList(_T("width=\"50\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		pBottom->Add(pName3);
		pBottom->Add(__m_pRich_Width);
		pBottom->Add(pName4);
		pBottom->Add(__m_pRich_Height);

		pV->Add(pTop);
		pV->Add(pBottom);

		Add(pAttr);
		Add(pV);
		if (__m_pControl) 
		{
			__m_pRich_X->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Pos::__EditNotify);
			__m_pRich_Y->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Pos::__EditNotify);
			__m_pRich_Width->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Pos::__EditNotify);
			__m_pRich_Height->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Pos::__EditNotify);

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
			__m_pRich_X->SetEnabled(__m_pControl->IsFloat());
			__m_pRich_Y->SetEnabled(__m_pControl->IsFloat());
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
				strRect[2].Format(_T("%d"), rc.right - rc.left);
				strRect[3].Format(_T("%d"), rc.bottom - rc.top);
				__m_pRich_X->SetText(strRect[0]);
				__m_pRich_Y->SetText(strRect[1]);
				__m_pRich_Width->SetText(strRect[2]);
				__m_pRich_Height->SetText(strRect[3]);
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
		if ( (pNotify->pSender == __m_pRich_X || pNotify->pSender == __m_pRich_Y || pNotify->pSender == __m_pRich_Width || pNotify->pSender == __m_pRich_Height )
			&& ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN ))
		{
			POINT pt = {_tstoi(__m_pRich_X->GetText()), _tstoi(__m_pRich_Y->GetText())};
			SIZE  sz = {_tstoi(__m_pRich_Width->GetText()), _tstoi(__m_pRich_Height->GetText())};
			DuiLib::CDuiString strValue;
			strValue.Format(_T("%d,%d,%d,%d")
				, pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);
			if (__m_strValue != strValue)
			{
				__m_strValue = strValue;
				__m_pControl->SetAttribute(__m_strAttrib, strValue);
				if (__m_pControl->GetParent())
					__m_pControl->GetParent()->SetPos(__m_pControl->GetParent()->GetPos());
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
	DuiLib::CRichEditUI*	__m_pRich_Width;
	DuiLib::CRichEditUI*	__m_pRich_Height;
	DuiLib::CDuiString		__m_strValue;
};

#endif