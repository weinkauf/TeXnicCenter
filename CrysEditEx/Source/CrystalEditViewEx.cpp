// CrystalEditViewEx.cpp: Implementierung der Klasse CCrystalEditViewEx.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrystalSyntaxParser.h"
#include "CrystalEditViewEx.h"
#include "cedefs.h"


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CCrystalEditViewEx,CCrystalEditView)


CCrystalEditViewEx::CCrystalEditViewEx()
: CCrystalEditView()
{
	m_pSyntaxParser = NULL;
}


DWORD CCrystalEditViewEx::ParseLine( DWORD dwCookie, int nLineIndex, 
																		TEXTBLOCK *pBuf, int &nActualItems )
{
	if( m_pSyntaxParser )
	{
		CCrystalTextBlock	*pTextBlock = 
			pBuf? new CCrystalTextBlock( (CCrystalTextBlock::TEXTBLOCK*)pBuf, nActualItems ) : NULL;
		dwCookie = m_pSyntaxParser->ParseLine( dwCookie, nLineIndex, pTextBlock );
		
		if( pTextBlock )
			delete pTextBlock;

		return dwCookie;
	}
	else
		return 0;
}


CCrystalSyntaxParser *CCrystalEditViewEx::SetSyntaxParser( CCrystalSyntaxParser *pParser )
{
	CCrystalSyntaxParser	*pOldParser = m_pSyntaxParser;

	m_pSyntaxParser = pParser;

	if( pParser )
		pParser->m_pEditViewEx = this;

	return pOldParser;
}
