////////////////////////////////////////////////////////////////////////////
//	File:		CrystalTextBlock.h
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Interface of the CCrystalTextBlock class, a part of Crystal Edit -
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
 * Revision 1.4  2003/12/16 20:04:38  svenwiegand
 * Implemented Feature 726766: "Option for selecting the language for the GUI"
 *
 * Revision 1.3  2002/04/24 15:37:11  cnorris
 * realtime spell check
 *
 * Revision 1.2  2002/04/10 14:29:07  cnorris
 * Spell check
 *
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:20+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

// INCLUDE-FILE FOR EVERYONE WHO WANTS TO USE CRYSTAL-EDIT-EX

#pragma once

#include "../Source/Speller.h"


#include "../Source/CrysEditEx.h"
#include "../Source/CCrystalEditView.h"
#include "../Source/CrystalParser.h"
#include "../Source/CrystalTextBlock.h"
#include "../Source/CrystalTextBufferEx.h"
#include "../Source/editcmd.h"
#include "../Source/SpellCheckDlg.h"
#include "../Source/BackgroundThread.h"
#include "../Source/AttributeMenu.h"