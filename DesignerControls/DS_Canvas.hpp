#pragma once
#ifndef __DS_CANVAS_H_2015_11_03
#define __DS_CANVAS_H_2015_11_03

// 实现双向滚动条同时可用

class CDS_CanvasUI : public DuiLib::CVerticalLayoutUI, public CControlPlugin_ContainerUI
{
public:
	CDS_CanvasUI()
	{
		SetControl(this);
	}
	LPCTSTR		GetClass() const
	{
		return _T("Canvas");
	}
	LPVOID		GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(_T("Canvas"), pstrName) == 0)
			return this;
		else if (_tcscmp(DUI_CTR_CONTROL_PLUGIN_BASE, pstrName) == 0)
			return static_cast<IControlPlugin*>(this);
		return DuiLib::CVerticalLayoutUI::GetInterface(pstrName);
	}
	void		SetPos(RECT rc, bool bNeedInvalidate = true)
	{
		DuiLib::CVerticalLayoutUI::SetPos(rc, bNeedInvalidate);
		int nMaxWidth = 0;
		for (int i = 0; i < m_items.GetSize(); ++i)
		{
			DuiLib::CControlUI* pItem = static_cast<DuiLib::CControlUI*>(m_items.GetAt(i));
			if (pItem->IsFloat()) continue;;
			RECT rcPos = pItem->GetPos();
			int nWidth = rcPos.right - rcPos.left;
			if (nMaxWidth < nWidth) nMaxWidth = nWidth;
		}
		if (GetHorizontalScrollBar())
		{
			RECT rcScroll = GetHorizontalScrollBar()->GetPos();
			int nRange = nMaxWidth - (rc.right - rc.left) + (rcScroll.bottom - rcScroll.top);
			if (nRange > 0)
			{
				GetHorizontalScrollBar()->SetScrollRange(nRange);
				GetHorizontalScrollBar()->SetVisible(true);
				RECT rcScrollBarPos = { rc.left, rc.bottom - GetHorizontalScrollBar()->GetFixedHeight(), rc.right, rc.bottom };
				if (::memcmp(&rcScrollBarPos, &rcScroll, sizeof(RECT)) != 0)
					GetHorizontalScrollBar()->SetPos(rcScrollBarPos);
			}
			else
				GetHorizontalScrollBar()->SetVisible(false);
		}
		DuiLib::CVerticalLayoutUI::SetPos(rc, bNeedInvalidate);
	}
};

#endif