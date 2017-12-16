#pragma once
#ifndef __DS_AUTOWIDTHHLAYOUT_HPP_2015_11_16
#define __DS_AUTOWIDTHHLAYOUT_HPP_2015_11_16

class CDS_AutoWidthHLayout : public DuiLib::CHorizontalLayoutUI, public CControlPlugin_HorizontalLayoutUI
{
public:
	CDS_AutoWidthHLayout()
	{
		SetControl(this);
	}
	LPCTSTR		GetClass() const
	{
		return _T("AutoWidthHLayout");
	}
	LPVOID		GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(_T("AutoWidthHLayout"), pstrName) == 0)
			return this;
		else if (_tcscmp(DUI_CTR_CONTROL_PLUGIN_BASE, pstrName) == 0)
			return static_cast<IControlPlugin*>(this);
		return DuiLib::CHorizontalLayoutUI::GetInterface(pstrName);
	}
	void		SetPos(RECT rc, bool bNeedInvalidate = true)
	{
		//SIZE sz = GetTextSize(GetManager()->GetPaintDC(), GetManager()->GetFont(GetFont()), GetText());
		//SetFixedWidth(sz.cx + m_rcTextPadding.left + m_rcTextPadding.right);
		DuiLib::CHorizontalLayoutUI::SetPos(rc);

		int nTotalWidth = 0;
		for (int i = 0; i < m_items.GetSize(); ++i)
		{
			DuiLib::CControlUI* pItem = static_cast<DuiLib::CControlUI*>(m_items.GetAt(i));
			if (pItem->IsFloat()) continue;;
			RECT rcPos = pItem->GetPos();
			RECT rcPadding = pItem->GetPadding();
			int nWidth = rcPos.right - rcPos.left + rcPadding.left + rcPadding.right;
			nTotalWidth += nWidth;
		}
		nTotalWidth += m_rcInset.left + m_rcInset.right;
		if (m_cxyFixed.cx != nTotalWidth)
			SetFixedWidth(nTotalWidth);
		DuiLib::CHorizontalLayoutUI::SetPos(rc);
	}
};

#endif