// BiBTeXEntry.h: interface for the CBiBTeXEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_)
#define AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define BIBTEX_ENTRY_SIZE 15

extern const TCHAR* BibTypeVerbose[];

class CBiBTeXEntry : public CObject  
{
public:
	static enum BibType {Book=0, Article, Booklet, Manual, Inproceedings, 
						Conference, Inbook, Incollection, Masterthesis,
						Misc, Phdthesis, Proceedings, Techreport,
						Unpublished, Unknown};


	CBiBTeXEntry(CString key, BibType type=Book);
	virtual ~CBiBTeXEntry();

	BOOL SetField(CString name, CString value, BOOL forceOverwrite=FALSE);
	BOOL GetField(CString name, CString &value);

	BibType GetType() const {return m_Type;}
	void SetType(BibType type) {m_Type = type;}

	CString ToString();
private:
	CMapStringToString m_Fields;
	BibType m_Type;	
	CString m_Key;
};

#endif // !defined(AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_)
