////////////////////////////////////////////////////////////////////////////
//	File:		CrystalParser.cpp
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Implementation of the CCrystalParser class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrysEditEx.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrysEditEx.h,v $
 * Revision 1.1  2003/12/16 20:04:38  svenwiegand
 * Implemented Feature 726766: "Option for selecting the language for the GUI"
 *
 *
 ********************************************************************/

#pragma once

#include "CeDefs.h"

void CRYSEDIT_CLASS_DECL CrystalEditExSetResourceHandle(HINSTANCE hResourceDll);
void CRYSEDIT_CLASS_DECL CrystalEditExResetResourceHandle();