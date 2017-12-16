#pragma once
#ifndef __MAINFRAME_H_2015_11_04
#define __MAINFRAME_H_2015_11_04

#define MYWM_SELECTEDCHANGE		WM_USER + 1

class CMainFrame : public DuiLib::WindowImplBase
{
	enum _Operation
	{
		OP_Normal = 0,
		OP_Size_Left = 0x1,
		OP_Size_Right = 0x2,
		OP_Size_Top = 0x4,
		OP_Size_Bottom = 0x8,
		OP_Size_LeftTop = 0x5,
		OP_Size_RightTop = 0x6,
		OP_Size_LeftBottom = 0x9,
		OP_Size_RightBottom = 0xA,
		OP_Add = 0x10,
		OP_Move,
	};
public:
	CMainFrame(LPCTSTR _lpSkinPath);
	DuiLib::CDuiString			GetSkinFolder();
	DuiLib::CDuiString			GetSkinFile();
	LPCTSTR						GetWindowClassName(void) const;
	DuiLib::CControlUI*			CreateControl(LPCTSTR pstrClass);
private:
	void						InitWindow();
	LRESULT						OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void						Notify(DuiLib::TNotifyUI& msg);
	LRESULT						HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT						ResponseDefaultKeyEvent(WPARAM wParam);
private:
	void						__AddControlsItem(LPCTSTR _lpImage, LPCTSTR _lpControl, int nPos = -1);
	void						__Point();
	void						__Select();
	void						__RefreshSelect();
	bool						__SetCursor();
	void						__Add();
	RECT						__CalRect(DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pParent);
	void						__ResetAttribs();
	void						__OnMenu(DuiLib::TNotifyUI& msg);
	void						__OnMenuSettings(DuiLib::TNotifyUI& msg);
	void						__OnMenuAbout(DuiLib::TNotifyUI& msg);
	void						__OnNewWnd(DuiLib::TNotifyUI& msg);
	void						__OnDeleteWnd(DuiLib::TNotifyUI& msg);
	void						__OnSave(DuiLib::TNotifyUI& msg);
	void						__OnSaveAs(DuiLib::TNotifyUI& msg);
	void						__OnSaveAll(DuiLib::TNotifyUI& msg);
	void						__OnOpen(DuiLib::TNotifyUI& msg);
	void						__OnLoadContols(DuiLib::TNotifyUI& msg);
	void						__OnSelectWnd(DuiLib::TNotifyUI& msg);
	void						__OnResetAttribs(DuiLib::TNotifyUI& msg);
	void						__OnUpdateTimer(DuiLib::TNotifyUI& msg);
	void						__OnAttribChange(DuiLib::TNotifyUI& msg);
	void						__OnCut(DuiLib::TNotifyUI& msg);
	void						__OnCopy(DuiLib::TNotifyUI& msg);
	void						__OnPaste(DuiLib::TNotifyUI& msg);
	void						__OnDel(DuiLib::TNotifyUI& msg);
	void						__OnPreview(DuiLib::TNotifyUI& msg);
	void						__OnDefaultData(DuiLib::TNotifyUI& msg);
	void						__OnUnDo(DuiLib::TNotifyUI& msg);
	void						__OnReDo(DuiLib::TNotifyUI& msg);
	void						__OnControlsType(DuiLib::TNotifyUI& msg);
	void						__OnLayoutSelected(DuiLib::TNotifyUI& msg);
	void						__SetEditFlag(int _nPos, bool _bEdit);
	bool						__IsEdit(int _nPos);
	void						__DelCurControl();
	bool						__Save(int _nIdex, bool _bSaveAs = false);
	void						__FillAttribTree(DuiLib::CControlUI* _pControl,_SETTINGS_CONTROL* _pSC);
	void						__UpdateToolBar();
	void						__SaveAll();
	void						__Close();
	void						__Cut();
	void						__Copy();
	void						__Paste();
	void						__NewWnd();
	void						__Open();
	bool						__ShortcutKey(int _nkey, bool _bDown);
	void						__Preview();
	void						__DefaultData();
	void						__ReadMemorandum(int _nID, int _nDefaultFont, DuiLib::CContainerUI* _pCavas);
	void						__WriteMemorandum(int _nPos);
	void						__RefreshControlsLayout();
	DuiLib::CTreeNodeUI*		__RefreshControlsLayout(DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pSelected, DuiLib::CTreeNodeUI* _pNode, int _nLevel = 0);
	void						__MouseDown(POINT _ptBegin);
	bool						__Move();
	void						__MouseUp();
	bool						__Size();
private:
	bool						__m_bIsInit;
	DuiLib::CDuiString			__m_strSkinPath;
	DuiLib::CControlUI*			__m_pTitle;
	DuiLib::CControlUI*			__m_pRestoreBtn;
	DuiLib::CControlUI*			__m_pMaxBtn;
	DuiLib::CControlUI*			__m_pMenu;
	DuiLib::CContainerUI*		__m_pWndsTitle;
	DuiLib::CControlUI*			__m_pWndsTitleBorder;
	DuiLib::CTabLayoutUI*		__m_pWndsTab;
	DuiLib::CTreeViewUI*		__m_pAttribs;
	DuiLib::CControlUI*			__m_pAttribsFolderStyle;
	DuiLib::CControlUI*			__m_pAttribsTitle;
	DuiLib::CTabLayoutUI*		__m_pControlsTab;
	DuiLib::CListUI*			__m_pControls;
	DuiLib::CTreeViewUI*		__m_pControlsLayoutTree;
	DuiLib::CControlUI*			__m_pControlsLayoutFolderStyle;
	DuiLib::CControlUI*			__m_pPointedStyle;
	DuiLib::CControlUI*			__m_pSelectedStyle;
	DuiLib::CButtonUI*			__m_pToolbarBtnUnDo;
	DuiLib::CButtonUI*			__m_pToolbarBtnRedo;
	DuiLib::CButtonUI*			__m_pToolbarBtnSave;
	DuiLib::CButtonUI*			__m_pToolbarBtnSaveAs;
	DuiLib::CButtonUI*			__m_pToolbarBtnStart;
	DuiLib::CButtonUI*			__m_pToolbarBtnCut;
	DuiLib::CButtonUI*			__m_pToolbarBtnCopy;
	DuiLib::CButtonUI*			__m_pToolbarBtnPaste;
	DuiLib::CButtonUI*			__m_pToolbarBtnDel;
	DuiLib::CButtonUI*			__m_pToolbarBtnDefaultData;
	BYTE						__m_byOperation;
	bool						__m_bLBtnDown;
	CFileManager				__m_FileManager;
	bool						__m_bCtrlDown;
	POINT						__m_ptOldMouse;
	bool						__m_bMove;
	bool						__m_bSize;
};

#endif