// AbstractLaTeXCommand.h: interface for the CAbstractLaTeXCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
#define AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStyleFile;

/**
 CAbstractLaTeXCommand is the base class for all LaTeX constructs handled by
 CStylefile(Container).
 */
class CAbstractLaTeXCommand : public CObject  
{
public:
	
	DECLARE_DYNAMIC(CAbstractLaTeXCommand)
	CAbstractLaTeXCommand(const CStyleFile *parent, CString &name);
	CAbstractLaTeXCommand(CAbstractLaTeXCommand &cmd);

	virtual ~CAbstractLaTeXCommand();
	/* Returns a pointer to the containing style or class file */
	const CStyleFile *GetStyleFile() const {return m_Parent;};
	/* Returns the LaTeX string representation */
	virtual CString ToLaTeX(BOOL showParString=TRUE) const {return m_Name;}
	/* Returns a unique identifier of the object (may be used as hash value) */
	virtual CString ToString();
	/* Maintain description */
	void SetDescription(CString &desc);
	CString GetDescription() const {return m_Description;};
	/* Name of the command (without any decorations )*/
	CString GetName() const {return m_Name;}
	/* Returns a pointer to the style file which contains this command */
	const CStyleFile *GetParent() const {return m_Parent;}

private:	
	CString				m_Description;
	CString				m_Name;
	const CStyleFile	*m_Parent;
protected:
	void SetName(CString &name);
};

#endif // !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
