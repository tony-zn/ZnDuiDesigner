#pragma once
#ifndef __CHECKBOX_HPP_2015_11_11
#define __CHECKBOX_HPP_2015_11_11

class CAttr_CheckBox : public DuiLib::CHorizontalLayoutUI
{
public:
	CAttr_CheckBox(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pOpt(nullptr)
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
		__m_pOpt = new DuiLib::COptionUI;
		__m_pOpt->ApplyAttributeList(_T("width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		Add(pAttr);
		Add(__m_pOpt);
		if (__m_pOpt) 
		{
			__m_pOpt->OnNotify += DuiLib::MakeDelegate(this, &CAttr_CheckBox::__EditNotify);
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
				__m_pOpt->Selected(strValue == _T("true"));
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
		if ( pNotify->pSender == __m_pOpt
			&& pNotify->sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			DuiLib::CDuiString strValue(__m_pOpt->IsSelected() ? _T("true") : _T("false"));
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
	DuiLib::COptionUI*		__m_pOpt;
	DuiLib::CDuiString		__m_strValue;
};

#endif