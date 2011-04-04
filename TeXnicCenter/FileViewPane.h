#pragma once

#include <memory>

#include "WorkspacePane.h"

class CProjectView;
class FileTreeCtrl;
class PrivateToolBar;

class FileViewPane
	: public WorkspacePane
{
	DECLARE_DYNAMIC(FileViewPane)
public:
	FileViewPane();
	~FileViewPane();

	FileTreeCtrl* GetTreeCtrl();
	const FileTreeCtrl* GetTreeCtrl() const;
	CProjectView* GetProjectView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	const std::unique_ptr<FileTreeCtrl> tree_;
	std::unique_ptr<PrivateToolBar> toolBar_;
};
