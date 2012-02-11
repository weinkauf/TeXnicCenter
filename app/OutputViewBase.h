#pragma once

/** Base class for all output views.
*/
class OutputViewBase
{
public:
	OutputViewBase()
		:bUpdateIsEnabled(true)
	{}

	virtual ~OutputViewBase()
	{}

	///Clears the content of the view.
	virtual void Clear() = 0;

	/** Enables or disables the redrawing of this view.

		When disabled, the view will not redraw.
		If enabled, it will redraw. Derived classes should do a little bit more.
	*/
	virtual void EnableUpdate(const bool bEnable)
	{
		bUpdateIsEnabled = bEnable;
	}

protected:
	///Whether update (redraw) is currently enabled.
	bool bUpdateIsEnabled;
};
