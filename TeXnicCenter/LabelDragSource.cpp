#include "stdafx.h"
#include "resource.h"

#include "OleDrop.h"
#include "LabelDragSource.h"
#include "structureparser.h"

LabelDragSource::LabelDragSource(void)
{
}

LabelDragSource::~LabelDragSource(void)
{
}

void LabelDragSource::MapKeyStateToFormat( UINT key, const FormatFunctionType& func )
{
	ASSERT(key_to_format_.find(key) == key_to_format_.end());
	key_to_format_.insert(std::make_pair(key,func));
}

void LabelDragSource::RemoveKeyStateToFormat( UINT keystate )
{
	key_to_format_.erase(keystate);
}

void LabelDragSource::ClearKeyStateToFormat()
{
	key_to_format_.clear();
}

void LabelDragSource::MapKeyStateToMessage( UINT keystate, UINT id )
{
	ASSERT(key_to_message_.find(keystate) == key_to_message_.end());
	key_to_message_.insert(std::make_pair(keystate,id));
}

void LabelDragSource::RemoveKeyStateToMessage( UINT key )
{
	key_to_message_.erase(key);
}

void LabelDragSource::ClearKeyStateToMessage()
{
	key_to_message_.clear();
}

const CString LabelDragSource::GetRef( UINT keystate, const CStructureItem &item ) const
{
	CString text;

	KeyStateFormatLabelContainerType::const_iterator it = key_to_format_.find(keystate);

	if (it != key_to_format_.end())
		text = it->second(item);

	return text;
}

void LabelDragSource::OnEndDragDrop()
{
	static_cast<CFrameWnd*>(AfxGetMainWnd())->SetMessageText(AFX_IDS_IDLEMESSAGE);
	DragSourceBase::OnEndDragDrop();
}

void LabelDragSource::UpdateMessageText( UINT keystate, CString& text, const CStructureItem &item )
{
	CString message;
	text = GetRef(keystate,item);

	KeyStateMessageContainerType::const_iterator it = key_to_message_.find(keystate);

	bool message_loaded = false;

	if (it != key_to_message_.end()) {
		message.LoadString(it->second);
		message_loaded = true;
	}

	bool can_drop = true;

	if (message.IsEmpty()) {
		bool format_found = key_to_format_.find(keystate) != key_to_format_.end();

		if (!format_found) {
			message.LoadString(IDS_NO_ACTION);
			can_drop = false;
		}
		else {
			message = text;
		}
	}

	if (!text.IsEmpty()) {
		int index = message.Find(_T('\n'));

		if (index != -1)
			message.Delete(0,index + 1);

		CString fmt;

		if (index != -1)
			fmt.Format(_T("%s: %s"),message,text);
		else
			fmt = text;

		message = fmt;
	}
	else if (text.IsEmpty() && message_loaded)
		message.Empty();
	
	if (text.IsEmpty() && message.IsEmpty())
		message.LoadString(IDS_NO_LABEL_AVAILABLE);
	
	static_cast<CFrameWnd*>(AfxGetMainWnd())->SetMessageText(message);

	SetCanDrop(can_drop);
}

HRESULT LabelDragSource::DoDragDrop( DWORD effects, LPDWORD result )
{
	const CStructureItem* item = GetDraggedItem();
	CString text;

	KeyStateFormatLabelContainerType::const_iterator it = key_to_format_.find(GetControlAsyncState());
	// There should be a label or the format function shouldn't return an empty string
	bool update = item && (item->HasLabels() || 
		(it != key_to_format_.end()) && !it->second(*item).IsEmpty());

	if (!update) {
		effects = 0;
		static_cast<CFrameWnd*>(AfxGetMainWnd())->SetMessageText(IDS_NO_LABEL_AVAILABLE);
	}
	else
		UpdateMessageText(GetControlAsyncState(),text,*item);

	return DragSourceBase::DoDragDrop(text,effects,result);
}

void LabelDragSource::OnDragKeyStateChanged( UINT keystate )
{
	bool clear = true;

	if (dragged_item_.get()) {
		CString text;
		UpdateMessageText(keystate,text,*dragged_item_);

		clear = text.IsEmpty();
	}

	SetCanDrop(!clear);
	DragSourceBase::OnDragKeyStateChanged(keystate);
}

void LabelDragSource::OnDragGetData(SimpleDataObject* o)
{
	bool clear = true;

	if (dragged_item_.get()) {	
		const CString text = GetRef(GetControlAsyncState(),*dragged_item_);

		if (!text.IsEmpty()) {
			clear = false;
			o->SetData(text,text.GetLength());
		}
	}

	if (clear)
		o->Clear();

	DragSourceBase::OnDragGetData(o);
}