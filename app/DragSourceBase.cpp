#include "stdafx.h"
#include "DragSourceBase.h"
#include "OleDrop.h"

template<bool A>
struct KeyState;

template<>
struct KeyState<true>
{
	static short GetState(int key)
	{
		return ::GetAsyncKeyState(key);
	}
};

template<>
struct KeyState<false>
{
	static short GetState(int key)
	{
		return ::GetKeyState(key);
	}
};

namespace {
	template<bool A>
	UINT DoGetControlState()
	{
		typedef KeyState<false> Key;
		UINT state = 0;

		if (Key::GetState(VK_CONTROL) >> 15 & 1) // Ctrl pressed
			state |= MK_CONTROL;

		if (Key::GetState(VK_SHIFT) >> 15 & 1) // Shift pressed
			state |= MK_SHIFT;

		if (Key::GetState(VK_MENU) >> 15 & 1) // Alt pressed
			state |= MK_ALT;

		return state;
	}
}

UINT GetControlSyncState()
{
	return DoGetControlState<false>();
}

UINT GetControlAsyncState()
{
	return DoGetControlState<true>();
}

class ATL_NO_VTABLE DragSourceBase::DragSource :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDropSourceImpl<DragSource>
{
	CPoint pt_;
	DragSourceBase* nav_;
	UINT keystate_;
	static HCURSOR stop_cursor_;

	static HCURSOR GetStopCursor()
	{
		// Use the ole32 crossed circle cursor
		// so we can update the cursor when the key state changes
		if (!stop_cursor_) {
			HMODULE m = ::LoadLibrary(_T("ole32.dll"));
			stop_cursor_ = ::LoadCursorA(m,MAKEINTRESOURCEA(1));

			if (!stop_cursor_)
				stop_cursor_ =::LoadCursor(0,IDC_NO); // Fallback, though this cursor probably won't match the ole32 one
		}

		return stop_cursor_;
	}

public:
	BEGIN_COM_MAP(DragSource)
		COM_INTERFACE_ENTRY(IDropSource)
	END_COM_MAP()

	DragSource()
	: pt_(-1,-1)
	, nav_(0)
	, keystate_(~0U)
	{
	}

	~DragSource()
	{
		::DestroyCursor(stop_cursor_);
	}

	void SetNavigator(DragSourceBase* nav = 0)
	{
		nav_ = nav;
	}

	STDMETHOD(GiveFeedback)(DWORD effect)
	{
		CPoint pt;
		::GetCursorPos(&pt);

		UINT keystate = GetControlAsyncState();

		if (keystate != keystate_) {
			keystate_ = keystate;

			if (nav_)
				nav_->OnDragKeyStateChanged(keystate);
		}

		if (pt != pt_) {
			pt_ = pt;

			if (nav_)
				nav_->OnDragMouseMove(pt);
		}

		ASSERT(nav_);
		HRESULT result;
		
		if (!nav_ || nav_->CanDrop())
			result = __super::GiveFeedback(effect);
		else {
			::SetCursor(GetStopCursor());
			result = S_OK;
		}

		return result;
	}
};

HCURSOR DragSourceBase::DragSource::stop_cursor_;

class ATL_NO_VTABLE DragSourceBase::DragObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public ISimpleDataObjectImpl<DragObject>
{
	DragSourceBase* nav_;

public:
	DragObject()
	: nav_(0)
	{
	}

	void SetNavigator(DragSourceBase* nav = 0)
	{
		nav_ = nav;
	}

	BEGIN_COM_MAP(DragObject)
		COM_INTERFACE_ENTRY(IDataObject)
	END_COM_MAP()

	STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
	{
		if (nav_)
			nav_->OnDragGetData(this);

		return __super::GetData(pformatetcIn,pmedium);
	}
};

DragSourceBase::DragSourceBase(void)
: drag_source_(0)
, data_object_(0)
, can_drop_(false)
{
}

DragSourceBase::~DragSourceBase(void)
{
	if (data_object_) {
		data_object_->SetNavigator();
		VERIFY(data_object_->Release() == 0);
	}

	if (drag_source_) {
		drag_source_->SetNavigator();
		VERIFY(drag_source_->Release() == 0);
	}
}

void DragSourceBase::OnDragKeyStateChanged(UINT keystate )
{
	UNUSED_ALWAYS(keystate);
}

void DragSourceBase::OnDragMouseMove( const CPoint& pt )
{
	UNUSED_ALWAYS(pt);
}

void DragSourceBase::OnDragGetData( SimpleDataObject* o )
{
	UNUSED_ALWAYS(o);
}

HRESULT DragSourceBase::DoDragDrop( const CString& text, DWORD effects, LPDWORD pdwEffect )
{
	HRESULT  result = E_FAIL;

	if (!drag_source_) {
		drag_source_ = new DataDragSource;
		drag_source_->AddRef();
		drag_source_->SetNavigator(this);
	}

	if (drag_source_) {
		can_drop_ = true;

		if (!OnBeginDragDrop())
			effects = 0;

		DataObject* object = GetDataObject();
		ASSERT_POINTER(object,DataObject);

		VERIFY(SUCCEEDED(object->SetData(text,text.GetLength())));

		result = ::DoDragDrop(object,drag_source_,effects,pdwEffect);

		OnEndDragDrop();
	}

	return result;
}

bool DragSourceBase::OnBeginDragDrop()
{
	return true;
}

void DragSourceBase::OnEndDragDrop()
{
}

DragSourceBase::DataObject* DragSourceBase::GetDataObject()
{
	if (!data_object_) {
		DataObject* object = new DataObject;
		object->AddRef();
		object->SetNavigator(this);

		data_object_ = object;
	}
	
	ASSERT_POINTER(data_object_,DataObject);
	return data_object_;
}

bool DragSourceBase::CanDrop() const
{
	return can_drop_;
}

void DragSourceBase::SetCanDrop( bool can_drop /*= true*/ )
{
	can_drop_ = can_drop;
}