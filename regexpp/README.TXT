Copyright (c) 1998-9
Dr John Maddock

Permission to use, copy, modify, distribute and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation.  Dr John Maddock makes no representations
about the suitability of this software for any purpose.  
It is provided "as is" without express or implied warranty.

************************************************************************

Full instructions for use of this library are contained inside
regex.htm.

When you extract the library from its zip file, you must preserve 
its internal directory structure (for example by using the -d 
option when extracting). If you didn't do that when extracting, 
then you'd better stop reading this, delete the files you just 
extracted, and try again!
Currently the library will automatically detect and configure 
itself for Borland, Microsoft and EGCS compilers only. The library 
will also detect the HP, SGI, Rogue Wave, or Microsoft STL implementations. 
Alternatively you can define JM_NO_STL and use the library with no 
underlying STL. If the STL type is detected, then the library will 
attempt to extract suitable compiler configuration options from the 
STL used. Otherwise the library will assume that the compiler is 
fully compliant with the latest draft standard, unless various 
options are defined to depreciate features not implemented by 
your compiler, these are documented in <jm_cfg.h>, do not edit 
this file directly, if you want to add permanent configuration 
options add them to <jm_opt.h> which is provided for this purpose - 
this will allow you to keep your configuration options between 
library versions by retaining <jm_opt.h>. I realise that the 
library needs a configure script for Unix builds, no doubt at 
some point I'll get around to writing one.

The library will encase all code inside namespace __JM unless 
the macro JM_NO_NAMESPACES is defined, __JM is a macro that 
defaults to "jm", but can be defined on the command line to 
be whatever you want. You can also use the macro __JM_STD to 
refer to the namespace enclosing your standard template library.
Unlike some other template libraries, this library consists of a 
mixture of template code (in the headers) and static code and data 
(in cpp files). Consequently it is necessary to build the library's 
support code into a library or archive file before you can use it, 
instructions for specific platforms are as follows:

Borland C++:
Open up a console window and change to the <regex++>\lib directory.
Select the appropriate batch file (for example bc5.bat for C++ 5, 
bcb1.bat for Builder1, bcb3.bat for Builder 3 etc).
Invoke the batch file passing the full path to your version of make 
on the command line, for example:

Bc5 c:\bc5\bin\make

Note that the full path to make is required if you have more than one 
version of make on your system (as happens if you have more than one 
version of Borland's tools installed).
The build process will build a variety of .lib and .dll files (the 
exact number depends upon the version of Borland's tools you are using), 
and you should see a message at the end indicating success. The .lib 
files will be copied to <BCROOT>/lib and the dll's to <BCROOT>/bin, 
where <BCROOT> corresponds to the install path of your Borland C++ tools.
Finally when you use regex++ it is only necessary for you to add 
<regex++>\include to your list of include directories for that project. 
It is not necessary for you to manually add a .lib file to the project; 
the headers will automatically select the correct .lib file for your 
build mode and tell the linker to include it. There is one caveat however: 
the library can not tell the difference between Borland C++ 5.02, and 
Borland C++ Builder 1, consequently if you use the library with C++ Builder 1 
then you must define the pre-processor symbol JM_USE_VCL in order to ensure 
that the correct link libraries are selected.
 
Microsoft Visual C++ 5/6
You need either version 5 (service pack 3) or version 6 of MSVC to build 
this library.
Open up a command prompt, which has the necessary MSVC environment 
variables defined (for example by using the batch file Vcvars32.bat 
installed by the Visual Studio installation), and change to the 
<regex++>\lib directory.
Execute the vc6.bat file - at the end you should have six .lib files and 
two dll files, copies of these will have been placed in your <msvc>\lib 
and <msvc>\bin directories.
Finally when you use regex++ it is only necessary for you to add 
<regex++>\include to your list of include directories for that project. 
It is not necessary for you to manually add a .lib file to the project; 
the headers will automatically select the correct .lib file for your 
build mode and tell the linker to include it.
 
EGCS
There is currently no configure script, however there is a makefile for 
win32 versions of the egcs compiler. With minor modification this should 
be suitable for egcs on other platforms. From the command prompt change 
to the <regex++>/lib directory and type:
make -f egcs.mak
At the end of the build process you should have a libregex++.a archive file. 
When you build projects that use regex++, you will need to add <regex++>/include 
to your list of include paths and add regex++ to your list of library files.
 
Other compilers:
You need to build all the .cpp files in the <regex++>/src directory into a 
library or archive file. Note that the code is designed to work with compilers 
that support automatic merging of duplicate template instances - lack of support 
for this is still a big handicap to making any template code truly portable. 
If your compiler does not support this then you may be better off adding the .cpp 
files directly to your project on a bespoke basis.





