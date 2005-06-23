// BibView.cpp: implementation of the CBibView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "texniccenter.h"
#include "global.h"
#include "BibView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBibView::CBibView()
{

}

CBibView::~CBibView()
{

}

// Took code from CEnvironmentView
void CBibView::OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint) {
	switch( lHint )
	{
		case COutputDoc::hintParsingFinished:
			{
				//How many items do we have in this view currently?
				//If none, i.e. first filled after loading a project, then expand the zeroth level
				const bool bExpandAll = (GetCount() == 0);

				//-----------------------------------------------------------
				// remember expanded items
				CString 			strSelectedItem = GetItemPath( GetSelectedItem() );
				CStringArray	astrExpandedItems;
				GetExpandedItems( astrExpandedItems );

				//-----------------------------------------------------------
				// fill view with parsing results
				const CStructureItemArray &a = GetProject()->m_aStructureItems;

				// initialization
				DeleteAllItems();

				HTREEITEM hBibParent = 0;

				// fill view
				for(int i = 0; i < a.GetSize(); i++ )
				{
					const CStructureItem &si = a.GetAt( i );

					switch( si.m_nType )
					{
						// thanks god we are sure, that a bibFile predecesses a bibItem
						case CStructureParser::bibFile:
						{
							hBibParent = InsertItem(
													si.m_strPath,
													si.m_nType, si.m_nType,
													TVI_ROOT, TVI_SORT );
							SetItemData( hBibParent, i );
							break;
						}

						case CStructureParser::bibItem:
						{				
							ASSERT(hBibParent != 0);
							HTREEITEM hItem = InsertItem( 
														si.m_strCaption, 
														si.m_nType, si.m_nType, 
														hBibParent, TVI_SORT );
							SetItemData( hItem, i );
							break;
						}
					}
				}

				//-----------------------------------------------------------
				//try to expand items
				if (!bExpandAll)
				{
					ExpandItems( astrExpandedItems );
					SelectItem( GetItemByPath( strSelectedItem ) );
				}
				else
				{
					ExpandItemsByLevel(0);
				}
			}
			break;
	}
}
