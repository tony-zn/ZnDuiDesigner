#pragma once
#ifndef __SETTINGSWND_H_2015_11_04
#define __SETTINGSWND_H_2015_11_04

class CSettingsWnd : public DuiLib::WindowImplBase
{
public:
	DuiLib::CDuiString			GetSkinFolder();
	DuiLib::CDuiString			GetSkinFile();
	LPCTSTR						GetWindowClassName(void) const;
	void						InitWindow();
	void						Notify(DuiLib::TNotifyUI& msg);
	LRESULT						OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void						__LoadConfig();
	void						__InitBasic();
	void						__InitControls();
	void						__InitAttribs();
	bool						__AddAttrib(_SETTINGS_ATTRIB& _sa, int _nIndex, bool _bEdit);
	void						__LoadAttrib(LPCTSTR _lpName, bool _bEdit);
	void						__SaveAttribs(DuiLib::CDuiString _strControl, bool _bEnable);
	inline DuiLib::CTreeNodeUI* __NewTreeNode(bool _bFolder);
	void						__SwapAttrib(int _nPos1, int _nPos2);
	void						__SelectCombo(DuiLib::CComboUI* pCombo, LPCTSTR _lpStr, LPCTSTR _lpHide = nullptr);
	void						__OnTypeChange(DuiLib::TNotifyUI& msg);
	void						__OnControlsTreeSelectChange(DuiLib::TNotifyUI& msg);
	void						__OnChangePreview(DuiLib::TNotifyUI& msg);
	void						__OnAttribAdd(DuiLib::TNotifyUI& msg);
	void						__OnAttribDelete(DuiLib::TNotifyUI& msg);
	void						__OnControlEnable(DuiLib::TNotifyUI& msg);
	void						__OnAttrbMoveUp(DuiLib::TNotifyUI& msg);
	void						__OnAttrbMoveDown(DuiLib::TNotifyUI& msg);
private:
	std::vector<DuiLib::CDuiString>	__m_vecAttribType;
	DuiLib::CTabLayoutUI*			__m_pRootTab;
	DuiLib::CTreeViewUI*			__m_pControlsTree;
	DuiLib::CVerticalLayoutUI*		__m_pControlAttr;
	DuiLib::CControlUI*				__m_pTreeNodeStyle;
	DuiLib::CControlUI*				__m_pTreeNodeStyleHor;
	DuiLib::CControlUI*				__m_pTreeNodeStyleDot;
	DuiLib::CControlUI*				__m_pTreeNodeStyleFolder;
	DuiLib::CControlUI*				__m_pTreeNodeStyleCheckBox;
	DuiLib::CControlUI*				__m_pTreeNodeStyleItem;
	DuiLib::CListUI*				__m_pAttribList;
	DuiLib::CControlUI*				__m_pAttribListItemStyle;
	DuiLib::CControlUI*				__m_pAttribListItemStyleName;
	DuiLib::CControlUI*				__m_pAttribListItemStyleType;
	DuiLib::CRichEditUI*			__m_pControlAliasRich;
	DuiLib::CRichEditUI*			__m_pControlImageRich;
	DuiLib::CComboUI*				__m_pParentCombo;
};

#endif