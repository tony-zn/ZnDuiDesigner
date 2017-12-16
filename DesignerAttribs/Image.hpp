#pragma once
#ifndef __IMAGE_HPP_2015_11_23
#define __IMAGE_HPP_2015_11_23

class CAttr_Image : public DuiLib::CVerticalLayoutUI
{
public:
	CAttr_Image(DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
		: __m_pControl(_pControl)
		, __m_strAttrib(_lpAttrib)
		, __m_strName(_lpName)
		, __m_pFile(nullptr)
		, __m_pButton(nullptr)
	{
		SetFixedHeight(20);
	}
	void DoInit()
	{
		ApplyAttributeList(_T("inset=\"5,0,5,0\" "));
		DuiLib::CHorizontalLayoutUI* pFile = new DuiLib::CHorizontalLayoutUI;
		pFile->ApplyAttributeList(_T("height=\"20\" "));
		DuiLib::CLabelUI* pAttr = new DuiLib::CLabelUI;
		pAttr->ApplyAttributeList(_T("height=\"20\" width=\"80\" align=\"left\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		pAttr->SetText(__m_strName);
		pAttr->SetToolTip(__m_strAttrib);
		__m_pFile = new DuiLib::CRichEditUI;
		__m_pFile->ApplyAttributeList(_T("readonly=\"false\" height=\"20\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		__m_pButton = new DuiLib::CButtonUI;
		__m_pButton->ApplyAttributeList(_T("height=\"20\" width=\"50\" text=\"浏览...\" textcolor=\"#FFCCCCCC\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FF888888\" "));
		pFile->Add(pAttr);
		pFile->Add(__m_pFile);
		pFile->Add(__m_pButton);

		__m_pPostionH = new DuiLib::CHorizontalLayoutUI;
		__m_pPostionH->ApplyAttributeList(_T("inset=\"10,5\" height=\"25\" visible=\"false\" "));
		DuiLib::CLabelUI* pLabel1 = new DuiLib::CLabelUI;
		pLabel1->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"源位置  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pSource = new DuiLib::CRichEditUI;
		__m_pSource->ApplyAttributeList(_T("height=\"20\" width=\"70\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pLabel2 = new DuiLib::CLabelUI;
		pLabel2->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"目标位置  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pDest = new DuiLib::CRichEditUI;
		__m_pDest->ApplyAttributeList(_T("height=\"20\" width=\"70\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		__m_pCornerH = new DuiLib::CHorizontalLayoutUI;
		__m_pCornerH->ApplyAttributeList(_T("inset=\"10,5\" height=\"25\" visible=\"false\" "));
		DuiLib::CLabelUI* pLabel3 = new DuiLib::CLabelUI;
		pLabel3->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"九宫格  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pCorner = new DuiLib::CRichEditUI;
		__m_pCorner->ApplyAttributeList(_T("height=\"20\" width=\"70\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );
		DuiLib::CLabelUI* pLabel4 = new DuiLib::CLabelUI;
		pLabel4->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"透明色  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pMask = new DuiLib::CRichEditUI;
		__m_pMask->ApplyAttributeList(_T("height=\"20\" width=\"70\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		__m_pHoleH = new DuiLib::CHorizontalLayoutUI;
		__m_pHoleH->ApplyAttributeList(_T("inset=\"10,5\" height=\"25\" visible=\"false\" "));
		DuiLib::CLabelUI* pLabel5 = new DuiLib::CLabelUI;
		pLabel5->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"隐藏中间  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pHole = new DuiLib::COptionUI;
		__m_pHole->ApplyAttributeList(_T("width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		DuiLib::CLabelUI* pLabel6 = new DuiLib::CLabelUI;
		pLabel6->ApplyAttributeList(_T("height=\"20\" width=\"110\" text=\"透明度  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pFade = new DuiLib::CRichEditUI;
		__m_pFade->ApplyAttributeList(_T("height=\"20\" width=\"70\" rich=\"false\" textcolor=\"#FFCCCCCC\" multiline=\"false\" inset=\"2,2,2,2\" bordersize=\"1\" bordercolor=\"FF3F3F46\" bkcolor=\"#FF333337\" wanttab=\"false\" wantreturn=\"false\" wantctrlreturn=\"false\" autohscroll=\"true\" ") );

		__m_pTiledH = new DuiLib::CHorizontalLayoutUI;
		__m_pTiledH->ApplyAttributeList(_T("inset=\"10,5\" height=\"25\" visible=\"false\" "));
		DuiLib::CLabelUI* pLabel7 = new DuiLib::CLabelUI;
		pLabel7->ApplyAttributeList(_T("height=\"20\" width=\"60\" text=\"水平平铺  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pXTiled = new DuiLib::COptionUI;
		__m_pXTiled->ApplyAttributeList(_T("width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));
		DuiLib::CLabelUI* pLabel8 = new DuiLib::CLabelUI;
		pLabel8->ApplyAttributeList(_T("height=\"20\" width=\"110\" text=\"垂直平铺  \" align=\"right\" valign=\"vcenter\" textcolor=\"#FFCCCCCC\" "));
		__m_pYTiled = new DuiLib::COptionUI;
		__m_pYTiled->ApplyAttributeList(_T("width=\"20\" height=\"20\" selectedimage=\"file='Attribs/selected.png' dest='5,5,15,15'\" normalimage=\"file='Attribs/unselected.png' dest='5,5,15,15'\""));

		__m_pPostionH->Add(pLabel1);
		__m_pPostionH->Add(__m_pSource);
		__m_pPostionH->Add(pLabel2);
		__m_pPostionH->Add(__m_pDest);
		__m_pCornerH->Add(pLabel3);
		__m_pCornerH->Add(__m_pCorner);
		__m_pCornerH->Add(pLabel4);
		__m_pCornerH->Add(__m_pMask);
		__m_pHoleH->Add(pLabel5);
		__m_pHoleH->Add(__m_pHole);
		__m_pHoleH->Add(pLabel6);
		__m_pHoleH->Add(__m_pFade);
		__m_pTiledH->Add(pLabel7);
		__m_pTiledH->Add(__m_pXTiled);
		__m_pTiledH->Add(pLabel8);
		__m_pTiledH->Add(__m_pYTiled);
		Add(pFile);
		Add(__m_pPostionH);
		Add(__m_pCornerH);
		Add(__m_pHoleH);
		Add(__m_pTiledH);
		if (__m_pControl) 
		{
			__m_pFile->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pButton->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pSource->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pDest->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pCorner->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pMask->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pFade->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pHole->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pXTiled->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			__m_pYTiled->OnNotify += DuiLib::MakeDelegate(this, &CAttr_Image::__EditNotify);
			DuiLib::TFontInfo* pFI = __m_pControl->GetManager()->GetDefaultFontInfo();
			if (pFI) GetManager()->SetDefaultFont(pFI->sFontName, pFI->iSize, pFI->bBold, pFI->bUnderline, pFI->bItalic, true);
		}
		__super::DoInit();
	}
	void DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if (__m_pControl)
		{
			IControlPlugin* pCP = get_control_interface(__m_pControl);
			DuiLib::CDuiString strValue = pCP->GetAttribute(__m_strAttrib);
			if (__m_strValue != strValue)
			{
				__m_strValue = strValue;
				__Split(__m_strValue);
				__ShowCustomSet(!__m_strValue.IsEmpty());
			}
		}
		__super::DoPaint(hDC, rcPaint);
	}
private:
	bool				__EditNotify(LPVOID _lpParam)
	{
		if (!__m_pControl) return true;
		DuiLib::TNotifyUI* pNotify = static_cast<DuiLib::TNotifyUI*>(_lpParam);
		if (!pNotify) return true;
		if (pNotify->pSender == __m_pButton && pNotify->sType == DUI_MSGTYPE_CLICK)
		{
			static TCHAR  szFileName[MAX_PATH] = { 0 };
			OPENFILENAME ofn = { 0 };
			ofn.hwndOwner = GetManager()->GetPaintWindow();
			ofn.lpstrFile = szFileName;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = _T("图片文件(*.bmp;*.jpeg;*.jpg;*.gif;*.png)\0*.bmp;*.jpeg;*.jpg;*.gif;*.png\0\0");
			LPCTSTR _lpFile = GetOpenFileName(&ofn) ? szFileName : nullptr;
			if (_lpFile) 
			{
				__m_pFile->SetText(__ConvertPath(_lpFile));
				__Update();
			}
		}
		else if (pNotify->pSender == __m_pFile && (pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN))
			__Update();
		else if ((pNotify->sType == DUI_MSGTYPE_KILLFOCUS || pNotify->sType == DUI_MSGTYPE_RETURN) && (
			pNotify->pSender == __m_pSource ||
			pNotify->pSender == __m_pDest ||
			pNotify->pSender == __m_pCorner ||
			pNotify->pSender == __m_pMask ||
			pNotify->pSender == __m_pFade ))
			__Update();
		else if (pNotify->sType == DUI_MSGTYPE_SELECTCHANGED && (
			pNotify->pSender == __m_pHole ||
			pNotify->pSender == __m_pXTiled ||
			pNotify->pSender == __m_pYTiled ))
			__Update();
		return true;
	}
	DuiLib::CDuiString	__ConvertPath(LPCTSTR _lpFile)
	{
		DuiLib::CDuiString strPath = _lpFile;
		__m_strResourcePath = get_resource_path(__m_pControl);
		if (!__m_strResourcePath.IsEmpty())
			strPath.Replace(__m_strResourcePath, _T(""));
		return strPath;
	}
	void				__Update()
	{
		_STREAM ss;
		if (!__m_pSource->GetText().IsEmpty())
			ss << _T(" source='") << __m_pSource->GetText().GetData() << _T("'");
		if (!__m_pDest->GetText().IsEmpty())
			ss << _T(" dest='") << __m_pDest->GetText().GetData() << _T("'");
		if (!__m_pCorner->GetText().IsEmpty())
			ss << _T(" corner='") << __m_pCorner->GetText().GetData() << _T("'");
		if (!__m_pMask->GetText().IsEmpty())
			ss << _T(" mask='") << __m_pMask->GetText().GetData() << _T("'");
		if (!__m_pDest->GetText().IsEmpty())
			ss << _T(" fade='") << __m_pFade->GetText().GetData() << _T("'");
		if (__m_pHole->IsSelected())
			ss << _T(" hole='true'");
		if (__m_pXTiled->IsSelected())
			ss << _T(" xtiled='true'");
		if (__m_pYTiled->IsSelected())
			ss << _T(" ytiled='true'");
		_STRING strValue;
		if (ss.str().empty())
			strValue = __m_pFile->GetText().GetData();
		else
		{
			strValue = _T("file='");
			strValue += __m_pFile->GetText().GetData();
			strValue += _T("'");
			strValue += ss.str();
		}
		if (__m_strValue != strValue.c_str())
		{
			__m_strValue = strValue.c_str();
			DuiLib::CDuiString strOld = __m_pControl->GetManager()->GetResourcePath();
			__m_pControl->GetManager()->SetResourcePath(__m_strResourcePath);
			__m_pControl->SetAttribute(__m_strAttrib, __m_strValue);
			__m_pControl->GetManager()->SetResourcePath(strOld);
			__ShowCustomSet(!__m_strValue.IsEmpty());
			GetManager()->SendNotify(this, DUI_MSGTYPE_ATTRIBCHANGE);
		}
	}
	void				__Split(LPCTSTR _lpPath)
	{
		__m_pFile->SetText(_T(""));
		__m_pSource->SetText(_T(""));
		__m_pDest->SetText(_T(""));
		__m_pCorner->SetText(_T(""));
		__m_pMask->SetText(_T(""));
		__m_pFade->SetText(_T(""));
		__m_pHole->Selected(false);
		__m_pXTiled->Selected(false);
		__m_pYTiled->Selected(false);
		_STREAM ss(_lpPath);
		while(ss && !ss.eof())
		{
			_STRING str;
			ss >> str;
			_STRING strName = str.substr(0, str.find_first_of(_T('=')));
			_STRING::size_type pos_begin = str.find_first_of(_T('\'')) + 1;
			_STRING::size_type pos_end = str.find_last_of(_T('\''));
			_STRING strValue = str.substr(pos_begin, pos_end - pos_begin);
			if (strName == _T("file"))
				__m_pFile->SetText(strValue.c_str());
			else if (strName == _T("source"))
				__m_pSource->SetText(strValue.c_str());
			else if (strName == _T("dest"))
				__m_pDest->SetText(strValue.c_str());
			else if (strName == _T("corner"))
				__m_pCorner->SetText(strValue.c_str());
			else if (strName == _T("mask"))
				__m_pMask->SetText(strValue.c_str());
			else if (strName == _T("fade"))
				__m_pFade->SetText(strValue.c_str());
			else if (strName == _T("hole"))
				__m_pHole->Selected(strValue == _T("true"));
			else if (strName == _T("xtiled"))
				__m_pXTiled->Selected(strValue == _T("true"));
			else if (strName == _T("ytiled"))
				__m_pYTiled->Selected(strValue == _T("true"));

		}
		if (__m_pFile->GetText().IsEmpty())
			__m_pFile->SetText(_lpPath);
	}
	void				__ShowCustomSet(bool _bShow)
	{
		__m_pPostionH->SetVisible(_bShow);
		__m_pCornerH->SetVisible(_bShow);
		__m_pHoleH->SetVisible(_bShow);
		__m_pTiledH->SetVisible(_bShow);
		int nHeight = _bShow ? 120 : 20;
		SetFixedHeight(nHeight);
		GetParent()->SetFixedHeight(nHeight);
		GetParent()->GetParent()->SetFixedHeight(nHeight);
	}
private:
	DuiLib::CControlUI*				__m_pControl;
	DuiLib::CDuiString				__m_strAttrib;
	DuiLib::CDuiString				__m_strName;
	DuiLib::CRichEditUI*			__m_pFile;
	DuiLib::CButtonUI*				__m_pButton;
	DuiLib::CRichEditUI*			__m_pSource;
	DuiLib::CRichEditUI*			__m_pDest;
	DuiLib::CRichEditUI*			__m_pCorner;
	DuiLib::CRichEditUI*			__m_pMask;
	DuiLib::CRichEditUI*			__m_pFade;
	DuiLib::COptionUI*				__m_pHole;
	DuiLib::COptionUI*				__m_pXTiled;
	DuiLib::COptionUI*				__m_pYTiled;
	DuiLib::CDuiString				__m_strValue;
	DuiLib::CDuiString				__m_strResourcePath;
	DuiLib::CHorizontalLayoutUI*	__m_pPostionH;
	DuiLib::CHorizontalLayoutUI*	__m_pCornerH;
	DuiLib::CHorizontalLayoutUI*	__m_pHoleH;
	DuiLib::CHorizontalLayoutUI*	__m_pTiledH;
};

#endif