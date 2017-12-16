#pragma once
#ifndef __DEFAULTATTRWND_H_2015_11_26
#define __DEFAULTATTRWND_H_2015_11_26

class CDefaultAttrWnd : public DuiLib::WindowImplBase
{
public:
	CDefaultAttrWnd(DuiLib::CContainerUI* _pWnd);
	~CDefaultAttrWnd();
	DuiLib::CDuiString			GetSkinFolder();
	DuiLib::CDuiString			GetSkinFile();
	LPCTSTR						GetWindowClassName(void) const;
	void						InitWindow();
	void						Notify(DuiLib::TNotifyUI& msg);
	LRESULT						OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void						__InitControls();
	void						__AddControlsItem(LPCTSTR _lpName);
	void						__ResetControlsItem(DuiLib::CControlUI* _pItem);
	DuiLib::CComboUI*			__GetControlsItemCmbo(DuiLib::CControlUI* _pItem);
	void						__InitAttributes();
	void						__ResetAttribs();
	void						__FillAttribTree( DuiLib::CControlUI* _pControl, _SETTINGS_CONTROL* _pSC );
	void						__SaveDefaultAttr();
	void						__OnControlsListSelectedChange(DuiLib::TNotifyUI& msg);
	void						__OnControlsListDBClick(DuiLib::TNotifyUI& msg);
	void						__OnControlsListComboChange(DuiLib::TNotifyUI& msg);
	void						__OnControlsAdd(DuiLib::TNotifyUI& msg);
	void						__OnControlsDelete(DuiLib::TNotifyUI& msg);
	void						__OnControlsListOptChange(DuiLib::TNotifyUI& msg);
	void						__OnChange(DuiLib::TNotifyUI& msg);
public:
	bool						m_bChange;
private:
	DuiLib::CListUI*					__m_pControlList;
	DuiLib::CTreeViewUI*				__m_pAttribTree;
	DuiLib::CControlUI*					__m_pFolderStyle;
	DuiLib::CContainerUI*				__m_pWnd;
	std::vector<DuiLib::CDuiString>		__m_vecControls;
	DuiLib::CVerticalLayoutUI*			__m_pVirParent;
	DuiLib::CTreeViewUI*				__m_pVirTree;
	int									__m_nFont;
};

#endif