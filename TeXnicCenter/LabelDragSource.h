#pragma once

#include <map>
#include <functional>
#include <memory>

#include "DragSourceBase.h"

class CStructureItem;

class LabelDragSource :
	public DragSourceBase
{
	std::auto_ptr<CStructureItem> dragged_item_;

public:
	typedef std::tr1::function<const CString (const CStructureItem&)> FormatFunctionType;

private:
	// Maps a key combination such as 0, MK_SHIFT, MK_SHIFT|MK_CONTROL
	// to a label formatting function
	typedef std::map<UINT,FormatFunctionType> KeyStateFormatLabelContainerType;
	KeyStateFormatLabelContainerType key_to_format_;

	typedef std::map<UINT,UINT> KeyStateMessageContainerType;
	KeyStateMessageContainerType key_to_message_;

public:
	LabelDragSource();
	~LabelDragSource();

	CStructureItem* GetDraggedItem() { return dragged_item_.get(); }
	const CStructureItem* GetDraggedItem() const { return dragged_item_.get(); }
	void SetDraggedItem(CStructureItem* item = 0) { dragged_item_.reset(item); }

	void MapKeyStateToFormat(UINT key, const FormatFunctionType& func);
	void RemoveKeyStateToFormat(UINT key);
	void ClearKeyStateToFormat();
	void MapKeyStateToMessage(UINT key, UINT id);
	void RemoveKeyStateToMessage(UINT key);
	void ClearKeyStateToMessage();
	HRESULT DoDragDrop(DWORD effects, LPDWORD result);

protected:
	const CString GetRef( UINT keystate, const CStructureItem &item ) const;

	void OnDragKeyStateChanged(UINT keystate);
	void OnEndDragDrop();
	void OnDragGetData(SimpleDataObject* o);

	void UpdateMessageText( UINT keystate, CString& text, const CStructureItem &item );
};
