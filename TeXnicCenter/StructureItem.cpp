#include "stdafx.h"
#include "StructureItem.h"
#include "LatexProject.h"
#include "TeXnicCenter.h"

StructureItem::StructureItem()
: main_project_file_(-1)
, m_nType(generic)
, m_nLine(0)
, m_nParent(-1)
, depth_(-1)
, missing_(false)
{
}

bool StructureItem::IsMainProjectFile() const
{
	if (main_project_file_ == -1)
	{
		if (CLaTeXProject* l = theApp.GetProject())
			main_project_file_ = !CPathTool::IsRelativePath(m_strTitle) && 
				m_strTitle.CompareNoCase(l->GetMainPath()) == 0;
	}

	return main_project_file_ != 0;
}

int StructureItem::GetType() const
{
	return m_nType;
}

void StructureItem::SetType(int val)
{
	m_nType = val;
}

const CString& StructureItem::GetPath() const
{
	return m_strPath;
}

void StructureItem::SetPath(const CString& val)
{
	m_strPath = val;
}

int StructureItem::GetLine() const
{
	return m_nLine;
}

void StructureItem::SetLine(int val)
{
	m_nLine = val;
}

const CString& StructureItem::GetCaption() const
{
	return m_strCaption;
}

void StructureItem::SetCaption(const CString& val)
{
	m_strCaption = val;
}

const CString StructureItem::GetLabel() const
{
	CString text;

	if (!labels_.empty())
		text = labels_[0];

	return text;
}

void StructureItem::SetLabel(const CString& val)
{
	RemoveLabels();
	AddLabel(val);
}

const CString& StructureItem::GetTitle() const
{
	return m_strTitle;
}

void StructureItem::SetTitle(const CString& val)
{
	m_strTitle = val;
}

const CString& StructureItem::GetComment() const
{
	return m_strComment;
}

void StructureItem::SetComment(const CString& val)
{
	m_strComment = val;
}

int StructureItem::GetParent() const
{
	return m_nParent;
}

const StructureItem* StructureItem::GetParent( const StructureItemContainer& items ) const
{
	return !HasParent() ? 0 : &items[GetParent()];
}

void StructureItem::SetParent(int val)
{
	m_nParent = val;
}

const StructureItem::LabelContainer& StructureItem::GetLabels() const
{
	return labels_;
}

StructureItem::LabelContainer& StructureItem::GetLabels()
{
	return labels_;
}

bool StructureItem::HasLabels() const
{
	return !labels_.empty();
}

void StructureItem::AddLabel( const CString& l )
{
	labels_.push_back(l);
}

void StructureItem::RemoveLabels()
{
	labels_.clear();
}

void StructureItem::SetDepth( int depth )
{
	ASSERT(depth >= -1 && depth < MAX_DEPTH);
	depth_ = depth;
}

int StructureItem::GetDepth() const
{
	return depth_;
}

const StructureItemInfo* StructureItem::GetItemInfo() const
{
	return item_info_.get();
}

StructureItemInfo* StructureItem::GetItemInfo()
{
	return item_info_.get();
}

void StructureItem::SetItemInfo( StructureItemInfo* info /*= 0*/ )
{
	item_info_.reset(info);
}

bool StructureItem::HasItemInfo() const
{
	return item_info_ != 0;
}

bool StructureItem::HasParent() const
{
	return m_nParent != -1;
}

void StructureItem::SetMissing( bool missing )
{
	missing_ = missing;
}

bool StructureItem::IsMissing() const
{
	return missing_;
}

bool StructureItem::IsEnvironment( Type type )
{
	bool result;

	switch (type) {
			case unknownEnv:
			case figure:
			case table:
			case itemization:
			case enumeration:
			case description:
			case equation:
			case quote:
			case quotation:
			case verse:
			case center:
				result = true;
				break;
			default:
				result = false;
				break;
	}

	return result;
}

bool StructureItem::IsEnvironment() const
{
	return IsEnvironment(static_cast<Type>(m_nType));
}