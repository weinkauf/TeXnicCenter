#pragma once

UINT GetControlAsyncState();
UINT GetControlSyncState();

template <class T, template<class> class EC>
class ISimpleDataObjectImpl;

class DragSourceBase
{
	bool can_drop_; // Can drop while drag & drop operation, useful when data changes

private:
	class DragSource;
	friend class DragSource;

	typedef CComObjectNoLock<DragSource> DataDragSource;
	DataDragSource* drag_source_;

protected:
	class DragObject;
	friend class DragObject;

	typedef ISimpleDataObjectImpl<DragObject,ATL::CComObjectNoLock> SimpleDataObject;
	typedef CComObjectNoLock<DragObject> DataObject;

private:
	DataObject* data_object_;

protected:
	DragSourceBase(void);
	~DragSourceBase(void);

	DataObject* GetDataObject();

protected:
	virtual void OnDragKeyStateChanged(UINT keystate);
	virtual void OnDragMouseMove(const CPoint& pt);
	virtual void OnDragGetData(SimpleDataObject* o);
	virtual bool OnBeginDragDrop();
	virtual void OnEndDragDrop();

public:
	HRESULT DoDragDrop(const CString& text, DWORD effects, LPDWORD pdwEffect);

	bool CanDrop() const;
	void SetCanDrop(bool can_drop = true);
};
