/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
* 
* This library is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* This library does not only contain file from us, but also from
* third party developers. Look at the source file headers to get
* detailed information.
*
* If you have further questions visit our homepage
*
*    http://www.ToolsCenter.org
*
********************************************************************/

#ifndef UTILS_PERFORMANCETIMER_H
#define UTILS_PERFORMANCETIMER_H

#ifdef WIN32
	#include <windows.h>
#else
	#include <time.h>
#endif


/** Provides a simple timer to measure code performance.

	Use this as follows:
	@code
		PerformanceTimer MyTimer();

		//some code that you want to measure

		printf("RunTime = %g seconds\n", MyTimer.ElapsedTime());
	@endcode

	To see the printf-output, you can run TeXnicCenter from a command line like this:
	@code
		TeXnicCenter.exe > out.txt
	@endcode

	Of course, you may also use a TRACE macro, but that works only in Debug mode,
	where performance measurements are less useful.

@author Tino Weinkauf
*/
class PerformanceTimer
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
	PerformanceTimer();
	virtual ~PerformanceTimer();

//Methods
public:
	///Returns elapsed time since construction of the object in seconds
	float ElapsedTime() const;

	///Returns elapsed time since construction of the object in seconds and resets the clock.
	inline float ElapsedTimeAndReset()
	{
		const float Time = ElapsedTime();
		Reset();
		return Time;
	}

	///Resets (or re-starts) the clock
	void Reset();

//Attributes
protected:
	#ifdef WIN32
		__int64 Start;
		__int64 Frequency;
	#else
		clock_t Start;
		clock_t Frequency;
	#endif
};


#endif //UTILS_PERFORMANCETIMER_H

