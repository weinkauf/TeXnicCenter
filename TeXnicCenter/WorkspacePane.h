#pragma once

#include "WorkspacePaneBase.h"

/**
 * @brief Dockable workspace pane with an optional toolbar and child client
 *		  window.
 */
class WorkspacePane : 
	public WorkspacePaneBase
{
	DECLARE_DYNAMIC(WorkspacePane)
public:
	WorkspacePane();
	
	/**
	 * @brief Returns the dockable pane client.
	 */
	CWnd* GetClient() const;

	/**
	 * @brief Sets the specified window as dockable pane client.
	 * 
	 * @param p The pointer to the window that should be the client or @c NULL.
	 */
	void SetClient(CWnd* p);

	/**
	 * @brief Sets the specified toolbar.
	 * 
	 * The ownership is maintained by the caller.
	 * 
	 * @param toolBar The toolbar to be set on top of the dockable pane.
	 */
	void SetToolBar(CMFCToolBar* toolBar);

	/**
	 * @brief Returns the dockable pane toolbar.
	 */
	CMFCToolBar* GetToolBar() const;

	/**
	 * @brief Creates a toolbar and assigns it to the dockable pane instance.
	 *
	 * The ownership of the dynamically created toolbar is held by the caller,
	 * who is responsible for deleting the instance.
	 * 
	 * @param type The toolbar runtime class.
	 * @param id The toolbar ID.
	 * @param style The toolbar style.
	 * 
	 * @return Pointer to the created toolbar.
	 */
	CMFCToolBar* CreateToolBar(CRuntimeClass* type, UINT id, UINT style = AFX_DEFAULT_TOOLBAR_STYLE);

protected:
	DECLARE_MESSAGE_MAP()

	virtual void AdjustLayout(CRect& rc);
	void AdjustLayout(void);
	void SetClientFocus();
	
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	void Focus();

private:
	CWnd* client_;
	CMFCToolBar* toolBar_;
};
