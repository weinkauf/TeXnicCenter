#pragma once

#include <map>
#include <functional>
#include <memory>

#include "DragSourceBase.h"

class StructureItem;

class LabelDragSource :
	public DragSourceBase
{
	std::auto_ptr<StructureItem> dragged_item_;

public:
	typedef std::tr1::function<const CString (const StructureItem&)> FormatFunctionType;

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

	StructureItem* GetDraggedItem() { return dragged_item_.get(); }
	const StructureItem* GetDraggedItem() const { return dragged_item_.get(); }
	void SetDraggedItem(StructureItem* item = 0) { dragged_item_.reset(item); }

	void MapKeyStateToFormat(UINT key, const FormatFunctionType& func);
	void RemoveKeyStateToFormat(UINT key);
	void ClearKeyStateToFormat();
	void MapKeyStateToMessage(UINT key, UINT id);
	void RemoveKeyStateToMessage(UINT key);
	void ClearKeyStateToMessage();
	HRESULT DoDragDrop(DWORD effects, LPDWORD result);

protected:
	const CString GetRef( UINT keystate, const StructureItem &item ) const;

	void OnDragKeyStateChanged(UINT keystate);
	void OnEndDragDrop();
	void OnDragGetData(SimpleDataObject* o);

	void UpdateMessageText( UINT keystate, CString& text, const StructureItem &item );
};
