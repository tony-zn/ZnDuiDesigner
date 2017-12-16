#include "StdAfx.h"
#include "FileManager.h"
#include <sstream>

DuiLib::CControlUI* CFileManager::Open( LPCTSTR _lpFile, DuiLib::IDialogBuilderCallback* _pCallback ,DuiLib::CPaintManagerUI* _pManager )
{
	DuiLib::CControlUI* pWnd = g_pMainFrame->CreateControl(STR_WINDOW);
	if (!pWnd) return nullptr;
	pWnd->SetBkColor(0xFF000000);
	DuiLib::CContainerUI* pWndContainer = static_cast<DuiLib::CContainerUI*>(pWnd->GetInterface(DUI_CTR_CONTAINER));
	if (!pWndContainer) {delete pWndContainer; return nullptr;}
	std::ifstream fs(__StrToXmlStr(_lpFile, CP_ACP).c_str());
	if (!fs) {delete pWnd; return nullptr;}
	rapidxml::file<> file(fs);
	rapidxml::xml_document<> xml;
	xml.clear();
	xml.parse<0>(file.data());
	rapidxml::xml_node<>* pWndNode = xml.first_node();
	if (!pWndNode) {delete pWnd; return nullptr;}
	int nFontID = 1000 + 100 * create_id();
	int nDefaultFontID = -1;
	DuiLib::CDuiString strFontID;
	strFontID.Format(_T("%d"), nFontID);
	pWnd->SetAttribute(_T("fontid"), strFontID);
	_SETTINGS_CONTROLS vecControls;
	config.GetAttribs(config.GetControlUseClassName(pWnd->GetClass()), vecControls);
	if (!vecControls.empty()) 
	{
		for (_SETTINGS_CONTROLS::iterator itControl = vecControls.begin(); itControl != vecControls.end(); ++itControl)
		{
			for (_SETTINGS_ATTRIBS::iterator it = itControl->vecAttrib.begin(); it != itControl->vecAttrib.end(); ++it)
			{
				rapidxml::xml_attribute<>* pAttrib = pWndNode->first_attribute(__StrToXmlStr(it->strName).c_str());
				if (pAttrib)
					pWnd->SetAttribute(it->strName, __XmlStrToStr(pAttrib->value()));
				else
					pWnd->SetAttribute(it->strName, it->strDefaultValue);
			}
		}
		pWndNode->remove_all_attributes();
		std::vector<rapidxml::xml_node<>*> vecDelete;
		int nFontPos = 0;
		for (rapidxml::xml_node<>* pChild = pWndNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			if (strcmp(pChild->name(), "Font") == 0)
			{
				int nRealID = nFontID + nFontPos++;
				DuiLib::CDuiString strName;
				int nSize = 0;
				bool bBold = false, bUnderLine = false, bItalic = false, bShared = false;
				rapidxml::xml_attribute<>* pName = pChild->first_attribute("name"); if (pName) strName = __XmlStrToStr(pName->value());
				rapidxml::xml_attribute<>* pSize = pChild->first_attribute("size"); if (pSize) nSize = atoi(pSize->value());
				rapidxml::xml_attribute<>* pBold = pChild->first_attribute("bold"); if (pBold) bBold = strcmp(pBold->value(), "true") == 0;
				rapidxml::xml_attribute<>* pUnderline = pChild->first_attribute("underline"); if (pUnderline) bUnderLine = strcmp(pUnderline->value(), "true") == 0;
				rapidxml::xml_attribute<>* pItalic = pChild->first_attribute("italic"); if (pItalic) bItalic = strcmp(pItalic->value(), "true") == 0;
				rapidxml::xml_attribute<>* pShared = pChild->first_attribute("shared"); if (pShared) bShared = strcmp(pShared->value(), "true") == 0;
				rapidxml::xml_attribute<>* pDefault = pChild->first_attribute("default"); 
				if (pDefault && strcmp(pDefault->value(), "true") == 0) 
				{
					DuiLib::CDuiString strDefaultFont;
					strDefaultFont.Format(_T("%d"), nRealID);
					pWnd->SetAttribute(_T("defaultfont"), strDefaultFont);
					nDefaultFontID = nRealID;
				}
				if (pName && pSize)
				{
					_pManager->AddFont(nRealID, strName, nSize, bBold, bUnderLine, bItalic, false);
					if (bShared) 
					{
						IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
						if (pWSD) pWSD->SetSharedFont(nRealID, true);
					}
				}
				vecDelete.push_back(pChild);
			}
			else if (strcmp(pChild->name(), "Default") == 0)
			{
				rapidxml::xml_attribute<>* pName = pChild->first_attribute("name");
				rapidxml::xml_attribute<>* pValue = pChild->first_attribute("value");
				if (pName && pValue)
				{
					DuiLib::CDuiString strName;
					bool bShared = false;
					strName.Format(_T("_%d_%s"), nFontID, __XmlStrToStr(pName->value()));
					rapidxml::xml_attribute<>* pShared = pChild->first_attribute("shared"); if (pShared) bShared = strcmp(pShared->value(), "true") == 0;
					_ZnString strAttr(__XmlStrToStr(pValue->value()));
					__AttrToVirAttr(strAttr, nFontID);
					_pManager->AddDefaultAttributeList(strName, strAttr.c_str(), false);
					if (bShared) 
					{
						IWindowSharedData* pWSD = get_wnd_shared_data(pWnd);
						if (pWSD) pWSD->SetSharedDefault(strName, true);
					}
				}
				vecDelete.push_back(pChild);
			}
			else if (strcmp(pChild->name(), "Image") == 0 ||
				strcmp(pChild->name(), "MultiLanguage") == 0 )
				vecDelete.push_back(pChild);
		}
		for (auto it = vecDelete.begin(); it != vecDelete.end(); ++it)
			pWndNode->remove_node(*it);
	}
	pWnd->SetAttribute(STR_RESOURCEPATH, get_path(_lpFile));
	__RepairXml(pWndNode);
	std::string str;
	rapidxml::print(std::back_inserter(str), xml);
	__HandleQuotes(str);
#ifdef _UNICODE
	std::wstring strxml = S2Wstr(str.c_str(), CP_UTF8).get();
#else
	std::wstring wstrxml = S2Wstr(str.c_str(), CP_UTF8).get();
	std::string& strxml = W2Str(wstrxml.c_str(), CP_UTF8).get();
#endif
	DuiLib::CDialogBuilder builder;
	DuiLib::CMarkup* pMarkup = builder.GetMarkup();
	if (!pMarkup->Load(strxml.c_str())) 
	{
		delete pWnd; 
		TCHAR chMsg[255], chLocation[255];
		pMarkup->GetLastErrorMessage(chMsg, 255);
		pMarkup->GetLastErrorLocation(chLocation, 255);
		_ZnString strOut(_T("XML解析失败：\n 在 "));
		strOut += chLocation;
		strOut += _T(" 处发生错误，错误信息：");
		strOut += chMsg;
		out_error(strOut.c_str()); 
		return nullptr;
	}
	DuiLib::CControlUI* pControl = builder.Create(_pCallback, _pManager);
	if (pControl)
	{
		SetDefaultAndFont(pControl, _pManager, nFontID, nDefaultFontID);
		pWndContainer->Add(pControl);
	}
	return pWnd;
}

bool CFileManager::Save( DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, LPCTSTR _lpFile )
{
	if (!_pWnd || !_lpFile || !_pWnd) return false;
	rapidxml::xml_document<TCHAR> xml;
	rapidxml::xml_node<TCHAR>* pWndNode = xml.allocate_node(rapidxml::node_element, STR_WINDOW);
	xml.append_node(pWndNode);
	__SaveFont(_pWnd, _pManager, pWndNode);
	__SaveDefault(_pWnd, _pManager, pWndNode);
	__SetControl(_pWnd, pWndNode, _pWnd, _pManager);
	std::ofstream fs(__StrToXmlStr(_lpFile, CP_ACP).c_str());
	fs << (char)0xef << (char)0xbb << (char)0xbf;
	fs << __StrToXmlStr(_T("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"), CP_UTF8).c_str() << std::endl;
	std::wstring str;
	rapidxml::print(std::back_inserter(str), xml);
	std::string strout = __StrToXmlStr(str.c_str(), CP_UTF8);
	__HandleQuotes(strout);
	fs << strout;
	return true;
}

bool CFileManager::SaveToStr( DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager
	, _ZnString& _str, bool _bSaveDefault, bool _bSaveFont, bool _bSaveWnd )
{
	if (!_pWnd) return false;
	rapidxml::xml_document<TCHAR> xml;
	rapidxml::xml_node<TCHAR>* pWndNode = xml.allocate_node(rapidxml::node_element, STR_WINDOW);
	xml.append_node(pWndNode);
// 	_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(_pControl->GetClass());
// 	rapidxml::xml_node<TCHAR>* pNode = xml.allocate_node(rapidxml::node_element, pInfo->strName);
// 	pWndNode->append_node(pNode);
	rapidxml::xml_node<TCHAR>* pNode = pWndNode;
	if (_bSaveWnd)
	{
		_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(_pControl->GetClass());
		pNode = xml.allocate_node(rapidxml::node_element, pInfo->strName);
		pWndNode->append_node(pNode);
	}
	if (_bSaveFont) __SaveFont(_pWnd, _pManager, pWndNode);
	if (_bSaveDefault) __SaveDefault(_pWnd, _pManager, pWndNode);
	__SetControl(_pControl, pNode, _pWnd, _pManager);
	std::stringstream ss;
	ss << __StrToXmlStr(_T("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"), CP_UTF8).c_str() << std::endl;
	std::wstring str;
	rapidxml::print(std::back_inserter(str), xml);
	std::string strout = __StrToXmlStr(str.c_str(), CP_UTF8);
	__HandleQuotes(strout);
	ss << strout;
	_str = __XmlStrToStr(ss.str().c_str(), CP_UTF8);
	return true;
}

void CFileManager::__SetAttrib( rapidxml::xml_node<TCHAR>* _pNode, DuiLib::CControlUI* _pControl, 
	DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pPaintManager )
{
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(_pControl->GetClass());
	IControlPlugin* pPlgin = get_control_interface(_pControl);
	IControlPlugin* pInterface = get_control_interface(_pWnd);
	DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
	int nFont = _tstoi(strFont);
	DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
	int nDefaultFont = _tstoi(strDefaultFont);
	std::map<DuiLib::CDuiString, DuiLib::CDuiString> mapAttr;
	__GetDefaultAttr(_pPaintManager, pSC, nFont, mapAttr);
	_SETTINGS_CONTROLS vecControl;
	config.GetAttribs(pSC, vecControl);
	for (_SETTINGS_CONTROLS::iterator itControl = vecControl.begin(); itControl != vecControl.end(); ++itControl)
	{
		for (_SETTINGS_ATTRIBS::iterator it = itControl->vecAttrib.begin(); it != itControl->vecAttrib.end(); ++it)
		{
			if (it->strName.IsEmpty()) continue;
			if (it->strName == STR_RESOURCEPATH) continue;

			auto itAttr = mapAttr.find(it->strName);
			DuiLib::CDuiString strDefaultValue;
			if (itAttr != mapAttr.end())
				strDefaultValue = itAttr->second;
			else
				strDefaultValue = it->strDefaultValue;

			DuiLib::CDuiString strValue = pPlgin->GetAttribute(it->strName);
			if (it->strName == _T("font") || it->strName == _T("itemfont"))
			{
				int nValue = _tstoi(strValue);
				int nDefaultAttrFont = nDefaultFont;
				if (itAttr != mapAttr.end()) 
					nDefaultAttrFont = _tstoi(strDefaultValue);
				if (nValue == nDefaultFont)
					continue;
				else if (nValue < 0)
					strValue = _T("-1");
				else
					strValue.Format(_T("%d"), nValue - nFont);
			}
			if (!strValue.IsEmpty() && strValue != strDefaultValue || 
				strValue.IsEmpty() && !strDefaultValue.IsEmpty() && strDefaultValue != _T("false"))
			{
				rapidxml::xml_attribute<TCHAR>* pAttrib = _pNode->document()->allocate_attribute(
					_pNode->document()->allocate_string(it->strName), 
					_pNode->document()->allocate_string(strValue));
				_pNode->append_attribute(pAttrib);
			}
		}
	}
	
}

void CFileManager::__SetAttrib( rapidxml::xml_node<TCHAR>* _pNode, LPCTSTR _lpName, LPCTSTR _lpValue )
{
	rapidxml::xml_attribute<TCHAR>* pAttrib = _pNode->document()->allocate_attribute(
		_pNode->document()->allocate_string(_lpName), 
		_pNode->document()->allocate_string(_lpValue));
	_pNode->append_attribute(pAttrib);
}

DuiLib::CDuiString CFileManager::__XmlStrToStr( LPCSTR _lpStr, UINT unCode )
{
#	ifdef _UNICODE
	return DuiLib::CDuiString(S2Wstr(_lpStr, unCode).get());
#	else
	return DuiLib::CDuiString(_lpStr);
#	endif
}

std::string CFileManager::__StrToXmlStr( LPCTSTR _lpStr, UINT unCode )
{
#	ifdef _UNICODE
	return std::string(W2Str(_lpStr, unCode).get());
#	else
	return std::string(_lpStr);
#	endif
}

void CFileManager::__SetControl( DuiLib::CControlUI* _pControl, rapidxml::xml_node<TCHAR>* _pNode, 
	DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pPaintManager )
{
	if (!_pControl) return;
	__SetAttrib(_pNode, _pControl, _pWnd, _pPaintManager);
	DuiLib::CTreeNodeUI* pNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetInterface(DUI_CTR_TREENODE));
	DuiLib::CTreeViewUI* pTreeView = static_cast<DuiLib::CTreeViewUI*>(_pControl->GetInterface(DUI_CTR_TREEVIEW));
	if (pTreeView)
	{
		// 只处理根节点
		for (int i = 0; i < pTreeView->GetCount(); ++i)
		{
			DuiLib::CControlUI* pItem = pTreeView->GetItemAt(i);
			DuiLib::CTreeNodeUI* pNode = static_cast<DuiLib::CTreeNodeUI*>(pItem->GetInterface(DUI_CTR_TREENODE));
			if (pNode && !pNode->GetParentNode())
			{
				_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(pNode->GetClass());
				rapidxml::xml_node<TCHAR>* pNewNode = _pNode->document()->allocate_node(rapidxml::node_element, pInfo->strName);
				_pNode->append_node(pNewNode);
				__SetControl(pNode, pNewNode, _pWnd, _pPaintManager);
			}
		}
	}
	else if (pNode)
	{
		IControlPlugin* pInterface = get_control_interface(_pWnd);
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
		int nFont = _tstoi(strFont);
		int nDefaultFont = _tstoi(strDefaultFont);
		_ZnString strHor, strFolder, strDottedLine, strCheckBox, strItemButton;
		GetAttrbList(pNode->GetTreeNodeHoriznotal(), strHor, nFont, nDefaultFont); 
		if (!strHor.empty()) __SetAttrib(_pNode, _T("horizattr"), strHor.c_str());
		GetAttrbList(pNode->GetFolderButton(), strFolder, nFont, nDefaultFont); 
		if (!strFolder.empty()) __SetAttrib(_pNode, _T("folderattr"), strFolder.c_str());
		GetAttrbList(pNode->GetDottedLine(), strDottedLine, nFont, nDefaultFont); 
		if (!strDottedLine.empty()) __SetAttrib(_pNode, _T("dotlineattr"), strDottedLine.c_str());
		GetAttrbList(pNode->GetCheckBox(), strCheckBox, nFont, nDefaultFont); 
		if (!strCheckBox.empty()) __SetAttrib(_pNode, _T("checkboxattr"), strCheckBox.c_str());
		GetAttrbList(pNode->GetItemButton(), strItemButton, nFont, nDefaultFont); 
		if (!strItemButton.empty()) __SetAttrib(_pNode, _T("itemattr"), strItemButton.c_str());
		for (int i = 0; i < pNode->GetCountChild(); ++i)
		{
			DuiLib::CTreeNodeUI* pItem = pNode->GetChildNode(i);
			_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(pItem->GetClass());
			rapidxml::xml_node<TCHAR>* pNewNode = _pNode->document()->allocate_node(rapidxml::node_element, pInfo->strName);
			_pNode->append_node(pNewNode);
			__SetControl(pItem, pNewNode, _pWnd, _pPaintManager);
		}
	}
	else
	{
		DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(_pControl->GetInterface(DUI_CTR_LIST));
		if (pList)
		{
			DuiLib::CControlUI* pItem = pList->GetHeader();
			_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(pItem->GetClass());
			rapidxml::xml_node<TCHAR>* pNewNode = _pNode->document()->allocate_node(rapidxml::node_element, pInfo->strName);
			_pNode->append_node(pNewNode);
			__SetControl(pItem, pNewNode, _pWnd, _pPaintManager);
		}
		DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(_pControl->GetInterface(DUI_CTR_CONTAINER));
		if (pContainer) 
		{
			for (int i = 0; i < pContainer->GetCount(); ++i)
			{
				DuiLib::CControlUI* pItem = pContainer->GetItemAt(i);
				_SETTINGS_CONTROL* pInfo = config.GetControlUseClassName(pItem->GetClass());
				if (!pInfo) continue;
				rapidxml::xml_node<TCHAR>* pNewNode = _pNode->document()->allocate_node(rapidxml::node_element, pInfo->strName);
				_pNode->append_node(pNewNode);
				__SetControl(pItem, pNewNode, _pWnd, _pPaintManager);
			}
		}
	}
	// 如果Control没有属性,XML会简写为<Control/> 这种格式会导致界面库解析XML错误必须加一空格如<Control />才行
	if (!_pNode->first_attribute())
	{
		size_t newsize = _pNode->name_size() + 2;
		TCHAR* pStr = _pNode->document()->allocate_string(0, newsize);
		_tcscpy_s(pStr, newsize, _pNode->name());
		_tcscat_s(pStr, newsize, _T(" "));
		_pNode->name(pStr, newsize - 1);
	}
}

template<typename T>
void CFileManager::__RepairXml( rapidxml::xml_node<T>* _pNode )
{
	// 如果Control没有属性没有子节点,XML会简写为<Control/> 这种格式会导致界面库解析XML错误必须加一空格如<Control />才行
	bool bHasChild = false;
	for (rapidxml::xml_node<T>* pChild = _pNode->first_node(); pChild; pChild = pChild->next_sibling())
	{
		__RepairXml(pChild);
		bHasChild = true;
	}
	if (!bHasChild && !_pNode->first_attribute())
	{
		size_t newsize = _pNode->name_size() + 2;
		T* pStr = _pNode->document()->allocate_string(0, newsize);
		strcpy_s(pStr, newsize, _pNode->name());
		strcat_s(pStr, newsize, " ");
		_pNode->name(pStr, newsize - 1);
	}
}

void CFileManager::__HandleQuotes( std::string& str )
{
	// rapidxml 的多重引号格式是 ' " &apos; &apos; " '
	// 界面库的xml的多重引号格式是 " &quot; '  ' &quot;"
	const static std::string strQuot("&quot;");
	const static std::string strApos("&apos;");
	const static std::string strQuots("\"");
	const static std::string strAposs("\'");
	for (std::string::size_type begin = str.find('\'', 0), end = str.find('\'', begin + 1); 
		begin > 0 && end > 0 && end > begin;
		begin =  str.find('\'', end + 1),end = str.find('\'', begin + 1))
	{
		bool bChange = false;
		while(true)
		{
			std::string::size_type quot = str.find(strQuots, begin + 1);
			bool bBreak = true;
			if (quot > 0 && quot < end)
			{
				str.replace(quot, strQuots.size(), strQuot);
				end += strQuot.size() - strQuots.size();
				bBreak = false;
				bChange = true;
			}
			std::string::size_type apos = str.find(strApos, begin + 1);
			if (apos > 0 && apos < end)
			{
				str.replace(apos, strApos.size(), strAposs);
				end -= strApos.size() - strAposs.size();
				bBreak = false;
				bChange = true;
			}

			if (bBreak) break;
		}
		if (bChange)
		{
			str.replace(begin, 1, 1, '\"');
			str.replace(end, 1, 1, '\"');
		}
	}
}

void CFileManager::SetDefaultAndFont( DuiLib::CControlUI* _pControl, DuiLib::CPaintManagerUI* _pManager, int _nFontID, int _nDefaultFontID )
{
	IControlPlugin* pInterface = get_control_interface(_pControl);
	if (pInterface)
	{
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("font"));
		DuiLib::CDuiString strItemFont = pInterface->GetAttribute(_T("itemfont"));
		int nFont = _tstoi(strFont);
		int nItemFont = _tstoi(strFont);
		if (nFont == -1 || strFont.IsEmpty()) 
			nFont = _nDefaultFontID;
		else 
			nFont = _nFontID + nFont;
		strFont.Format(_T("%d"), nFont);
		_pControl->SetAttribute(_T("font"), strFont);
		if (nItemFont == -1 || strItemFont.IsEmpty())
			nItemFont = _nDefaultFontID;
		else
			nItemFont = _nFontID + nItemFont;
		strItemFont.Format(_T("%d"), nItemFont);
		_pControl->SetAttribute(_T("itemfont"), strItemFont);
	}
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(_pControl->GetClass());
	if (pSC)
	{
		DuiLib::CDuiString strName;
		strName.Format(_T("_%d_%s"), _nFontID, pSC->strName);
		LPCTSTR lpDefaultAttributes = _pManager->GetDefaultAttributeList(strName);
		if (lpDefaultAttributes) 
			_pControl->ApplyAttributeList(lpDefaultAttributes);
	}

	DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(_pControl->GetInterface(DUI_CTR_CONTAINER));
	if (pContainer)
	{
		if (pContainer->GetHorizontalScrollBar())
		{
			DuiLib::CDuiString strName;
			strName.Format(_T("_%d_%s"), _nFontID, STR_HSCROLL);
			LPCTSTR lpDefaultAttributes = _pManager->GetDefaultAttributeList(strName);
			if (lpDefaultAttributes) 
				pContainer->GetHorizontalScrollBar()->ApplyAttributeList(lpDefaultAttributes);
		}
		if (pContainer->GetVerticalScrollBar())
		{
			DuiLib::CDuiString strName;
			strName.Format(_T("_%d_%s"), _nFontID, STR_VSCROLL);
			LPCTSTR lpDefaultAttributes = _pManager->GetDefaultAttributeList(strName);
			if (lpDefaultAttributes) 
				pContainer->GetVerticalScrollBar()->ApplyAttributeList(lpDefaultAttributes);
		}
		for (int i = 0; i < pContainer->GetCount(); ++i)
			SetDefaultAndFont(pContainer->GetItemAt(i), _pManager, _nFontID, _nDefaultFontID);
	}
	DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(_pControl->GetInterface(DUI_CTR_LIST));
	if (pList)
		SetDefaultAndFont(pList->GetHeader(), _pManager, _nFontID, _nDefaultFontID);
}

void CFileManager::SetDefaultAndFont( DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager )
{
	IControlPlugin* pInterface = get_control_interface(_pWnd);
	if (pInterface)
	{
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
		SetDefaultAndFont(_pControl, _pManager, _tstoi(strFont), _tstoi(strDefaultFont));
	}
}

void CFileManager::__SaveFont( DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, rapidxml::xml_node<TCHAR>* _pWndNode )
{
	IControlPlugin* pInterface = get_control_interface(_pWnd);
	if (pInterface)
	{
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		DuiLib::CDuiString strDefaultFont = pInterface->GetAttribute(_T("defaultfont"));
		int nFont = _tstoi(strFont);
		int nDefaultFont = _tstoi(strDefaultFont);
		for (int i = nFont; i < nFont + 100; ++i)
		{
			DuiLib::TFontInfo* pFI = _pManager->GetFontInfo(i);
			if (!pFI) continue;
			if (pFI->hFont == _pManager->GetDefaultFontInfo()->hFont) continue;
			rapidxml::xml_node<TCHAR>* pFont = _pWndNode->document()->allocate_node(rapidxml::node_element, _T("Font"));
			_pWndNode->append_node(pFont);
			DuiLib::CDuiString strTmp;
			strTmp.Format(_T("%d"), i - nFont);
			rapidxml::xml_attribute<TCHAR>* pID = _pWndNode->document()->allocate_attribute(_T("id"), _pWndNode->document()->allocate_string(strTmp.GetData()));
			pFont->append_attribute(pID);
			rapidxml::xml_attribute<TCHAR>* pName = _pWndNode->document()->allocate_attribute(_T("name"), _pWndNode->document()->allocate_string(pFI->sFontName));
			pFont->append_attribute(pName);
			strTmp.Format(_T("%d"), pFI->iSize);
			rapidxml::xml_attribute<TCHAR>* pSize = _pWndNode->document()->allocate_attribute(_T("size"), _pWndNode->document()->allocate_string(strTmp.GetData()));
			pFont->append_attribute(pSize);
			if (pFI->bBold)
			{
				rapidxml::xml_attribute<TCHAR>* pBold = _pWndNode->document()->allocate_attribute(_T("bold"), _T("true"));
				pFont->append_attribute(pBold);
			}
			if (pFI->bItalic)
			{
				rapidxml::xml_attribute<TCHAR>* pItalic = _pWndNode->document()->allocate_attribute(_T("italic"), _T("true"));
				pFont->append_attribute(pItalic);
			}
			if (pFI->bUnderline)
			{
				rapidxml::xml_attribute<TCHAR>* pUnderline = _pWndNode->document()->allocate_attribute(_T("underline"), _T("true"));
				pFont->append_attribute(pUnderline);
			}
			IWindowSharedData* pWSD = get_wnd_shared_data(_pWnd);
			if (pWSD && pWSD->IsSharedFont(i))
			{
				rapidxml::xml_attribute<TCHAR>* pShared = _pWndNode->document()->allocate_attribute(_T("shared"), _T("true"));
				pFont->append_attribute(pShared);
			}
			if (i == nDefaultFont)
			{
				rapidxml::xml_attribute<TCHAR>* pDefault = _pWndNode->document()->allocate_attribute(_T("default"), _T("true"));
				pFont->append_attribute(pDefault);
			}
		}
	}
}

void CFileManager::__SaveDefault( DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, rapidxml::xml_node<TCHAR>* _pWndNode )
{
	IControlPlugin* pInterface = get_control_interface(_pWnd);
	if (pInterface)
	{
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		int nFont = _tstoi(strFont);
		_SETTINGS_CONTROLS vecControl = config.Settings().vecControl;
		_SETTINGS_CONTROL con;
		con.strName = _T("VScrollBar");
		vecControl.push_back(con);
		con.strName = _T("HScrollBar");
		vecControl.push_back(con);
		for (_SETTINGS_CONTROLS::iterator it = vecControl.begin(); it != vecControl.end(); ++it)
		{
			DuiLib::CDuiString strName;
			strName.Format(_T("_%d_%s"), nFont, it->strName);
			LPCTSTR lpDefaultAttributes = _pManager->GetDefaultAttributeList(strName);
			if (lpDefaultAttributes)
			{
				rapidxml::xml_node<TCHAR>* pDefault = _pWndNode->document()->allocate_node(rapidxml::node_element, _T("Default"));
				_pWndNode->append_node(pDefault);
				rapidxml::xml_attribute<TCHAR>* pName = _pWndNode->document()->allocate_attribute(_T("name"), _pWndNode->document()->allocate_string(it->strName));
				pDefault->append_attribute(pName);
				_ZnString strAttr(lpDefaultAttributes);
				__VirAttrToAttr(strAttr, nFont);
				rapidxml::xml_attribute<TCHAR>* pValue = _pWndNode->document()->allocate_attribute(_T("value"), _pWndNode->document()->allocate_string(strAttr.c_str()));
				pDefault->append_attribute(pValue);
				IWindowSharedData* pWSD = get_wnd_shared_data(_pWnd);
				if (pWSD && pWSD->IsSharedDefault(strName))
				{
					rapidxml::xml_attribute<TCHAR>* pShared = _pWndNode->document()->allocate_attribute(_T("shared"), _T("true"));
					pDefault->append_attribute(pShared);
				}
			}
		}
	}
}

void CFileManager::DeleteAllDefaultAndFont( DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager )
{
	IControlPlugin* pInterface = get_control_interface(_pWnd);
	if (pInterface)
	{
		DuiLib::CDuiString strFont = pInterface->GetAttribute(_T("fontid"));
		int nFont = _tstoi(strFont);
		for (int i = nFont; i < nFont + 100; ++i)
			_pManager->RemoveFont(i, false);

		_SETTINGS_CONTROLS vecControl = config.Settings().vecControl;
		_SETTINGS_CONTROL con;
		con.strName = STR_VSCROLL;
		vecControl.push_back(con);
		con.strName = STR_HSCROLL;
		vecControl.push_back(con);
		for (_SETTINGS_CONTROLS::iterator it = vecControl.begin(); it != vecControl.end(); ++it)
		{
			DuiLib::CDuiString strName;
			strName.Format(_T("_%d_%s"), nFont, it->strName);
			_pManager->RemoveDefaultAttributeList(strName, false);
		}
	}
}

void CFileManager::__GetDefaultAttr( DuiLib::CPaintManagerUI* _pManager, _SETTINGS_CONTROL* _pControl, int _nFont, std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr )
{
	if (!_pControl) return;
	DuiLib::CDuiString strName;
	strName.Format(_T("_%d_%s"), _nFont, _pControl->strName);
	LPCTSTR lpAttr = _pManager->GetDefaultAttributeList(strName);
	if (lpAttr) split_attrib_list(lpAttr, _mapAttr);
}

void CFileManager::__AttrToVirAttr( _ZnString& _str, int _nFont )
{
	std::map<DuiLib::CDuiString, DuiLib::CDuiString> mapAttr;
	split_attrib_list(_str.c_str(), mapAttr);
	bool bChange = false;
	auto itFont = mapAttr.find(_T("font"));
	if (itFont != mapAttr.end()) 
	{
		itFont->second.Format(_T("%d"), _tstoi(itFont->second.GetData()) + _nFont);
		bChange = true;
	}
	auto itItemFont = mapAttr.find(_T("itemfont"));
	if (itItemFont != mapAttr.end()) 
	{
		itItemFont->second.Format(_T("%d"), _tstoi(itItemFont->second.GetData()) + _nFont);
		bChange = true;
	}
	if (bChange) create_attrib_list(_str, mapAttr);
}

void CFileManager::__VirAttrToAttr( _ZnString& _str, int _nFont )
{
	std::map<DuiLib::CDuiString, DuiLib::CDuiString> mapAttr;
	split_attrib_list(_str.c_str(), mapAttr);
	bool bChange = false;
	auto itFont = mapAttr.find(_T("font"));
	if (itFont != mapAttr.end()) 
	{
		itFont->second.Format(_T("%d"), _tstoi(itFont->second.GetData()) - _nFont);
		bChange = true;
	}
	auto itItemFont = mapAttr.find(_T("itemfont"));
	if (itItemFont != mapAttr.end()) 
	{
		itItemFont->second.Format(_T("%d"), _tstoi(itItemFont->second.GetData()) - _nFont);
		bChange = true;
	}
	if (bChange) create_attrib_list(_str, mapAttr);
}

void CFileManager::GetAttrbList( DuiLib::CControlUI* _pControl, _ZnString& _strAttrb, int _nFontID, int _nDefaultID )
{
	if (!_pControl) return;
	IControlPlugin* pInterface = get_control_interface(_pControl);
	if (!pInterface) return;
	_SETTINGS_CONTROL* pSC = config.GetControlUseClassName(_pControl->GetClass());
	if (!pSC) return;
	_SETTINGS_CONTROLS vecControls;
	config.GetAttribs(pSC, vecControls);
	_ZnSStream ss;
	for (_SETTINGS_CONTROLS::iterator itControl = vecControls.begin(); itControl != vecControls.end(); ++itControl)
	{
		for (_SETTINGS_ATTRIBS::iterator it = itControl->vecAttrib.begin(); it != itControl->vecAttrib.end(); ++it)
		{
			DuiLib::CDuiString strValue = pInterface->GetAttribute(it->strName);
			if (!strValue.IsEmpty() && strValue != it->strDefaultValue || 
				strValue.IsEmpty() && !it->strDefaultValue.IsEmpty() && it->strDefaultValue != _T("false"))
			{
				if (it->strName == _T("font") || it->strName == _T("itemfont"))
				{
					DuiLib::CDuiString strFont;
					int nFont = _tstoi(strValue) - _nFontID;
					int nDefaultFont = _nDefaultID - _nFontID;
					if (nFont < 0) nFont = -1;
					if (nDefaultFont < 0) nDefaultFont = -1;
					if (nFont == -1 || nFont == nDefaultFont) continue;
					strFont.Format(_T("%d"), nFont);
					ss << it->strName.GetData() << _T("=\"") << strFont.GetData() << _T("\" ");
				}
				else
					ss << it->strName.GetData() << _T("=\"") << strValue.GetData() << _T("\" ");
			}
		}
	}
	_strAttrb = ss.str();
}
