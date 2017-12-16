#include "stdafx.h"
#include "MainFrame.h"

CMainFrame::CMainFrame(LPCTSTR _lpSkinPath)
	: __m_bIsInit(false)
	, __m_strSkinPath(_lpSkinPath)
	, __m_byOperation(OP_Normal)
	, __m_bLBtnDown(false)
	, __m_bCtrlDown(false)
	, __m_bMove(false)
	, __m_bSize(false)
{}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return __m_strSkinPath.GetData();
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("MainFrame.xml");
}

LPCTSTR CMainFrame::GetWindowClassName( void ) const
{
	return _T("MainFrame");
}

void CMainFrame::InitWindow()
{
	__super::InitWindow();
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTitle					, _T("title"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pRestoreBtn				, _T("restorebtn"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pMaxBtn					, _T("maxbtn"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pMenu						, _T("menu.v"));
	DUI_GET_CONTROL_PTR(DuiLib::CContainerUI		, __m_pWndsTitle				, _T("wnds.title.h"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pWndsTitleBorder			, _T("wnds.title.border"));
	DUI_GET_CONTROL_PTR(DuiLib::CTabLayoutUI		, __m_pWndsTab					, _T("wnds.content.tab"));
	DUI_GET_CONTROL_PTR(DuiLib::CTreeViewUI			, __m_pAttribs					, _T("attribs.tree"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pAttribsFolderStyle		, _T("attribs.tree.nodestyle.folderattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pAttribsTitle				, _T("attribs.title"));
	DUI_GET_CONTROL_PTR(DuiLib::CTabLayoutUI		, __m_pControlsTab				, _T("controls.tab"));
	DUI_GET_CONTROL_PTR(DuiLib::CListUI				, __m_pControls					, _T("controls.list"));
	DUI_GET_CONTROL_PTR(DuiLib::CTreeViewUI			, __m_pControlsLayoutTree		, _T("controls.layout.tree"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pControlsLayoutFolderStyle, _T("controls.layout.tree.nodestyle.folderattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pPointedStyle				, _T("pointedstyle"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pSelectedStyle			, _T("selectedstyle"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnUnDo			, _T("toolbar.undo"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnRedo			, _T("toolbar.redo"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnSave			, _T("toolbar.save"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnSaveAs			, _T("toolbar.saveas"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnStart			, _T("toolbar.start"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnCut			, _T("toolbar.cut"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnCopy			, _T("toolbar.copy"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnPaste			, _T("toolbar.paste"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnDel			, _T("toolbar.del"));
	DUI_GET_CONTROL_PTR(DuiLib::CButtonUI			, __m_pToolbarBtnDefaultData	, _T("toolbar.defaultdata"));
	
	::SetWindowText(GetHWND(), __m_pTitle->GetText().GetData());
	__m_bIsInit = true;
	m_PaintManager.SetTimer(m_PaintManager.GetRoot(), 1, 200);
}

LRESULT CMainFrame::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (__m_bIsInit)
	{
		bool bMax = !!::IsZoomed(GetHWND());
		__m_pRestoreBtn->SetVisible(bMax);
		__m_pMaxBtn->SetVisible(!bMax);
		::PostMessage(GetHWND(), MYWM_SELECTEDCHANGE, NULL, NULL);
	}
	return __super::OnSize(uMsg, wParam, lParam, bHandled);
}

DuiLib::CControlUI* CMainFrame::CreateControl( LPCTSTR pstrClass )
{
	return plugin_control.CreateControl(pstrClass);
}

void CMainFrame::Notify( DuiLib::TNotifyUI& msg )
{
	for (DuiLib::CControlUI* pC = msg.pSender; pC; pC = pC->GetParent())
		if (pC->GetInterface(STR_WINDOW)) return;

	ON_DUI_NOTIFY(__OnMenu					, _T("menubtn")												, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnMenuSettings			, _T("settingsbtn")											, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnMenuAbout				, _T("aboutbtn")											, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnNewWnd				, _T("toolbar.new")											, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnDeleteWnd				, _T("wnds.title.h.item.manager.del.btn")					, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnSave					, _T("toolbar.save")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnSaveAs				, _T("toolbar.saveas")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnOpen					, _T("toolbar.open")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnCut					, _T("toolbar.cut")											, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnCopy					, _T("toolbar.copy")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnPaste					, _T("toolbar.paste")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnDel					, _T("toolbar.del")											, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnPreview				, _T("toolbar.start")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnDefaultData			, _T("toolbar.defaultdata")									, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnUnDo					, _T("toolbar.undo")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnReDo					, _T("toolbar.redo")										, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnLoadContols			, _T("root")												, DUI_MSGTYPE_WINDOWINIT);
	ON_DUI_NOTIFY(__OnSelectWnd				, _T("wnds.title.h.item.option")							, DUI_MSGTYPE_SELECTCHANGED);
	ON_DUI_NOTIFY(__OnResetAttribs			, _T("wnds.content.tab")									, DUI_MSGTYPE_TABSELECT);
	ON_DUI_NOTIFY(__OnUpdateTimer			, _T("root")												, DUI_MSGTYPE_TIMER);
	ON_DUI_NOTIFY(__OnAttribChange			, _T("")													, DUI_MSGTYPE_ATTRIBCHANGE);
	ON_DUI_NOTIFY(__OnControlsType			, _T("controls.type")										, DUI_MSGTYPE_SELECTCHANGED);
	ON_DUI_NOTIFY(__OnLayoutSelected		, _T("controls.layout.tree")								, DUI_MSGTYPE_ITEMSELECT);
	__super::Notify(msg);
}

LRESULT CMainFrame::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		__MouseDown(m_PaintManager.GetMousePos());
		__Select();
		__Add();
	case WM_NCLBUTTONDOWN:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			AutoHide(_T("menu.v"), pt);
		}
		break;
	case WM_LBUTTONUP:
		__MouseUp();
		break;
	case WM_MOUSEMOVE:
		__Point();
		__Move();
		__Size();
		break;
	case WM_SETCURSOR:
		if (!__SetCursor()) return NULL;
		break;
	case WM_KEYDOWN:
		__ShortcutKey(wParam, true);
		break;
	case WM_KEYUP:
		__ShortcutKey(wParam, false);
		break;
	case WM_CLOSE:
		__Close();
		return NULL;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::ResponseDefaultKeyEvent( WPARAM wParam )
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		__MouseUp();
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::__OnMenu( DuiLib::TNotifyUI& msg )
{
	RECT rcDst = msg.pSender->GetPos();
	RECT rcMenu = {rcDst.left, 
		rcDst.bottom,
		rcDst.left + __m_pMenu->GetFixedWidth(), 
		rcDst.bottom + __m_pMenu->GetFixedHeight()};
	__m_pMenu->SetPos(rcMenu);
	__m_pMenu->SetVisible(true);
}

void CMainFrame::__OnMenuSettings( DuiLib::TNotifyUI& msg )
{
	CSettingsWnd dlg;
	dlg.Create(GetHWND(), NULL, UI_WNDSTYLE_DIALOG, NULL);
	dlg.CenterWindow();
	dlg.ShowModal();
	__OnLoadContols(msg);
	__OnResetAttribs(msg);
}

void CMainFrame::__OnMenuAbout( DuiLib::TNotifyUI& msg )
{
	CAboutWnd dlg;
	dlg.Create(GetHWND(), NULL, UI_WNDSTYLE_DIALOG, NULL);
	dlg.CenterWindow();
	dlg.ShowModal();
}

void CMainFrame::__OnNewWnd( DuiLib::TNotifyUI& msg )
{
	__NewWnd();
}

void CMainFrame::__OnLoadContols( DuiLib::TNotifyUI& msg )
{
	__m_pControls->RemoveAll();
	__AddControlsItem(_T("file='Controls/point.png' dest='3,3,17,17'"), _T("Point"));
	for (_SETTINGS_CONTROLS::iterator it = config.Settings().vecControl.begin(); it != config.Settings().vecControl.end(); ++it)
		if (it->bEnable && it->strName != STR_WINDOW && it->strName != STR_SCROLLBAR) 
			__AddControlsItem(it->strImage, it->strName);
}

void CMainFrame::__OnSelectWnd( DuiLib::TNotifyUI& msg )
{
	__m_pWndsTab->SelectItem(reinterpret_cast<DuiLib::CControlUI*>(msg.pSender->GetTag()));
	for (int i = 0; i < __m_pWndsTitle->GetCount(); ++i)
	{
		DuiLib::CContainerUI* pItem = static_cast<DuiLib::CContainerUI*>(__m_pWndsTitle->GetItemAt(i)->GetInterface(DUI_CTR_CONTAINER));
		DuiLib::CControlUI* pManager = pItem->FindSubControl(_T("wnds.title.h.item.manager"));
		pManager->SetVisible(__m_pWndsTitle->GetItemAt(i)->GetTag() == msg.pSender->GetTag());
	}
}

void CMainFrame::__OnResetAttribs( DuiLib::TNotifyUI& msg )
{
	__ResetAttribs();
}

void CMainFrame::__AddControlsItem( LPCTSTR _lpImage, LPCTSTR _lpControl, int nPos /*= -1*/ )
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pItem = builder.Create(_T("ControlsListItem.xml"), nullptr, this, &m_PaintManager);
	DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(pItem->GetInterface(DUI_CTR_CONTAINER));
	if (pContainer)
	{
		pContainer->FindSubControl(_T("controls.list.item.image"))->SetBkImage(_lpImage);
		pContainer->FindSubControl(_T("controls.list.item.name"))->SetText(_lpControl);
		pItem->SetUserData(_lpControl);
	}
	if (nPos == -1)
		__m_pControls->Add(pItem);
	else
		__m_pControls->AddAt(pItem, nPos);

}

DuiLib::CControlUI* CALLBACK FindControlFromPt(DuiLib::CControlUI* _pControl, LPVOID _pPt)
{
	POINT* pPt = reinterpret_cast<POINT*>(_pPt);
	if (pPt && _pControl->IsVisible() && 
		// CListBodyUI不选中，原库中没有判断是否是CListBodyUI的方法，只能这样判断
		!(_pControl->GetInterface(DUI_CTR_VERTICALLAYOUT) && _pControl->GetParent() && _pControl->GetParent()->GetInterface(DUI_CTR_LIST)) 
		&& ::PtInRect(&_pControl->GetPos(), *pPt)) return _pControl;
	return nullptr;
}

void CMainFrame::__Point()
{
	if ((__m_byOperation != OP_Normal || __m_byOperation != OP_Add) && __m_bLBtnDown) return;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	DuiLib::CControlUI* pPointed = pCavas->GetItemAt(1);
	if (!pWnd || !pPointed) return;
	POINT pt = m_PaintManager.GetMousePos();
	if (::PtInRect(&pCavas->GetPos(), pt))
	{
		DuiLib::CControlUI* pPoint = pWnd->FindControl(FindControlFromPt, &pt, NULL);
		if (pPoint && pPoint != pWnd)
		{
			pPointed->SetPos(__CalRect(pPoint, pCavas));
			pPointed->SetVisible(true);
			pPointed->SetTag(reinterpret_cast<UINT_PTR>(pPoint));
			return;
		}
	}
	pPointed->SetVisible(false);
	
}

void CMainFrame::__Select()
{
	if (__m_byOperation != OP_Normal) return;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) 
		return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	DuiLib::CControlUI* pPointed = pCavas->GetItemAt(1);
	if (!pWnd || !pPointed) 
		return;
	POINT pt = m_PaintManager.GetMousePos();
	if (::PtInRect(&pCavas->GetPos(), pt))
	{
		UINT_PTR tag = pCavas->GetTag();
		if (pPointed->IsVisible())
			pCavas->SetTag(pPointed->GetTag());
		else
			pCavas->SetTag(0);
	}
	__RefreshSelect();
}

RECT CMainFrame::__CalRect( DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pParent )
{
	if (!_pControl || !_pParent) return RECT();
	RECT rc = _pControl->GetPos();
	RECT rcParent = _pParent->GetPos();
	::OffsetRect(&rc, -rcParent.left, -rcParent.top);
	rc.left -= 2;
	rc.top -= 2;
	rc.right += 2;
	rc.bottom += 2;
	return rc;
}

bool CMainFrame::__SetCursor()
{
	__m_byOperation = OP_Normal;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return true;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return true;
	RECT rc = pCavas->GetPos();
	if (pCavas->GetHorizontalScrollBar() && pCavas->GetHorizontalScrollBar()->IsVisible()) 
		rc.bottom -= pCavas->GetHorizontalScrollBar()->GetFixedHeight();
	if (pCavas->GetVerticalScrollBar() && pCavas->GetVerticalScrollBar()->IsVisible()) 
		rc.right -= pCavas->GetVerticalScrollBar()->GetFixedWidth();
	POINT pt = m_PaintManager.GetMousePos();
	if (!::PtInRect(&rc, pt)) return true;

	if (__m_pControls->GetCurSel() > 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
		__m_byOperation = OP_Add;
		return false;
	}
	DuiLib::CControlUI* pSelected = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSelected) return true;
	const int nBorderSize = 5;
	DuiLib::CControlUI* pSelect = pCavas->GetItemAt(2);
	if (pSelect)
	{
		RECT rcSelect = pSelect->GetPos();
		if (::PtInRect(&rcSelect, pt))
		{
			if (rcSelect.right - pt.x < nBorderSize)
				__m_byOperation |= OP_Size_Right;
			else if (pt.x - rcSelect.left < nBorderSize)
				__m_byOperation |= OP_Size_Left;
			if (rcSelect.bottom - pt.y < nBorderSize)
				__m_byOperation |= OP_Size_Bottom;
			else if (pt.y - rcSelect.top < nBorderSize)
				__m_byOperation |= OP_Size_Top;
			if (!pSelected->IsFloat())
				__m_byOperation &= ~(OP_Size_Left | OP_Size_Top);
			switch (__m_byOperation)
			{
			case OP_Size_Left:
			case OP_Size_Right:
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
				return false;
			case OP_Size_Top:
			case OP_Size_Bottom:
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
				return false;
			case OP_Size_LeftTop:
			case OP_Size_RightBottom:
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
				return false;
			case OP_Size_RightTop:
			case OP_Size_LeftBottom:
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
				return false;
			}
		}
	}
	if (pSelected->IsFloat())
	{
		if (!__m_bLBtnDown && ::PtInRect(&pSelected->GetPos(), pt) || __m_bLBtnDown)
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
			__m_byOperation = OP_Move;
			return false;
		}
	}
	return true;
}

void CMainFrame::__Add()
{
	if (__m_byOperation != OP_Add) return;
	if (__m_pControls->GetCurSel() <= 0) return;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	DuiLib::CDuiString strControl, strXml;
	strControl = __m_pControls->GetItemAt(__m_pControls->GetCurSel())->GetUserData();
	strXml.Format(_T("<Window><%s /></Window>"), strControl);
	__m_pControls->SelectItem(0);
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	DuiLib::CContainerUI* pWndLayout = static_cast<DuiLib::CContainerUI*>(pWnd->GetInterface(DUI_CTR_CONTAINER));
	if (!pWndLayout) return;
	DuiLib::CControlUI* pPointed = pCavas->GetItemAt(1);
	if (!pWnd || !pPointed) return;
	DuiLib::CControlUI* pParent = pWnd;
	if (pPointed->IsVisible() && pPointed->GetTag())
		pParent = reinterpret_cast<DuiLib::CControlUI*>(pPointed->GetTag());
	if (!pParent && pWndLayout->GetCount() == 0) pParent = pWnd;
	if (!pParent) return;
	if (!pParent->GetInterface(DUI_CTR_CONTAINER)) pParent = pParent->GetParent();
	if (!pParent) return;
	if (strControl == STR_TREENODE)
	{
		if (!pParent->GetInterface(DUI_CTR_TREENODE) && !pParent->GetInterface(DUI_CTR_TREEVIEW))
		{
			if (pParent->GetParent()->GetInterface(DUI_CTR_TREENODE) || pParent->GetParent()->GetInterface(DUI_CTR_TREEVIEW))
				pParent = pParent->GetParent();
			else
				pParent = nullptr;
		}
		if (!pParent) return out_error(_T("TreeNode 只能在TreeNode或TreeView中添加！"));
	}
	else if (strControl == STR_LISTHEADER || strControl == STR_LISTELEMENT || strControl == STR_LISTLABELELEMENT || 
		strControl == STR_LISTTEXTELEMENT || strControl == STR_LISTCONTAINERELEMENT)
	{
		if (!pParent->GetInterface(DUI_CTR_LIST))
			return out_error(_T("该控件只能在List中添加!"));
	}
	else if (strControl == STR_LISTHEADERITEM)
	{
		if (!pParent->GetInterface(DUI_CTR_LISTHEADER))
			return out_error(_T("该控件只能在ListHeader中添加!"));
	}
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pNew = nullptr;
	pNew = builder.Create(strXml.GetData(), NULL, this, &m_PaintManager, pParent);
	if (!pNew) return;
	pNew->SetManager(&m_PaintManager, pParent);
	__m_FileManager.SetDefaultAndFont(pNew, pWnd, &m_PaintManager);
	DuiLib::CControlUI* pSelected = pCavas->GetItemAt(2);
	if (!pSelected) return;
	pSelected->SetPos(__CalRect(pNew, pCavas));
	pSelected->SetVisible(true);
	pCavas->SetTag(reinterpret_cast<UINT_PTR>(pNew));
	__ResetAttribs();
	__RefreshSelect();
	__SetEditFlag(__m_pWndsTab->GetCurSel(), true);
}

void CMainFrame::__ResetAttribs()
{
	__m_pAttribs->RemoveAll();
	if (__m_pWndsTab->GetCount() == 0) return;
	DuiLib::CControlUI* pControl = reinterpret_cast<DuiLib::CControlUI*>(__m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel())->GetTag());
	if (!pControl)
	{
		DuiLib::CControlUI* pItem = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
		DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pItem->GetInterface(DUI_CTR_CONTAINER));
		if (pCavas) pControl = pCavas->GetItemAt(0);
		if (!pControl) return;
	}
	_SETTINGS_ATTRIBS vecAttribs;
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(pControl->GetClass());
	if (!pSC) 
		__m_pAttribsTitle->SetText(__m_pAttribsTitle->GetUserData());
	else
		__m_pAttribsTitle->SetText(pSC->strName + _T(" ") + __m_pAttribsTitle->GetUserData());
	__FillAttribTree(pControl, pSC);
	__m_pAttribs->ApplyAttributeList(__m_pAttribs->GetUserData());
}

void CMainFrame::__RefreshSelect()
{
	if (__m_byOperation != OP_Normal && __m_bLBtnDown) return;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) 
		return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	DuiLib::CControlUI* pSelected = pCavas->GetItemAt(2);
	if (!pCavas || !pSelected) 
		return;
	int nTag = pCavas->GetTag();
	DuiLib::CControlUI* pCur = reinterpret_cast<DuiLib::CControlUI*>(nTag);
	if (nTag == 0) 
	{
		nTag = reinterpret_cast<UINT_PTR>(pWnd);
		pCur = pWnd;
	}
	RECT rc = __CalRect(pCur, pCavas);
	RECT rcSelected = pSelected->GetPos();
	RECT rcParent = pSelected->GetParent()->GetPos();
	::OffsetRect(&rcSelected, -rcParent.left, -rcParent.top);
	pSelected->SetVisible(pCur != pWnd);
	if (pSelected->IsVisible() && memcmp(&rcSelected, &rc, sizeof(RECT)) != 0)
		pSelected->SetPos(rc);
	if (nTag != pSelected->GetTag())
	{
		pSelected->SetTag(nTag);
		__ResetAttribs();
		__RefreshControlsLayout();
	}
}

void CMainFrame::__OnUpdateTimer( DuiLib::TNotifyUI& msg )
{
	__RefreshSelect();
	__UpdateToolBar();
}

void CMainFrame::__OnDeleteWnd( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pItem = reinterpret_cast<DuiLib::CControlUI*>(msg.pSender->GetTag());
	if (pItem)
	{
		DuiLib::CControlUI* pWndItem = reinterpret_cast<DuiLib::CControlUI*>(pItem->GetTag());
		int nItem = __m_pWndsTitle->GetItemIndex(pItem);
		if (__IsEdit(nItem))
		{
			int nRet = ::MessageBox(GetHWND(), 
				_T("当前文档未保存需要保存吗?"),
				_T("关闭文档"),
				MB_YESNOCANCEL);
			switch(nRet)
			{
			case IDYES:
				if (!__Save(nItem))
				{
					::MessageBox(GetHWND(), 
						_T("保存失败!"),
						_T("保存文档"),
						MB_OK);
					return;
				}
				break;
			case IDCANCEL:
				return;
				break;
			}

		}
		DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pWndItem->GetInterface(DUI_CTR_CONTAINER));
		if (pCavas && pCavas->GetItemAt(0))
		{
			DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
			IControlPlugin* pInterface = get_control_interface(pWnd);
			int nID = 0;
			if (pInterface) nID =  _tstoi(pInterface->GetAttribute(_T("fontid")));
			if (nID >= 1000) memorandum.Delete(nID);
			__m_FileManager.DeleteAllDefaultAndFont(pCavas->GetItemAt(0), &m_PaintManager);
		}
		__m_pWndsTitle->RemoveAt(nItem);
		__m_pWndsTab->Remove(pWndItem);
		__ResetAttribs();
		DuiLib::CControlUI* pNext = __m_pWndsTitle->GetItemAt(__m_pWndsTab->GetCurSel());
		if (pNext)
		{
			DuiLib::CContainerUI* pNextItem = static_cast<DuiLib::CContainerUI*>(pNext->GetInterface(DUI_CTR_CONTAINER));
			if (pNextItem)
			{
				DuiLib::COptionUI* pOption = static_cast<DuiLib::COptionUI*>(pNextItem->FindSubControl(_T("wnds.title.h.item.option")));
				if (pOption) pOption->Selected(true);
			}
		}
	}
	__m_pWndsTitleBorder->SetVisible(__m_pWndsTitle->GetCount() != 0);
	__RefreshControlsLayout();
}

void CMainFrame::__OnSave( DuiLib::TNotifyUI& msg )
{
	__Save(__m_pWndsTab->GetCurSel());
}

void CMainFrame::__SetEditFlag( int _nPos, bool _bEdit )
{
	DuiLib::CControlUI* pItem = __m_pWndsTitle->GetItemAt(_nPos);
	if (pItem)
	{
		DuiLib::CContainerUI* pItemContainer = static_cast<DuiLib::CContainerUI*>(pItem->GetInterface(DUI_CTR_CONTAINER));
		if (pItemContainer)
		{
			DuiLib::CControlUI* pEditFlag = pItemContainer->FindSubControl(_T("wnds.title.h.item.editflag"));
			if (pEditFlag) pEditFlag->SetVisible(_bEdit);
			if (_bEdit) 
			{
				__RefreshControlsLayout();
				__WriteMemorandum(_nPos);
			}
		}
	}
}

bool CMainFrame::__IsEdit( int _nPos )
{
	DuiLib::CControlUI* pItem = __m_pWndsTitle->GetItemAt(_nPos);
	if (pItem)
	{
		DuiLib::CContainerUI* pItemContainer = static_cast<DuiLib::CContainerUI*>(pItem->GetInterface(DUI_CTR_CONTAINER));
		if (pItemContainer)
		{
			DuiLib::CControlUI* pEditFlag = pItemContainer->FindSubControl(_T("wnds.title.h.item.editflag"));
			if (pEditFlag)
				return pEditFlag->IsVisible();
		}
	}
	return false;
}

void CMainFrame::__DelCurControl()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	DuiLib::CControlUI* pPointed = pCavas->GetItemAt(1);
	DuiLib::CControlUI* pSelected = pCavas->GetItemAt(1);
	DuiLib::CControlUI* pDeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (pDeleted && pDeleted != pWnd)
	{
		// List自带的ListHeader和TreeNode中的组合控件要做特殊处理
		DuiLib::CTreeNodeUI* pNode = is_tree_node_sub_control(pDeleted);
		if (pDeleted->GetInterface(DUI_CTR_LISTHEADER) && pDeleted->GetParent() && pDeleted->GetParent()->GetInterface(DUI_CTR_LIST))
			return out_error(_T("不能单独删除List自带的ListHeader!"));
		else if ( pNode && pNode != pDeleted)
			return out_error(_T("不能单独删除TreeNode自带的子控件!"));
		if (pDeleted->GetInterface(DUI_CTR_SCROLLBAR))
			return out_error(_T("不能手动删除滚动条，请在控件属性中禁用滚动条!"));

		if (pDeleted->GetParent() != pWnd)
			pControl->SetTag(reinterpret_cast<UINT_PTR>(pDeleted->GetParent()));
		else
			pControl->SetTag(0);
		pPointed->SetVisible(false);
		pSelected->SetVisible(false);
		__ResetAttribs();
		__RefreshSelect();
		DuiLib::CContainerUI* pParent = static_cast<DuiLib::CContainerUI*>(pDeleted->GetParent()->GetInterface(DUI_CTR_CONTAINER));
		if (pParent) 
			pParent->Remove(pDeleted);
		__SetEditFlag(__m_pWndsTab->GetCurSel(), true);
		__RefreshControlsLayout();
	}
}

void CMainFrame::__OnOpen( DuiLib::TNotifyUI& msg )
{
	__Open();
}

void CMainFrame::__OnSaveAll( DuiLib::TNotifyUI& msg )
{
	__SaveAll();
}

bool CMainFrame::__Save( int _nIdex, bool _bSaveAs )
{
	DuiLib::CControlUI* pItem = __m_pWndsTab->GetItemAt(_nIdex);
	if (!pItem) return false;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pItem->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return false;
	DuiLib::CControlUI* pWTitleItem = __m_pWndsTitle->GetItemAt(_nIdex);
	if (!pWTitleItem) return false;
	DuiLib::CContainerUI* pWTitleContainer = static_cast<DuiLib::CContainerUI*>(pWTitleItem->GetInterface(DUI_CTR_CONTAINER));
	if (!pWTitleContainer) return false;
	DuiLib::COptionUI* pOption = static_cast<DuiLib::COptionUI*>(pWTitleContainer->FindSubControl(_T("wnds.title.h.item.option")));
	if (!pOption) return false;
	if (pOption->GetUserData().IsEmpty() || _bSaveAs)
	{
		static TCHAR  szFileName[MAX_PATH] = { 0 };
		OPENFILENAME ofn = { 0 };
		ofn.hwndOwner = GetHWND();
		ofn.lpstrFile = szFileName;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = _T("*.xml");
		LPCTSTR _lpFile = GetSaveFileName(&ofn) ? szFileName : nullptr;
		if (!_lpFile) return false;
		size_t len = _tcslen(_lpFile);
		if (len < 4 || 
			_lpFile[len - 4] != _T('.') || _lpFile[len - 3] != _T('x') && _lpFile[len - 3] != _T('X') ||
			_lpFile[len - 4] != _T('.') || _lpFile[len - 2] != _T('m') && _lpFile[len - 2] != _T('M') ||
			_lpFile[len - 4] != _T('.') || _lpFile[len - 1] != _T('l') && _lpFile[len - 1] != _T('L'))
			_tcscat_s(szFileName, _T(".xml"));
		__m_FileManager.Save(pCavas->GetItemAt(0), &m_PaintManager, _lpFile);
		pOption->SetText(get_filename(_lpFile));
		pOption->SetPos(pOption->GetPos());
		pOption->SetUserData(_lpFile);
		pOption->SetToolTip(_lpFile);
	}
	else
		__m_FileManager.Save(pCavas->GetItemAt(0), &m_PaintManager, pOption->GetUserData());
	__SetEditFlag(_nIdex, false);
	return true;
}

void CMainFrame::__OnAttribChange( DuiLib::TNotifyUI& msg )
{
	__SetEditFlag(__m_pWndsTab->GetCurSel(), true);
}

void CMainFrame::__OnSaveAs( DuiLib::TNotifyUI& msg )
{
	__Save(__m_pWndsTab->GetCurSel(), true);
}

void CMainFrame::__OnCut( DuiLib::TNotifyUI& msg )
{
	__Cut();
}

void CMainFrame::__OnCopy( DuiLib::TNotifyUI& msg )
{
	__Copy();
}

void CMainFrame::__OnPaste( DuiLib::TNotifyUI& msg )
{
	__Paste();
}

void CMainFrame::__OnPreview( DuiLib::TNotifyUI& msg )
{
	__Preview();
}

void CMainFrame::__FillAttribTree( DuiLib::CControlUI* _pControl, _SETTINGS_CONTROL* _pSC )
{
	_SETTINGS_CONTROLS vecControl;
	config.GetAttribs(_pSC, vecControl);
	for (_SETTINGS_CONTROLS::iterator it = vecControl.begin(); it != vecControl.end(); ++it)
	{
		if (it->vecAttrib.empty()) continue;;
		DuiLib::CTreeNodeUI* pNode = new DuiLib::CTreeNodeUI;
		pNode->GetFolderButton()->ApplyAttributeList(__m_pAttribsFolderStyle->GetUserData());
		pNode->SetItemText(it->strName);
		for (_SETTINGS_ATTRIBS::iterator itAttr = it->vecAttrib.begin(); itAttr != it->vecAttrib.end(); ++itAttr)
		{
			DuiLib::CControlUI* pAttrib = plugin_attrib.CreateAttrib(itAttr->strType, _pControl, itAttr->strName, itAttr->strAlias);
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
		__m_pAttribs->Add(pNode);
	}
}

void CMainFrame::__OnDefaultData( DuiLib::TNotifyUI& msg )
{
	__DefaultData();
}

void CMainFrame::__UpdateToolBar()
{
	DuiLib::CControlUI* pWnd = nullptr;
	DuiLib::CControlUI* pSeleted = nullptr;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (pControl)
	{
		DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
		if (pCavas)
		{
			pWnd = pCavas->GetItemAt(0);
			if (pWnd) pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
		}
	}
	IControlPlugin* pInterface = get_control_interface(pWnd);
	int nID = 0;
	if (pInterface) nID =  _tstoi(pInterface->GetAttribute(_T("fontid")));
	__m_pToolbarBtnUnDo->SetEnabled(true);
	__m_pToolbarBtnRedo->SetEnabled(true);
	__m_pToolbarBtnSave->SetEnabled(!!pWnd);
	__m_pToolbarBtnSaveAs->SetEnabled(!!pWnd);
	__m_pToolbarBtnStart->SetEnabled(!!pWnd);
	__m_pToolbarBtnCut->SetEnabled(!!pSeleted);
	__m_pToolbarBtnCopy->SetEnabled(!!pSeleted);
	__m_pToolbarBtnPaste->SetEnabled(!clipboard.IsEmpty() && !!pWnd);
	__m_pToolbarBtnDel->SetEnabled(!!pSeleted);
	__m_pToolbarBtnDefaultData->SetEnabled(!!pWnd);
	__m_pToolbarBtnUnDo->SetEnabled(nID >= 1000 && memorandum.GetSize(nID) && memorandum.GetCurPos(nID) != 0);
	__m_pToolbarBtnRedo->SetEnabled(nID >= 1000 && memorandum.GetSize(nID) && memorandum.GetCurPos(nID) < memorandum.GetSize(nID) - 1);
}

void CMainFrame::__SaveAll()
{
	for (int i = 0; i < __m_pWndsTab->GetCount(); ++i)
		__Save(i);
}

void CMainFrame::__Close()
{
	bool bEdit = false;
	for (int i = 0; i < __m_pWndsTab->GetCount(); ++i)
	{
		if (__IsEdit(i)) 
		{
			bEdit = true;
			break;
		}
	}
	if (bEdit)
	{
		int nRet = ::MessageBox(GetHWND(), 
			_T("有文档未保存需要保存吗?"),
			_T("退出程序"),
			MB_YESNOCANCEL);
		switch(nRet)
		{
		case IDYES:
			__SaveAll();
			return;
		case IDCANCEL:
			return;
		}
	}
	::PostQuitMessage(0);
}

void CMainFrame::__Cut()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted || !pSeleted->GetParent() || pSeleted == pWnd) return;
	if (!clipboard.Write(pSeleted)) return;
	__DelCurControl();
}

void CMainFrame::__Copy()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted || !pSeleted->GetParent() || pSeleted == pWnd) return;
	clipboard.Write(pSeleted);
}

void CMainFrame::__Paste()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted) pSeleted = pWnd;
	DuiLib::CContainerUI* pSelectedContainer = static_cast<DuiLib::CContainerUI*>(pSeleted->GetInterface(DUI_CTR_CONTAINER));
	if (!pSelectedContainer && pSeleted->GetParent() && pSeleted != pWnd) 
		pSelectedContainer = static_cast<DuiLib::CContainerUI*>(pSeleted->GetParent()->GetInterface(DUI_CTR_CONTAINER));
	if (!pSelectedContainer) return;
	DuiLib::CControlUI* pNew = clipboard.Read(this, &m_PaintManager, pSelectedContainer);
	if (!pNew) return;
	pCavas->SetTag(reinterpret_cast<UINT_PTR>(pNew));
	__RefreshSelect();
}

void CMainFrame::__NewWnd()
{
	// pCavas的Tag保存当前选择的控件指针
	// pOption的Tag保存pCavas,用以切换Tab
	DuiLib::CControlUI* pNew = CreateControl(_T("Canvas")); 
	if (!pNew) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pNew->GetInterface(DUI_CTR_CONTAINER)); 
	if (!pCavas) return;
	pCavas->ApplyAttributeList(__m_pWndsTab->GetUserData());
	DuiLib::CControlUI* pNewWindow = CreateControl(STR_WINDOW); 
	if (!pNewWindow) return;
	pNewWindow->ApplyAttributeList(_T("bkcolor=\"#FF000000\" size=\"800,600\" sizebox=\"0,0,0,0\" caption=\"0,0,0,0\" alpha=\"255\" disabledfontcolor=\"#FFA7A6AA\" defaultfontcolor=\"#FF000000\" linkfontcolor=\"#FF0000FF\" linkhoverfontcolor=\"#FFD3215F\" selectedcolor=\"#FFBAE4FF\""));
	DuiLib::CDuiString strID;
	strID.Format(_T("%d"), 1000 + 100 * create_id());
	pNewWindow->SetAttribute(_T("fontid"), strID);
	strID.Format(_T("%d"), -1);
	pNewWindow->SetAttribute(_T("defaultfont"), strID);
	DuiLib::CControlUI* pPointed = new DuiLib::CControlUI;
	pPointed->ApplyAttributeList(__m_pPointedStyle->GetUserData());
	DuiLib::CControlUI* pSelected = new DuiLib::CControlUI;
	pSelected->ApplyAttributeList(__m_pSelectedStyle->GetUserData());
	pCavas->Add(pNewWindow);
	pCavas->Add(pPointed);
	pCavas->Add(pSelected);
	pCavas->EnableScrollBar(true, true);
	pCavas->GetHorizontalScrollBar()->ApplyAttributeList(m_PaintManager.GetDefaultAttributeList(STR_HSCROLL));
	pCavas->GetVerticalScrollBar()->ApplyAttributeList(m_PaintManager.GetDefaultAttributeList(STR_VSCROLL));

	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pWTitleItem = builder.Create(_T("WndsTitleItem.xml"), NULL, this, &m_PaintManager);
	DuiLib::CContainerUI* pWTitleContainer = static_cast<DuiLib::CContainerUI*>(pWTitleItem->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::COptionUI* pOption = static_cast<DuiLib::COptionUI*>(pWTitleContainer->FindSubControl(_T("wnds.title.h.item.option")));
	DuiLib::COptionUI* pDelete = static_cast<DuiLib::COptionUI*>(pWTitleContainer->FindSubControl(_T("wnds.title.h.item.manager.del.btn")));
	pOption->SetText(_T("未命名"));
	pOption->SetTag(reinterpret_cast<UINT_PTR>(pCavas));
	pWTitleItem->SetTag(pOption->GetTag());
	pDelete->SetTag(reinterpret_cast<UINT_PTR>(pWTitleItem));
	__m_pWndsTitle->Add(pWTitleItem);
	__m_pWndsTab->Add(pCavas);
	__m_pWndsTitleBorder->SetVisible(true);
	pOption->Selected(true);
	__m_pWndsTab->SelectItem(pCavas);
	if (__m_pWndsTab->GetCount() == 1)
		m_PaintManager.SendNotify(__m_pWndsTab, DUI_MSGTYPE_TABSELECT);
	__SetEditFlag(__m_pWndsTab->GetCount() - 1, false);
	__m_pWndsTab->SetPos(__m_pWndsTab->GetPos());
	__WriteMemorandum(__m_pWndsTab->GetCount() - 1);
}

void CMainFrame::__Open()
{
	static TCHAR  szFileName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.hwndOwner = GetHWND();
	ofn.lpstrFile = szFileName;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = _T("XML文档\0*.xml\0\0");
	LPCTSTR _lpFile = GetOpenFileName(&ofn) ? szFileName : nullptr;
	if (!_lpFile) return;

	// pCavas的Tag保存当前选择的控件指针
	// pOption的Tag保存pCavas,用以切换Tab
	DuiLib::CControlUI* pNew = CreateControl(_T("Canvas")); 
	if (!pNew) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pNew->GetInterface(DUI_CTR_CONTAINER)); 
	if (!pCavas) return;
	pCavas->ApplyAttributeList(__m_pWndsTab->GetUserData());
	DuiLib::CControlUI* pNewWindow = __m_FileManager.Open(_lpFile, this, &m_PaintManager);
	if (!pNewWindow) return;
	DuiLib::CControlUI* pPointed = new DuiLib::CControlUI;
	pPointed->ApplyAttributeList(__m_pPointedStyle->GetUserData());
	DuiLib::CControlUI* pSelected = new DuiLib::CControlUI;
	pSelected->ApplyAttributeList(__m_pSelectedStyle->GetUserData());
	pCavas->Add(pNewWindow);
	pCavas->Add(pPointed);
	pCavas->Add(pSelected);
	pCavas->EnableScrollBar(true, true);
	pCavas->GetHorizontalScrollBar()->ApplyAttributeList(m_PaintManager.GetDefaultAttributeList(STR_HSCROLL));
	pCavas->GetVerticalScrollBar()->ApplyAttributeList(m_PaintManager.GetDefaultAttributeList(STR_VSCROLL));

	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pWTitleItem = builder.Create(_T("WndsTitleItem.xml"), NULL, this, &m_PaintManager);
	DuiLib::CContainerUI* pWTitleContainer = static_cast<DuiLib::CContainerUI*>(pWTitleItem->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::COptionUI* pOption = static_cast<DuiLib::COptionUI*>(pWTitleContainer->FindSubControl(_T("wnds.title.h.item.option")));
	DuiLib::COptionUI* pDelete = static_cast<DuiLib::COptionUI*>(pWTitleContainer->FindSubControl(_T("wnds.title.h.item.manager.del.btn")));
	pOption->SetText(get_filename(_lpFile));
	pOption->SetUserData(_lpFile);
	pOption->SetToolTip(_lpFile);
	pOption->SetTag(reinterpret_cast<UINT_PTR>(pCavas));
	pWTitleItem->SetTag(pOption->GetTag());
	pDelete->SetTag(reinterpret_cast<UINT_PTR>(pWTitleItem));
	__m_pWndsTitle->Add(pWTitleItem);
	__m_pWndsTab->Add(pCavas);
	__m_pWndsTitleBorder->SetVisible(true);
	pOption->Selected(true);
	__m_pWndsTab->SelectItem(pCavas);
	__m_pWndsTab->SetPos(__m_pWndsTab->GetPos());
	__WriteMemorandum(__m_pWndsTab->GetCount() - 1);
	if (__m_pWndsTab->GetCount() == 1)
		m_PaintManager.SendNotify(__m_pWndsTab, DUI_MSGTYPE_TABSELECT);
}

DuiLib::CControlUI* CALLBACK FindControlFromFocused(DuiLib::CControlUI* _pControl, LPVOID _lpVoid)
{
	if (_pControl->IsVisible() && _pControl->IsEnabled() && _pControl->IsFocused() && _pControl->GetControlFlags() & UIFLAG_TABSTOP) return _pControl;
	return nullptr;
}

bool CMainFrame::__ShortcutKey( int _nkey, bool _bDown )
{
	if (_bDown)
	{
		switch (_nkey)
		{
		case VK_DELETE:
			{
				DuiLib::CControlUI* pFocusedControl = m_PaintManager.GetRoot()->FindControl(FindControlFromFocused, nullptr, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if (!pFocusedControl || pFocusedControl == __m_pControlsLayoutTree)
				{
					__DelCurControl();
					return true;
				}
			}
			return false;
		case VK_CONTROL:
			__m_bCtrlDown = true;
			return false;
		case _T('X'):
			if (__m_bCtrlDown) 
			{
				DuiLib::CControlUI* pFocusedControl = m_PaintManager.GetRoot()->FindControl(FindControlFromFocused, nullptr, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if (!pFocusedControl)
				{
					__Cut();
					return true;
				}
			}
			return false;
		case _T('C'):
			if (__m_bCtrlDown) 
			{
				DuiLib::CControlUI* pFocusedControl = m_PaintManager.GetRoot()->FindControl(FindControlFromFocused, nullptr, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if (!pFocusedControl)
				{
					__Copy();
					return true;
				}
			}
			return false;
		case _T('V'):
			if (__m_bCtrlDown) 
			{
				DuiLib::CControlUI* pFocusedControl = m_PaintManager.GetRoot()->FindControl(FindControlFromFocused, nullptr, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if (!pFocusedControl)
				{
					__Paste();
					return true;
				}
			}
			return false;
		case _T('N'):
			if (__m_bCtrlDown) __NewWnd();
			return true;
		case _T('O'):
			if (__m_bCtrlDown) __Open();
			return true;
		case _T('S'):
			if (__m_bCtrlDown) __Save(__m_pWndsTab->GetCurSel());
			return true;
		case VK_F5:
			__Preview();
			return true;
		case _T('W'):
			if (__m_bCtrlDown) __DefaultData();
			return true;
		}
	}
	else if (_nkey == VK_CONTROL)
		__m_bCtrlDown = false;
	return false;
}

void CMainFrame::__Preview()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CContainerUI* pWndContainer = static_cast<DuiLib::CContainerUI*>(pWnd->GetInterface(DUI_CTR_CONTAINER));
	if (!pWndContainer || pWndContainer->GetCount() == 0) return;

	IControlPlugin* pInterface = get_control_interface(pWnd);
	if (!pInterface) return;
	_ZnString str;
	__m_FileManager.SaveToStr(pWnd, pWnd, &m_PaintManager, str, true, true, false);
	if (str.empty()) return;
	DuiLib::CDuiString strPath = pInterface->GetAttribute(STR_RESOURCEPATH);
	DuiLib::CDuiString strOldPath = DuiLib::CPaintManagerUI::GetResourcePath();
	DuiLib::CPaintManagerUI::SetResourcePath(strPath);
	plugin_control.SetResourcePath(strPath);
	CPreviewWnd dlg(str.c_str(), strPath);
	dlg.Create(GetHWND(), NULL, UI_WNDSTYLE_DIALOG, NULL);
	dlg.CenterWindow();
	dlg.ShowModal();
	DuiLib::CPaintManagerUI::SetResourcePath(strOldPath);
	plugin_control.SetResourcePath(strOldPath);
}

void CMainFrame::__DefaultData()
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CContainerUI* pWndContainer = static_cast<DuiLib::CContainerUI*>(pWnd->GetInterface(DUI_CTR_CONTAINER));
	if (!pWndContainer) return;
	CDefaultAttrWnd dlg(pWndContainer);
	dlg.Create(GetHWND(), nullptr, UI_WNDSTYLE_DIALOG, NULL);
	dlg.CenterWindow();
	dlg.ShowModal();
	if (dlg.m_bChange)
	{
		__SetEditFlag(__m_pWndsTab->GetCurSel(), true);
		__m_FileManager.SetDefaultAndFont(pWndContainer->GetItemAt(0), pWnd, &m_PaintManager);
		pWnd->NeedUpdate();
	}
}

void CMainFrame::__OnUnDo( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	IControlPlugin* pInterface = get_control_interface(pWnd);
	if (!pInterface) return;
	int nID = _tstoi(pInterface->GetAttribute(_T("fontid")));
	if (nID < 1000) return;
	memorandum.MovePre(nID);
	int nDefaultFont = _tstoi(pInterface->GetAttribute(_T("defaultfont")));
	__ReadMemorandum(nID, nDefaultFont, pCavas);
}

void CMainFrame::__OnReDo( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	IControlPlugin* pInterface = get_control_interface(pWnd);
	if (!pInterface) return;
	int nID = _tstoi(pInterface->GetAttribute(_T("fontid")));
	if (nID < 1000) return;
	memorandum.MoveNext(nID);
	int nDefaultFont = _tstoi(pInterface->GetAttribute(_T("defaultfont")));
	__ReadMemorandum(nID, nDefaultFont, pCavas);
}

void CMainFrame::__ReadMemorandum(int _nID, int _nDefaultFont, DuiLib::CContainerUI* _pCavas)
{
	_ZnString str;
	if (memorandum.Get(_nID, str) && !str.empty())
	{
		DuiLib::CDialogBuilder builder;
		DuiLib::CControlUI* pNewWnd =  builder.Create(str.c_str(), NULL, this, &m_PaintManager, nullptr);
		if (pNewWnd)
		{
			DuiLib::CControlUI* pOldWnd = _pCavas->GetItemAt(0);
			IControlPlugin* pInterface = get_control_interface(pOldWnd);
			DuiLib::CDuiString strPath = pInterface->GetAttribute(STR_RESOURCEPATH).GetData();
			_pCavas->AddAt(pNewWnd, 0);
			_pCavas->SetTag(0);
			_pCavas->Remove(pOldWnd);
			__ResetAttribs();
			__RefreshControlsLayout();
			__m_FileManager.SetDefaultAndFont(pNewWnd, &m_PaintManager, _nID, _nDefaultFont);
			DuiLib::CDuiString strValue;
			strValue.Format(_T("%d"), _nID);
			pNewWnd->SetAttribute(_T("fontid"), strValue);
			strValue.Format(_T("%d"), _nDefaultFont);
			pNewWnd->SetAttribute(_T("defaultfont"), strValue);
			pNewWnd->SetAttribute(STR_RESOURCEPATH, strPath);
			pNewWnd->SetBkColor(0xFF000000);

		}
	}
}

void CMainFrame::__WriteMemorandum( int _nPos )
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(_nPos);
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	IControlPlugin* pInterface = get_control_interface(pWnd);
	if (!pInterface) return;
	int nID = _tstoi(pInterface->GetAttribute(_T("fontid")));
	if (nID < 1000) return;
	_ZnString str;
	__m_FileManager.SaveToStr(pWnd, pWnd, &m_PaintManager, str);
	memorandum.Add(nID, str);
}

void CMainFrame::__OnControlsType( DuiLib::TNotifyUI& msg )
{
	__m_pControlsTab->SelectItem(_tstoi(msg.pSender->GetUserData()));
	__RefreshControlsLayout();
}

void CMainFrame::__RefreshControlsLayout()
{
	if (__m_pControlsTab->GetCurSel() != 1) return;
	__m_pControlsLayoutTree->RemoveAll();
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) return;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted) pSeleted = pWnd;
	DuiLib::CTreeNodeUI* pSeletedNode = __RefreshControlsLayout(pWnd, pSeleted, nullptr);
	if (pSeletedNode)
	{
		__m_pControlsLayoutTree->ApplyAttributeList(__m_pControlsLayoutTree->GetUserData());
		__m_pControlsLayoutTree->SetPos(__m_pControlsLayoutTree->GetPos());
		__m_pControlsLayoutTree->SelectItem(__m_pControlsLayoutTree->GetItemIndex(pSeletedNode));
	}
}

DuiLib::CTreeNodeUI* CMainFrame::__RefreshControlsLayout( DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pSelected
	, DuiLib::CTreeNodeUI* _pNode, int _nLevel )
{
	// 返回值是已选择的节点
	if (!_pControl) return nullptr;
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(_pControl->GetClass());
	if (!pSC) return nullptr;
	DuiLib::CTreeNodeUI* pSelectedNode = nullptr;
	DuiLib::CTreeNodeUI* pNode = new DuiLib::CTreeNodeUI(_pNode);
	DuiLib::CTreeViewUI* pTree = static_cast<DuiLib::CTreeViewUI*>(_pControl->GetInterface(DUI_CTR_TREEVIEW));
	DuiLib::CTreeNodeUI* pTreeNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetInterface(DUI_CTR_TREENODE));
	if (pTree)
	{
		for (int i = 0; i < pTree->GetCount(); ++i)
		{
			DuiLib::CTreeNodeUI* pTreeItem = static_cast<DuiLib::CTreeNodeUI*>(pTree->GetItemAt(i));
			if (!pTreeItem->GetParentNode())
			{
				DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pTree->GetItemAt(i), _pSelected, pNode, _nLevel + 1);
				if ( pRet ) pSelectedNode = pRet;
			}
		}
	}
	else if (pTreeNode)
	{
		DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pTreeNode->GetTreeNodeHoriznotal(), _pSelected, pNode, _nLevel + 1);
		if ( pRet ) pSelectedNode = pRet;
		for (int i = 0; i < pTreeNode->GetCountChild(); ++i)
		{
			DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pTreeNode->GetChildNode(i), _pSelected, pNode, _nLevel + 1);
			if ( pRet ) pSelectedNode = pRet;
		}
	}
	else
	{
		DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(_pControl->GetInterface(DUI_CTR_LIST));
		if (pList)
		{
			DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pList->GetHeader(), _pSelected, pNode, _nLevel + 1);
			if ( pRet ) pSelectedNode = pRet;
		}
		DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(_pControl->GetInterface(DUI_CTR_CONTAINER));
		if (pContainer)
		{
			if (pContainer->GetHorizontalScrollBar())
			{
				DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pContainer->GetHorizontalScrollBar(), _pSelected, pNode, _nLevel + 1);
				if ( pRet ) pSelectedNode = pRet;
			}
			if (pContainer->GetVerticalScrollBar())
			{
				DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pContainer->GetVerticalScrollBar(), _pSelected, pNode, _nLevel + 1);
				if ( pRet ) pSelectedNode = pRet;
			}
			for (int i = 0; i < pContainer->GetCount(); ++i)
			{
				DuiLib::CTreeNodeUI* pRet = __RefreshControlsLayout(pContainer->GetItemAt(i), _pSelected, pNode, _nLevel + 1);
				if ( pRet ) pSelectedNode = pRet;
			}
		}
	}
	if (pNode->IsHasChild())
		pNode->GetFolderButton()->ApplyAttributeList(__m_pControlsLayoutFolderStyle->GetUserData());
	else
		pNode->GetFolderButton()->SetEnabled(false);
	pNode->SetItemText(pSC->strName.GetData());
	if (_pNode)
	{
		pNode->SetTag(reinterpret_cast<UINT_PTR>(_pControl));
		_pNode->Add(pNode);
	}
	else
		__m_pControlsLayoutTree->Add(pNode);
	if (_pControl == _pSelected)
		pSelectedNode = pNode;
	return pSelectedNode;
}

void CMainFrame::__OnLayoutSelected( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) return;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) return;
	DuiLib::CControlUI* pItem = __m_pControlsLayoutTree->GetItemAt(msg.wParam);
	UINT_PTR tag = 0;
	if (pItem)
	{
		DuiLib::CTreeNodeUI* pNode = static_cast<DuiLib::CTreeNodeUI*>(pItem->GetInterface(DUI_CTR_TREENODE));
		if (pNode) tag = pNode->GetTag();
	}
	pCavas->SetTag(tag);
}

void CMainFrame::__OnDel( DuiLib::TNotifyUI& msg )
{
	__DelCurControl();
}

bool CMainFrame::__Move()
{
	if (!__m_bLBtnDown || __m_byOperation != OP_Move) return false;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) 
		return false;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) 
		return false;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) 
		return false;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted || !pSeleted->GetParent() || pSeleted == pWnd) 
		return false;
	RECT rcPos = pSeleted->GetRelativePos();
	POINT ptMouse = m_PaintManager.GetMousePos();
	int x = ptMouse.x - __m_ptOldMouse.x;
	int y = ptMouse.y - __m_ptOldMouse.y;
	if (x || y)
	{
		::OffsetRect(&rcPos, x, y);
		pSeleted->SetPos(rcPos);
		__m_bMove = true;
		__m_ptOldMouse = m_PaintManager.GetMousePos();
		__m_pAttribs->NeedUpdate();
		return true;
	}
	return false;
}

void CMainFrame::__MouseDown( POINT _ptBegin )
{
	//if (__m_byOperation != OP_Move) return;
	__m_bLBtnDown = true;
	__m_ptOldMouse = m_PaintManager.GetMousePos();
	__m_bMove = false;
	__m_bSize = false;
}

void CMainFrame::__MouseUp()
{
	//if (__m_byOperation != OP_Move) return;
	__m_bLBtnDown = false;
	if (__m_bMove || __m_bSize)
	{
		__m_pAttribs->NeedUpdate();
		__SetEditFlag(__m_pWndsTab->GetCurSel(), true);
	}
}

bool CMainFrame::__Size()
{
	if (!__m_bLBtnDown || __m_byOperation < OP_Size_Left || __m_byOperation > OP_Size_RightBottom) 
		return false;
	DuiLib::CControlUI* pControl = __m_pWndsTab->GetItemAt(__m_pWndsTab->GetCurSel());
	if (!pControl) 
		return false;
	DuiLib::CContainerUI* pCavas = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pCavas) 
		return false;
	DuiLib::CControlUI* pWnd = pCavas->GetItemAt(0);
	if (!pWnd) 
		return false;
	DuiLib::CControlUI* pSeleted = reinterpret_cast<DuiLib::CControlUI*>(pCavas->GetTag());
	if (!pSeleted || !pSeleted->GetParent() || pSeleted == pWnd) 
		return false;
	RECT rcPos = pSeleted->GetRelativePos();
	POINT ptMouse = m_PaintManager.GetMousePos();
	BYTE byOperation = __m_byOperation & 0x0F; 
	int x = ptMouse.x - __m_ptOldMouse.x;
	int y = ptMouse.y - __m_ptOldMouse.y;
	if (x || y)
	{
		if (__m_byOperation & OP_Size_Left)
			rcPos.left += x;
		else if (__m_byOperation & OP_Size_Right)
			rcPos.right += x;
		if (__m_byOperation & OP_Size_Top)
			rcPos.top += y;
		else if (__m_byOperation & OP_Size_Bottom)
			rcPos.bottom += y;
		rcPos = normalize_rect(rcPos);
		pSeleted->SetFixedWidth(rcPos.right - rcPos.left);
		pSeleted->SetFixedHeight(rcPos.bottom - rcPos.top);
		pSeleted->SetPos(rcPos);
		__m_bSize = true;
		__m_ptOldMouse = m_PaintManager.GetMousePos();
		__m_pAttribs->NeedUpdate();
		return true;
	}
	return false;
}
