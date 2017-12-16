#pragma once
#ifndef __FONT_HPP_2015_11_25
#define __FONT_HPP_2015_11_25

class CAttr_Font : public DuiLib::CVerticalLayoutUI
{
public:
	CAttr_Font(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pCombo(nullptr)
	{
		SetFixedHeight(20);
	}
	void DoInit()
	{
		ApplyAttributeList(_T("inset=\"5,0,5,0\" childpadding=\"5\" "));
		DuiLib::CLabelUI* pAttr = new DuiLib::CLabelUI;
		pAttr->ApplyAttributeList(_T("height=\"20\" width=\"80\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		pAttr->SetText(__m_strName);
		pAttr->SetToolTip(__m_strAttrib);
		__m_pCombo = new DuiLib::CComboUI;
		DuiLib::CHorizontalLayoutUI* pHor = new DuiLib::CHorizontalLayoutUI;
		pHor->Add(pAttr);
		pHor->Add(__m_pCombo);
		__m_pManagerBtn = new DuiLib::CButtonUI;
		__m_pManagerBtn->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"管理\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		pHor->Add(__m_pManagerBtn);
		Add(pHor);
		__m_pCombo->ApplyAttributeList(_T("width=\"150\" height=\"20\" bkimage=\"file='drop.png' dest='136,9,142,12' \" vscrollbar=\"true\" textpadding=\"2,2,22,2\"  bordersize=\"1\" bordercolor=\"#FF3F3F46\" bkcolor=\"#FF333337\" dropboxsize=\"150,300\" itemtextcolor=\"#FFCCCCCC\" itemhottextcolor=\"#FFCCCCCC\" itemselectedtextcolor=\"#FFCCCCCC\" itemhotbkcolor=\"#FF3F3F46\" itemselectedbkcolor=\"#FF007ACC\" itemtextpadding=\"5\" "));
		__m_pCombo->SetDropBoxAttributeList(_T("bkcolor=\"#FF252526\" bordercolor=\"#FF3F3F46\" "));
		__m_pList = new DuiLib::CListUI;
		__InitList(__m_pList);
		if (__m_pControl)
		{
			GetManager()->AddDefaultAttributeList(_T("VScrollBar"), __m_pControl->GetManager()->GetDefaultAttributeList(_T("VScrollBar")), true);
			__m_pList->GetVerticalScrollBar()->ApplyAttributeList(GetManager()->GetDefaultAttributeList(_T("VScrollBar")));
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetDefaultFontInfo();
			GetManager()->SetDefaultFont(pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic, true);
			__FillCombo(__m_pCombo);
			__FillList(__m_pList);	
		}
		DuiLib::CHorizontalLayoutUI* pListManager = new DuiLib::CHorizontalLayoutUI;
		pListManager->ApplyAttributeList(_T("height=\"20\" childpadding=\"10\""));
		DuiLib::CControlUI* pLeft = new DuiLib::CControlUI;
		__m_pBtnAdd = new DuiLib::CButtonUI;
		__m_pBtnAdd->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"添加\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		__m_pBtnDel = new DuiLib::CButtonUI;
		__m_pBtnDel->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"删除\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		__m_pBtnDefault = new DuiLib::CButtonUI;
		__m_pBtnDefault->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"默认\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		DuiLib::CControlUI* pRight = new DuiLib::CControlUI;
		pListManager->Add(pLeft);
		pListManager->Add(__m_pBtnAdd);
		pListManager->Add(__m_pBtnDel);
		pListManager->Add(__m_pBtnDefault);
		pListManager->Add(pRight);
		__m_pFontManagerV = new DuiLib::CVerticalLayoutUI;
		__m_pFontManagerV->ApplyAttributeList(_T("height=\"200\""));
		__m_pFontManagerV->SetVisible(false);
		__m_pFontManagerV->Add(pListManager);
		__m_pFontManagerV->Add(__m_pList);
		Add(__m_pFontManagerV);
		if (__m_pCombo)			__m_pCombo->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontChange);
		if (__m_pManagerBtn)	__m_pManagerBtn->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnManager);
		if (__m_pBtnAdd)		__m_pBtnAdd->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnAdd);
		if (__m_pBtnDel)		__m_pBtnDel->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnDelete);
		if (__m_pBtnDefault)	__m_pBtnDefault->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnDefault);
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
				for (int i = 0; i < __m_pCombo->GetCount(); ++i)
				{
					if (__m_strValue == __m_pCombo->GetItemAt(i)->GetUserData())
					{
						__m_pCombo->SelectItem(i);
						break;
					}
				}
			}
		}
		__super::DoPaint(hDC, rcPaint);
	}
private:
	bool __OnManager(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (pNotify->pSender == __m_pManagerBtn && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			if (__m_pFontManagerV->IsVisible())
			{
				SetFixedHeight(20);
				GetParent()->SetFixedHeight(20);
				GetParent()->GetParent()->SetFixedHeight(20);
				__m_pFontManagerV->SetVisible(false);
			}
			else
			{
				__m_pFontManagerV->SetVisible(true);
				SetFixedHeight(225);
				GetParent()->SetFixedHeight(225);
				GetParent()->GetParent()->SetFixedHeight(225);
			}
			return false;
		}
		return true;
	}
	bool __OnFontChange(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if ( pNotify->pSender == __m_pCombo
			&& pNotify->sType == DUI_MSGTYPE_ITEMSELECT)
		{
			DuiLib::CControlUI* pItem = __m_pCombo->GetItemAt(__m_pCombo->GetCurSel());
			DuiLib::CDuiString strValue(pItem->GetUserData());
			if (__m_strValue != strValue)
			{
				__m_strValue = strValue;
				__m_pControl->SetAttribute(__m_strAttrib, strValue);
				__m_pControl->NeedUpdate();
				GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
			}
			return false;
		}
		return true;
	}
	bool __OnFontStyleOption(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		DuiLib::COptionUI* pOpt = static_cast<DuiLib::COptionUI*>(pNotify->pSender->GetInterface(DUI_CTR_OPTION));	
		if (!pOpt) return true;
		if (pNotify->sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			int nFont = pNotify->pSender->GetTag();
			DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
			IControlPlugin* pInterface = get_control_interface(pWnd);
			if (!pInterface) return true;
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetFontInfo(nFont);
			if (!pFI || pFI->hFont == __m_pControl->GetManager()->GetDefaultFontInfo()->hFont)
				return true;
			if (pOpt->GetUserData() == _T("bold"))
				pFI->bBold = pOpt->IsSelected();
			else if (pOpt->GetUserData() == _T("italic"))
				pFI->bItalic = pOpt->IsSelected();
			else if (pOpt->GetUserData() == _T("underline"))
				pFI->bUnderline = pOpt->IsSelected();
			else if (pOpt->GetUserData() == _T("shared"))
			{
				IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
				if (!pWSD) return true;
				pWSD->SetSharedFont(nFont, pOpt->IsSelected());
			}
			__m_pControl->GetManager()->AddFont(nFont, pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic);
			__ChangeFont();
			return false;
		}
		return true;
	}
	bool __OnAdd(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if (pNotify->pSender == __m_pBtnAdd && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
			IControlPlugin* pInterface = get_control_interface(pWnd);
			IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
			if (pWnd && pInterface && pWSD)
			{
				DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
				DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
				int nFont = _tstoi(strFont);
				int nDefaultFont = _tstoi(strDefaultFont);
				for (int i = nFont; i < nFont + 100; ++i)
				{
					DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetFontInfo(i);
					if (!pFI || pFI->hFont == __m_pControl->GetManager()->GetDefaultFontInfo()->hFont)
					{
						GetManager()->AddFont(i, _T("微软雅黑"), 12, false, false, false);
						DuiLib::TFontInfo* pFI = GetManager()->GetFontInfo(i);
						if (pFI) 
						{
							__AddFontListItem(__m_pList, i, nFont, pFI, i == nDefaultFont, false);
							__m_pList->SetPos(__m_pList->GetPos());
							__m_pList->SelectItem(__m_pList->GetCount() - 1);
							__m_pList->GetVerticalScrollBar()->SetScrollPos(__m_pList->GetVerticalScrollBar()->GetScrollRange());
							__ChangeFont();
							return false;
						}
					}
				}
			}
			return true;
		}
		return true;
	}
	bool __OnDelete(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if (pNotify->pSender == __m_pBtnDel && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			DuiLib::CControlUI* pItem = __m_pList->GetItemAt(__m_pList->GetCurSel());
			if (!pItem) return true;
			int nFont = pItem->GetTag();
			__m_pList->Remove(pItem);
			DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
			IControlPlugin* pInterface = get_control_interface(pWnd);
			IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
			if (pWnd && pInterface && pWSD)
			{
				GetManager()->RemoveFont(nFont);
				pWSD->SetSharedFont(nFont, false);
			}
			__ChangeFont();
			return false;
		}
		return true;
	}
	bool __OnDefault(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if (pNotify->pSender == __m_pBtnDefault && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			DuiLib::CControlUI* pItem = __m_pList->GetItemAt(__m_pList->GetCurSel());
			if (!pItem) return true;
			DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
			DuiLib::CDuiString strDefaultFont;
			strDefaultFont.Format(_T("%d"), pItem->GetTag());
			pWnd->SetAttribute(_T("defaultfont"), strDefaultFont);
			for (int i = 0; i < __m_pList->GetCount(); ++i)
			{
				DuiLib::CControlUI* pItem = __m_pList->GetItemAt(i);
				DuiLib::CControlUI* pHor = static_cast<DuiLib::CContainerUI*>(pItem)->GetItemAt(0);
				DuiLib::CControlUI* pDefault = static_cast<DuiLib::CContainerUI*>(pHor)->GetItemAt(0);
				pDefault->SetText(i == __m_pList->GetCurSel() ? _T("*") : _T(""));
			}
			__ChangeFont();
			return false;
		}
		return true;
	}
	bool __OnFontNameChange(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if ( pNotify->sType == DUI_MSGTYPE_ITEMSELECT)
		{
			DuiLib::CComboUI* pCmbo = static_cast<DuiLib::CComboUI*>(pNotify->pSender->GetInterface(DUI_CTR_COMBO));
			if (!pCmbo) return true;
			int nFont = pCmbo->GetTag();
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetFontInfo(nFont);
			if (!pFI || pFI->hFont == __m_pControl->GetManager()->GetDefaultFontInfo()->hFont)
				return true;
			DuiLib::CControlUI* pItem = pCmbo->GetItemAt(pCmbo->GetCurSel());
			if (!pItem) return true;
			__m_pControl->GetManager()->AddFont(nFont, pItem->GetText().GetData(), pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic);
			__ChangeFont();
			return false;
		}
		return true;
	}
	bool __OnFontSizeChange(LPVOID _lpParam)
	{
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (!__m_pControl) return true;
		if ( pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN )
		{
			int nFont = pNotify->pSender->GetTag();
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetFontInfo(nFont);
			if (!pFI || pFI->hFont == __m_pControl->GetManager()->GetDefaultFontInfo()->hFont)
				return true;
			int nSize = _tstoi(pNotify->pSender->GetText());
			if (nSize > 0)
				__m_pControl->GetManager()->AddFont(nFont, pFI->sFontName, nSize, pFI->bBold, pFI->bUnderline, pFI->bItalic);
			__ChangeFont();
			return false;
		}
		return true;
	}
	void __InitList(DuiLib::CListUI* _pList)
	{
		_pList->ApplyAttributeList(_T("bordersize=\"1\" bordercolor=\"#FF3F3F46\" vscrollbar=\"true\" itemhotimage=\"gray.png\" itemselectedimage=\"blue.png\" itemlinecolor=\"#FF3F3F46\" sepheight=\"2\""));
		DuiLib::CListHeaderUI* pHeader = _pList->GetHeader();
		pHeader->SetFixedHeight(20);
		pHeader->ApplyAttributeList(_T("height=\"20\" sepwidth=\"0\" bottombordersize=\"1\" bordercolor=\"#FF3F3F46\" bkcolor=\"#FF3F3F46\" childpadding=\"2\""));
		DuiLib::CLabelUI* pItem0 = new DuiLib::CLabelUI;
		pItem0->ApplyAttributeList(_T("width=\"20\" text=\"\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem1 = new DuiLib::CLabelUI;
		pItem1->ApplyAttributeList(_T("width=\"30\" text=\"ID\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem2 = new DuiLib::CLabelUI;
		pItem2->ApplyAttributeList(_T("width=\"80\" text=\"字体\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem3 = new DuiLib::CLabelUI;
		pItem3->ApplyAttributeList(_T("width=\"30\" text=\"字号\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem4 = new DuiLib::CLabelUI;
		pItem4->ApplyAttributeList(_T("width=\"20\" text=\"B\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem5 = new DuiLib::CLabelUI;
		pItem5->ApplyAttributeList(_T("width=\"20\" text=\"I\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem6 = new DuiLib::CLabelUI;
		pItem6->ApplyAttributeList(_T("width=\"20\" text=\"U\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		DuiLib::CLabelUI* pItem7 = new DuiLib::CLabelUI;
		pItem7->ApplyAttributeList(_T("width=\"20\" text=\"S\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		pHeader->Add(pItem0);
		pHeader->Add(pItem1);
		pHeader->Add(pItem2);
		pHeader->Add(pItem3);
		pHeader->Add(pItem4);
		pHeader->Add(pItem5);
		pHeader->Add(pItem6);
		pHeader->Add(pItem7);
	}
	void __FillList(DuiLib::CListUI* _pList)
	{
		DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
		IControlPlugin* pInterface = get_control_interface(pWnd);
		IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
		if (!pInterface) return;
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
		int nFont = _tstoi(strFont);
		int nDefaultFont = _tstoi(strDefaultFont);
		for (int i = nFont; i < nFont + 100; ++i)
		{
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetFontInfo(i);
			if (!pFI || pFI->hFont == __m_pControl->GetManager()->GetDefaultFontInfo()->hFont)
				continue;
			__AddFontListItem(_pList, i, nFont, pFI, i == nDefaultFont, pWSD->IsSharedFont(i));
		}
	}
	void __FillCombo(DuiLib::CComboUI* _pCombo)
	{
		DuiLib::CControlUI* pWnd = get_wnd_ptr(__m_pControl);
		DuiLib::CControlUI* pRoot = get_root_ptr(__m_pControl);
		IControlPlugin* pInterface = get_control_interface(pWnd);
		if (pInterface)
		{
			int nBeginID = _tstoi(pInterface->GetAttribute(_T("fontid")));
			int nDefaultID = _tstoi(pInterface->GetAttribute(_T("defaultfont")));
			DuiLib::CDuiString strTmp;
			for (int i = nBeginID; i < nBeginID + 100; ++i)
			{
				DuiLib::TFontInfo* pFI = pWnd->GetManager()->GetFontInfo(i);
				if (!pFI) continue;
				if (pFI->hFont == pWnd->GetManager()->GetDefaultFontInfo()->hFont) continue;
				DuiLib::CListLabelElementUI* pItem = new DuiLib::CListLabelElementUI;
				strTmp.Format(_T("%c %d %s %d %c%c%c")
					, i == nDefaultID ? _T('*') : _T(' ')
					, i - nBeginID
					, pFI->sFontName.GetData()
					, pFI->iSize
					, pFI->bBold ? _T('B') : _T(' ')
					, pFI->bItalic ? _T('I') : _T(' ')
					, pFI->bUnderline ? _T('U') : _T(' '));
				pItem->SetText(strTmp);
				strTmp.Format(_T("%d"), i);
				pItem->SetUserData(strTmp);
				_pCombo->Add(pItem);
			}
		}
	}
	void __ChangeFont()
	{
		__m_pCombo->RemoveAll();
		__FillCombo(__m_pCombo);
		__m_strValue.Empty();
		__m_pControl->NeedUpdate();
		GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
	}
	void __AddFontListItem(DuiLib::CListUI* _pList, int _nFont, int _nBeginFont, DuiLib::TFontInfo* _pFI, bool _bDefault, bool _bShared)
	{
		DuiLib::CDuiString strTmp;
		DuiLib::CListContainerElementUI* pItem = new DuiLib::CListContainerElementUI;
		pItem->ApplyAttributeList(_T("height=\"24\" inset=\"0,2,0,2\""));
		pItem->SetTag(_nFont);
		DuiLib::CHorizontalLayoutUI* pHor = new DuiLib::CHorizontalLayoutUI;
		pHor->ApplyAttributeList(_T("childpadding=\"2\""));
		DuiLib::CLabelUI* pDefault = new DuiLib::CLabelUI;
		pDefault->ApplyAttributeList(_T("width=\"20\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		pDefault->SetText(_bDefault ? _T("*") : _T(""));

		DuiLib::CLabelUI* pID = new DuiLib::CLabelUI;
		pID->ApplyAttributeList(_T("width=\"30\" textcolor=\"#FFCCCCCC\" align=\"center\" valign=\"vcenter\""));
		strTmp.Format(_T("%d"), _nFont - _nBeginFont); pID->SetText(strTmp.GetData());

		DuiLib::CComboUI* pName = new DuiLib::CComboUI;
		pName->ApplyAttributeList(_T("width=\"80\" height=\"20\" vscrollbar=\"true\" textpadding=\"2,2,2,2\"  bordersize=\"1\" bordercolor=\"#FF3F3F46\" bkcolor=\"#FF333337\" dropboxsize=\"150,300\" itemtextcolor=\"#FFCCCCCC\" itemhottextcolor=\"#FFCCCCCC\" itemselectedtextcolor=\"#FFCCCCCC\" itemhotbkcolor=\"#FF3F3F46\" itemselectedbkcolor=\"#FF007ACC\" itemtextpadding=\"5\" "));
		pName->SetDropBoxAttributeList(_T("bkcolor=\"#FF252526\" bordercolor=\"#FF3F3F46\" "));
		__FillFontName(pName);
		__SelectFontName(pName, _pFI->sFontName);
		pName->SetTag(_nFont);
		pName->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontNameChange);

		DuiLib::CRichEditUI* pSize = new DuiLib::CRichEditUI;
		pSize->ApplyAttributeList(_T("width=\"30\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		strTmp.Format(_T("%d"), _pFI->iSize); pSize->SetText(strTmp.GetData());
		pSize->SetTag(_nFont);
		pSize->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontSizeChange);

		DuiLib::COptionUI* pB = new DuiLib::COptionUI;
		pB->ApplyAttributeList(_T("tooltip=\"加粗\" width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		pB->Selected(_pFI->bBold);
		pB->SetTag(_nFont);
		pB->SetUserData(_T("bold"));
		pB->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontStyleOption);

		DuiLib::COptionUI* pI = new DuiLib::COptionUI;
		pI->ApplyAttributeList(_T("tooltip=\"斜体\" width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		pI->Selected(_pFI->bItalic);
		pI->SetTag(_nFont);
		pI->SetUserData(_T("italic"));
		pI->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontStyleOption);

		DuiLib::COptionUI* pU = new DuiLib::COptionUI;
		pU->ApplyAttributeList(_T("tooltip=\"下划线\" width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		pU->Selected(_pFI->bUnderline);
		pU->SetTag(_nFont);
		pU->SetUserData(_T("underline"));
		pU->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontStyleOption);

		DuiLib::COptionUI* pS = new DuiLib::COptionUI;
		pS->ApplyAttributeList(_T("tooltip=\"设置为共享字体\" width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		pS->Selected(_bShared);
		pS->SetTag(_nFont);
		pS->SetUserData(_T("shared"));
		pS->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Font::__OnFontStyleOption);

		pHor->Add(pDefault);
		pHor->Add(pID);
		pHor->Add(pName);
		pHor->Add(pSize);
		pHor->Add(pB);
		pHor->Add(pI);
		pHor->Add(pU);
		pHor->Add(pS);
		pItem->Add(pHor);
		_pList->Add(pItem);
	}
	void __FillFontName(DuiLib::CComboUI* _pName)
	{
		const std::vector<DuiLib::CDuiString>& vecName = get_sys_font_name();
		for (std::vector<DuiLib::CDuiString>::const_iterator it = vecName.begin(); it != vecName.end(); ++it)
		{
			DuiLib::CListLabelElementUI* pItem = new DuiLib::CListLabelElementUI;
			pItem->SetText(it->GetData());
			_pName->Add(pItem);
		}
	}
	void __SelectFontName(DuiLib::CComboUI* _pName, LPCTSTR _lpFont)
	{
		_pName->SelectItem(0);
		for (int i = 0; i < _pName->GetCount(); ++i)
		{
			if (_pName->GetItemAt(i)->GetText() == _lpFont)
			{
				_pName->SelectItem(i);
				return;
			}
		}
	}
private:
	DuiLib::CControlUI*				__m_pControl;
	DuiLib::CDuiString				__m_strAttrib;
	DuiLib::CDuiString				__m_strName;
	DuiLib::CComboUI*				__m_pCombo;
	DuiLib::CButtonUI*				__m_pManagerBtn;
	DuiLib::CDuiString				__m_strValue;
	DuiLib::CVerticalLayoutUI*		__m_pFontManagerV;
	DuiLib::CListUI*				__m_pList;
	DuiLib::CButtonUI*				__m_pBtnAdd;
	DuiLib::CButtonUI*				__m_pBtnDel;
	DuiLib::CButtonUI*				__m_pBtnDefault;
};

#endif