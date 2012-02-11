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

#include "stdafx.h"
#include "PerformanceTimer.h"


PerformanceTimer::PerformanceTimer()
{
	#ifdef WIN32
		QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	#else
		Frequency = CLOCKS_PER_SEC;
	#endif

	Reset();
}


PerformanceTimer::~PerformanceTimer()
{

}


void PerformanceTimer::Reset()
{
	#ifdef WIN32
		QueryPerformanceCounter((LARGE_INTEGER*)&Start);
	#else
		Start = clock();
	#endif
}


float PerformanceTimer::ElapsedTime() const
{
	#ifdef WIN32
		__int64 End;
		QueryPerformanceCounter((LARGE_INTEGER*)&End);
	#else
		clock_t End = clock();
	#endif

	return (float)(End - Start) / Frequency;
}

