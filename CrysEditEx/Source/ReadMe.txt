CrystalEditEx-Classes

An extension of Andrei Stcherbatchenko's CrystalEdit-Classes
============================================================

By Sven Wiegand in 1999
sven.wiegand@gmx.de


License Agreement
-----------------
See source headers (for example CCrystalTextView.h) for information.


Finding my code changes
-----------------------
You will find all my changes by searching for the comment

	//BEGIN SW

Code I have added is surrounded by

	//BEGIN SW
	<new code>
	//END SW

Changes I have made are marked by

	//BEGIN SW
	<new code>
	/*ORIGINAL
	<original code>
	*///END SW


DLL-support
-----------
I have made a MFC-extension-DLL, so that you can link dynamically to the crystal classes. To build 
the project as DLL you have to specify the preprocessor definition 

	CE_DLL_BUILD

To use the crystal classes from a DLL you have to specify the preprocessor definition 

	CE_FROM_DLL 

in the client application.


Preparation of the resources for other languages
------------------------------------------------
The resources are prepared for other languages. To compile the DLL for a specific language specify 
the preprocessor definitions

	AFX_TARG_NEU	(for resources which are not language specific)

and

	AFX_TARG_<LANG>	(for the language you would like to have)

under "project|properties|resources|preprocessor definitions".

To build an english DLL you have to specify 

	AFX_TARG_NEU and AFX_TARG_ENU 

and for the new german version you have to specify

	AFX_TARG_NEU and AFX_TARG_DEU


New Features
------------
* Word Wrapping: standard word wrapping is implemented (breaks line at whitespace before the word 
  that would exceed the screen width) and can be overridden by directly overriding 
  CCrystalTextView::WrapLine() or by deriving own classes from CCrystalParser (recommanded).

* Incremental Search: The commands ID_EDIT_FIND_INCREMENTAL_FORWARD and 
  ID_EDIT_FIND_INCREMENTAL_BACKWARD will switch the editor/text viewer into incremental search 
  mode. From then on, every typed charachter and the backsapce key will modify the word to search 
  (displayed in the message part of the status bar). The search is canceled by Esc- or cursor-keys. 
  Two ID_EDIT_FIND_INCREMENTAL_... commands in series without any character typing will search for 
  the latest searched word.

* "Go to last change command": The ID_EDIT_GOTO_LAST_CHANGE command sets the cursor to the text 
  position where the user made his last changes (inserted or deleted text). This is usefull, if 
  you are going to another text position to look for something and then quickly want to go back 
  to the position where you made your last changes.

* German translation of all resources


New classes
-----------
The following classes whrere added to the project:

* CCrystalParser: 
  Classes derived from CCrystalParser contain all language specific stuff (at this time only 
  syntax highlighting and word wrapping). So you can switch the language on the fly by calling 
  CCrystalTextView::SetParser().

* CCrystalTextBlock: 
  Objects of this class are passed to the CCrystalParser::ParseLine()-method instead of the 
  simple array used by the original CCrystalTextView::ParseLine()-method. The advantage of this 
  class is, that you can insert color changes in free order (you do not have to sort them by 
  character position).

* CCrystalTextBufferEx: You can use this class instead of CCrystalTextBuffer when your application 
  uses the MFC document-view-architecture. You do not have to derive your own class from 
  CCrystalTextBuffer then, because objects of this class inform the attached document about 
  changes of the buffer contents.

For detailed information about using those classes see the header files.


Bug fixes
---------
* When the cursor position is changed by clicking with the mouse the variable 
  CCrystalTextView::m_nIdealCharPos is now updated to the new position.

* The backward search now does what it should do ;-)


Other changes
-------------
* In my application I am using the BCG-classes of Stas Levin (also available add code guru -- really 
  great) which provide user customizable toolbars, menubars and accelerator tables. Therefore I 
  have removed all accelerators, which are not elementary for editing the text, from the accelerator 
  table of the DLL.


Berlin, 15. September 1999

Sven Wiegand (sven.wiegand@gmx.de)