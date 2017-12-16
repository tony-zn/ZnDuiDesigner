#include "stdafx.h"
#include "DefaultAttrWnd.h"

CDefaultAttrWnd::CDefaultAttrWnd( DuiLib::CContainerUI* _pWnd )
	: __m_pWnd(_pWnd)
	, __m_nFont(0)
	, m_bChange(false)
{
	__m_pVirParent = new DuiLib::CVerticalLayoutUI;
	__m_pVirParent->ApplyAttributeList(_T("float=\"true\" pos=\"10000,0,11000,1000\" vscrollbar=\"true\" hscrollbar=\"true\""));
	__m_pVirTree = new DuiLib::CTreeViewUI;
	__m_pVirTree->ApplyAttributeList(_T("float=\"true\" pos=\"10000,0,11000,1000\""));
	__m_pWnd->Add(__m_pVirParent);
	__m_pWnd->Add(__m_pVirTree);
}

CDefaultAttrWnd::~CDefaultAttrWnd()
{
	__m_pWnd->Remove(__m_pVirParent);
	__m_pWnd->Remove(__m_pVirTree);
	__m_pVirParent = nullptr;
}

DuiLib::CDuiString CDefaultAttrWnd::GetSkinFolder()
{
	return g_pMainFrame->GetSkinFolder();
}

DuiLib::CDuiString CDefaultAttrWnd::GetSkinFile()
{
	return _T("DefaultAttrWnd.xml");
}

LPCTSTR CDefaultAttrWnd::GetWindowClassName( void ) const
{
	return _T("DefaultAttrWnd");
}

void CDefaultAttrWnd::InitWindow()
{
	__super::InitWindow();
	DUI_GET_CONTROL_PTR(DuiLib::CListUI					, __m_pControlList					, _T("controls.list"));
	DUI_GET_CONTROL_PTR(DuiLib::CTreeViewUI				, __m_pAttribTree					, _T("attribs.tree"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI				, __m_pFolderStyle					, _T("attribs.tree.nodestyle.folderattr"));
	__InitControls();
}

void CDefaultAttrWnd::Notify( DuiLib::TNotifyUI& msg )
{
	ON_DUI_NOTIFY(__OnControlsListSelectedChange	, _T("controls.list")						, DUI_MSGTYPE_ITEMSELECT);
	ON_DUI_NOTIFY(__OnControlsListDBClick			, _T("controls.list.item")					, DUI_MSGTYPE_ITEMACTIVATE);
	ON_DUI_NOTIFY(__OnControlsListComboChange		, _T("controls.list.item.cmbo")				, DUI_MSGTYPE_ITEMSELECT);
	ON_DUI_NOTIFY(__OnControlsListOptChange			, _T("controls.list.item.opt")				, DUI_MSGTYPE_SELECTCHANGED);
	ON_DUI_NOTIFY(__OnControlsAdd					, _T("add.btn")								, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnControlsDelete				, _T("delete.btn")							, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnChange						, _T("")									, DUI_MSGTYPE_ATTRIBCHANGE);
	__super::Notify(msg);
}

void CDefaultAttrWnd::__InitControls()
{
	__m_vecControls.push_back(STR_VSCROLL);
	__m_vecControls.push_back(STR_HSCROLL);
	for (_SETTINGS_CONTROLS::iterator it = config.Settings().vecControl.begin(); it != config.Settings().vecControl.end(); ++it)
	{
		if (it->strName == STR_WINDOW || it->strName == STR_SCROLLBAR || !it->bEnable) continue;
		__m_vecControls.push_back(it->strName);
	}
	IControlPlugin* pInterface = get_control_interface(__m_pWnd);
	if (!pInterface) Close();
	__m_nFont = _tstoi(pInterface->GetAttribute(_T("fontid")));
	for (auto it = __m_vecControls.begin(); it != __m_vecControls.end(); ++it)
	{
		DuiLib::CDuiString strAttrName;
		strAttrName.Format(_T("_%d_%s"), __m_nFont, *it);
		LPCTSTR lpAttr = __m_pWnd->GetManager()->GetDefaultAttributeList(strAttrName);
		if (lpAttr) __AddControlsItem(*it);
	}
}

void CDefaultAttrWnd::__InitAttributes()
{
	DuiLib::CControlUI* pItem = __m_pControlList->GetItemAt(__m_pControlList->GetCurSel());
	if (!pItem) return;

}

void CDefaultAttrWnd::__AddControlsItem( LPCTSTR _lpName )
{
	IWindowSharedData* pWSD = get_wnd_shared_data(__m_pWnd);
	if (!pWSD) return;
	DuiLib::CListContainerElementUI* pItem = new DuiLib::CListContainerElementUI;
	DuiLib::CDuiString strName = __m_pControlList->GetName();
	strName += _T(".item");
	pItem->SetName(strName);
	pItem->SetFixedHeight(30);
	DuiLib::CHorizontalLayoutUI* pHor = new DuiLib::CHorizontalLayoutUI;
	pHor->ApplyAttributeList(_T("inset=\"5,5,0,5\" mouse=\"false\""));
	DuiLib::COptionUI* pOpt = new DuiLib::COptionUI;
	pOpt->SetName(strName + _T(".opt"));
	pOpt->SetTag(reinterpret_cast<UINT_PTR>(pItem));
	pOpt->ApplyAttributeList(_T("tooltip=\"是否设置为共享数据\" width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
	DuiLib::CDuiString strAttrName;
	strAttrName.Format(_T("_%d_%s"), __m_nFont, _lpName);
	bool bShared = pWSD->IsSharedDefault(strAttrName);
	pOpt->Selected(bShared);
	pItem->SetUserData(bShared ? _T("shared") : _T(""));
	DuiLib::CComboUI* pCmbo = new DuiLib::CComboUI;
	pCmbo->SetName(strName + _T(".cmbo"));
	pCmbo->ApplyAttributeList(_T("width=\"150\" height=\"20\" mouse=\"false\" vscrollbar=\"true\" textpadding=\"2,2,2,2\"  bordersize=\"1\" bordercolor=\"#FF3F3F46\" dropboxsize=\"150,300\" itemtextcolor=\"#FFCCCCCC\" itemhottextcolor=\"#FFCCCCCC\" itemselectedtextcolor=\"#FFCCCCCC\" itemhotbkcolor=\"#FF3F3F46\" itemselectedbkcolor=\"#FF007ACC\" itemtextpadding=\"5\" "));
	pCmbo->SetDropBoxAttributeList(_T("bkcolor=\"#FF252526\" bordercolor=\"#FF3F3F46\" "));
	pCmbo->SetTag(reinterpret_cast<UINT_PTR>(pItem));
	for (auto it = __m_vecControls.begin(); it != __m_vecControls.end(); ++it)
	{
		DuiLib::CListLabelElementUI* pCmboItem = new DuiLib::CListLabelElementUI;
		pCmbo->Add(pCmboItem);
		pCmboItem->SetText(*it);
		pCmboItem->Select(*it == _lpName);
	}
	pHor->Add(pOpt);
	pHor->Add(pCmbo);
	pItem->Add(pHor);
	__ResetControlsItem(pItem);
	__m_pControlList->Add(pItem);
}

void CDefaultAttrWnd::__ResetControlsItem( DuiLib::CControlUI* _pItem )
{
	DuiLib::CComboUI* pCmbo = __GetControlsItemCmbo(_pItem);
	if (!pCmbo) return;
	DuiLib::CDuiString strName = pCmbo->GetText();
	DuiLib::CControlUI* pControl = reinterpret_cast<DuiLib::CControlUI*>(_pItem->GetTag());
	if (pControl && strName != STR_VSCROLL && strName != STR_HSCROLL)
	{
		__m_pVirParent->Remove(pControl);
	}
	pControl = nullptr;
	_pItem->SetTag(0);

	DuiLib::CDuiString strAttrName;
	strAttrName.Format(_T("_%d_%s"), __m_nFont, strName);
	LPCTSTR lpAttr = __m_pWnd->GetManager()->GetDefaultAttributeList(strAttrName);
	if (strName == STR_VSCROLL)
	{
		if (lpAttr) __m_pVirParent->GetVerticalScrollBar()->ApplyAttributeList(lpAttr);
		_pItem->SetTag(reinterpret_cast<UINT_PTR>(__m_pVirParent->GetVerticalScrollBar()));
	}
	else if (strName == STR_HSCROLL)
	{
		if (lpAttr) __m_pVirParent->GetHorizontalScrollBar()->ApplyAttributeList(lpAttr);
		_pItem->SetTag(reinterpret_cast<UINT_PTR>(__m_pVirParent->GetHorizontalScrollBar()));
	}
	else
	{
		DuiLib::CDuiString strControl;
		strControl.Format(_T("<Window><%s /></Window>"), strName);
		DuiLib::CDialogBuilder builder;
		if (strName == STR_TREENODE)
		{
			DuiLib::CTreeNodeUI* pNode = new DuiLib::CTreeNodeUI;
			__m_pVirTree->Add(pNode);
			pControl = pNode;
		}
		else
			pControl = builder.Create(strControl.GetData(), NULL, g_pMainFrame, __m_pVirParent->GetManager(), __m_pVirParent);
		if (pControl) 
		{
			if (lpAttr) pControl->ApplyAttributeList(lpAttr);
			pControl->SetManager(__m_pVirParent->GetManager(), __m_pVirParent);
		}
		_pItem->SetTag(reinterpret_cast<UINT_PTR>(pControl));
	}
}

DuiLib::CComboUI* CDefaultAttrWnd::__GetControlsItemCmbo( DuiLib::CControlUI* _pItem )
{
	DuiLib::CContainerUI* pItemContainer = static_cast<DuiLib::CContainerUI*>(_pItem->GetInterface(DUI_CTR_CONTAINER));
	if (!pItemContainer) return nullptr;
	DuiLib::CControlUI* pHor = pItemContainer->GetItemAt(0);
	if (!pHor) return nullptr;
	DuiLib::CContainerUI* pHorLayout = static_cast<DuiLib::CContainerUI*>(pHor->GetInterface(DUI_CTR_CONTAINER));
	if (!pHorLayout) return nullptr;
	DuiLib::CControlUI* pHorItem = pHorLayout->GetItemAt(1);
	if (!pHorItem) return nullptr;
	return static_cast<DuiLib::CComboUI*>(pHorItem->GetInterface(DUI_CTR_COMBO));
}

void CDefaultAttrWnd::__OnControlsListSelectedChange( DuiLib::TNotifyUI& msg )
{
	for (int i = 0; i < __m_pControlList->GetCount(); ++i)
	{
		DuiLib::CComboUI* pCmbo = __GetControlsItemCmbo(__m_pControlList->GetItemAt(i));
		if (!pCmbo) continue;
		pCmbo->SetMouseEnabled(false);
		pCmbo->SetBkColor(0x00000000);
	}
	__ResetAttribs();
}

void CDefaultAttrWnd::__FillAttribTree( DuiLib::CControlUI* _pControl, _SETTINGS_CONTROL* _pSC )
{
	if (!_pSC->strParent.IsEmpty())
	{
		_SETTINGS_CONTROL* pParent = config.GetControl(_pSC->strParent);
		if (pParent) __FillAttribTree(_pControl, pParent);
	}
	if (_pSC->vecAttrib.empty()) return;
	DuiLib::CTreeNodeUI* pNode = new DuiLib::CTreeNodeUI;
	pNode->GetFolderButton()->ApplyAttributeList(__m_pFolderStyle->GetUserData());
	pNode->SetItemText(_pSC->strName);
	for (_SETTINGS_ATTRIBS::iterator it = _pSC->vecAttrib.begin(); it != _pSC->vecAttrib.end(); ++it)
	{
		DuiLib::CControlUI* pAttrib = plugin_attrib.CreateAttrib(it->strType, _pControl, it->strName, it->strAlias);
		if (pAttrib)
		{
			DuiLib::CTreeNodeUI* pNodeAttrib = new DuiLib::CTreeNodeUI;
			pNodeAttrib->SetFixedHeight(pAttrib->GetFixedHeight());
			pNodeAttrib->GetItemButton()->SetVisible(false);
			pNodeAttrib->GetFolderButton()->SetFixedWidth(1);
			pNodeAttrib->GetFolderButton()->SetEnabled(false);
			pNodeAttrib->GetTreeNodeHoriznotal()->Add(pAttrib);
			pNodeAttrib->SetMouseEnabled(false);
			pNode->Add(pNodeAttrib);
		}
	}
	__m_pAttribTree->Add(pNode);
}

void CDefaultAttrWnd::__ResetAttribs()
{
	__m_pAttribTree->RemoveAll();
	DuiLib::CControlUI* pItem = __m_pControlList->GetItemAt(__m_pControlList->GetCurSel());
	if (!pItem) return;
	DuiLib::CControlUI* pControl = reinterpret_cast<DuiLib::CControlUI*>(pItem->GetTag());
	if (!pControl) return;
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(pControl->GetClass());
	if (!pSC) return;
	__FillAttribTree(pControl, pSC);
	__m_pAttribTree->ApplyAttributeList(__m_pAttribTree->GetUserData());
}

void CDefaultAttrWnd::__OnControlsListDBClick( DuiLib::TNotifyUI& msg )
{
	for (int i = 0; i < __m_pControlList->GetCount(); ++i)
	{
		DuiLib::CComboUI* pCmbo = __GetControlsItemCmbo(__m_pControlList->GetItemAt(i));
		if (!pCmbo) continue;
		if (i == __m_pControlList->GetCurSel())
		{
			pCmbo->SetBkColor(0xFF252526);
			pCmbo->SetMouseEnabled(true);
		}
		else
		{
			pCmbo->SetBkColor(0);
			pCmbo->SetMouseEnabled(false);
		}
	}
}

void CDefaultAttrWnd::__OnControlsListComboChange( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pItem = __m_pControlList->GetItemAt(__m_pControlList->GetCurSel());
	if (!pItem) return;
	__ResetControlsItem(pItem);
	__ResetAttribs();
	m_bChange = true;
}

void CDefaultAttrWnd::__OnControlsAdd( DuiLib::TNotifyUI& msg )
{
	__AddControlsItem(_T("Control"));
	__m_pControlList->SelectItem(__m_pControlList->GetCount());
	__m_pControlList->SetPos(__m_pControlList->GetPos());
	__m_pControlList->GetVerticalScrollBar()->SetScrollPos(__m_pControlList->GetVerticalScrollBar()->GetScrollRange());
	m_bChange = true;
}

void CDefaultAttrWnd::__OnControlsDelete( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pItem = __m_pControlList->GetItemAt(__m_pControlList->GetCurSel());
	if (!pItem) return;
	__m_pControlList->SelectItem(0);
	__ResetAttribs();
	DuiLib::CControlUI* pControl = reinterpret_cast<DuiLib::CControlUI*>(pItem->GetTag());
	if (pControl) __m_pVirParent->Remove(pControl);
	__m_pControlList->Remove(pItem);
	m_bChange = true;
}

void CDefaultAttrWnd::__SaveDefaultAttr()
{
	IWindowSharedData* pWSD = get_wnd_shared_data(__m_pWnd);
	if (!pWSD) return;
	for (auto it = __m_vecControls.begin(); it != __m_vecControls.end(); ++it)
	{
		DuiLib::CDuiString strAttrName;
		strAttrName.Format(_T("_%d_%s"), __m_nFont, *it);
		__m_pWnd->GetManager()->RemoveDefaultAttributeList(strAttrName);
		pWSD->SetSharedDefault(strAttrName, false);
	}
	for (int i = 0; i < __m_pControlList->GetCount(); ++i)
	{
		DuiLib::CControlUI* pItem = __m_pControlList->GetItemAt(i);
		DuiLib::CControlUI* pControl = reinterpret_cast<DuiLib::CControlUI*>(pItem->GetTag());
		if (!pControl) continue;
		_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(pControl->GetClass());
		if (!pSC) continue;
		_SETTINGS_CONTROLS vecControls;
		config.GetAttribs(pSC, vecControls);
		IControlPlugin* pInterface = get_control_interface(pControl);
		if (!pInterface) continue;
		_ZnSStream ss;
		pControl->SetInternVisible(true);
		for (_SETTINGS_CONTROLS::iterator itControl = vecControls.begin(); itControl != vecControls.end(); ++itControl)
		{
			for (_SETTINGS_ATTRIBS::iterator it = itControl->vecAttrib.begin(); it != itControl->vecAttrib.end(); ++it)
			{
				DuiLib::CDuiString strValue = pInterface->GetAttribute(it->strName);
				if (strValue == it->strDefaultValue) continue;
				if (it->strName == _T("font") || it->strName == _T("itemfont"))
				{
					IControlPlugin* pWndInterface = get_control_interface(__m_pWnd);
					DuiLib::CDuiString strFont = pWndInterface->GetAttribute(_T("fontid"));
					DuiLib::CDuiString strDefaultFont = pWndInterface->GetAttribute(_T("defaultfont"));
					int nFontID = _tstoi(strFont);
					int nDefaultFont = _tstoi(strDefaultFont);
					int nFont = _tstoi(strValue) - nFontID;
					int nRealDefaultFont = nDefaultFont - nFontID;
					if (nFont < 0) nFont = -1;
					if (nRealDefaultFont < 0) nRealDefaultFont = -1;
					if (nFont == -1 || nFont == nRealDefaultFont) continue;
					strFont.Format(_T("%d"), nFont);
					ss << it->strName.GetData() << _T("=\"") << strFont.GetData() << _T("\" ");
				}
				else
					ss << it->strName.GetData() << _T("=\"") << strValue.GetData() << _T("\" ");
			}
		}
		if (!ss.str().empty())
		{
			DuiLib::CDuiString strAttrName;
			if (pSC->strName == STR_SCROLLBAR)
			{
				DuiLib::CScrollBarUI* pScrollBar = static_cast<DuiLib::CScrollBarUI*>(pControl->GetInterface(DUI_CTR_SCROLLBAR));
				strAttrName.Format(_T("_%d_%s"), __m_nFont, pScrollBar->IsHorizontal() ? STR_HSCROLL : STR_VSCROLL);
			}
			else
				strAttrName.Format(_T("_%d_%s"), __m_nFont, pSC->strName);
			__m_pWnd->GetManager()->AddDefaultAttributeList(strAttrName, ss.str().c_str());
			pWSD->SetSharedDefault(strAttrName, pItem->GetUserData() == _T("shared"));
		}
	}
}

void CDefaultAttrWnd::__OnControlsListOptChange( DuiLib::TNotifyUI& msg )
{
	DuiLib::COptionUI* pOpt = static_cast<DuiLib::COptionUI*>(msg.pSender->GetInterface(DUI_CTR_OPTION));
	if (!pOpt) return;
	DuiLib::CControlUI* pItem = reinterpret_cast<DuiLib::CControlUI*>(pOpt->GetTag());
	if (!pItem) return;
	pItem->SetUserData(pOpt->IsSelected() ? _T("shared") : _T(""));
	m_bChange = true;
}

void CDefaultAttrWnd::__OnChange( DuiLib::TNotifyUI& msg )
{
	m_bChange = true;
}

LRESULT CDefaultAttrWnd::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__SaveDefaultAttr();
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

