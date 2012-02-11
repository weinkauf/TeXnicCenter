#include "stdafx.h"

#include "resource.h"
#include "FileTreeCtrl.h"
#include "FileViewPane.h"
#include "PrivateToolBar.h"
#include "SearchToolBarEditButton.h"

IMPLEMENT_DYNAMIC(FileViewPane, WorkspacePane)

BEGIN_MESSAGE_MAP(FileViewPane, WorkspacePane)
	ON_WM_CREATE()
END_MESSAGE_MAP()

FileViewPane::FileViewPane()
	: tree_(new FileTreeCtrl)
{
	SetClient(tree_.get());
}

FileViewPane::~FileViewPane()
{
}

FileTreeCtrl* FileViewPane::GetTreeCtrl()
{
	return tree_.get();
}

const FileTreeCtrl* FileViewPane::GetTreeCtrl() const
{
	return tree_.get();
}

int FileViewPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (WorkspacePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!tree_->Create(this))
		return -1;

	return 0;
}

CProjectView* FileViewPane::GetProjectView()
{
	return tree_.get();
}
