TeXnicCenter
============

TeXnicCenter is an integrated development environment (IDE) for typesetting
LaTeX documents. The applications runs on Windows and requires a TeX
distribution such as `MiKTeX <http://miktex.org/>`_ or `TeX Live
<http://www.tug.org/texlive/>`_ to be installed.

Prerequisites for compiling TeXnicCenter
----------------------------------------

To compile TeXnicCenter from source, the following tools and libraries are
required:

  * `CMake 2.8.9 <http://www.cmake.org/>`_ or higher
  * Visual C++ 2010 with MFC libraries
  * Microsoft HTML Help Workshop

To checkout the repository, you will also need to install `Mercurial
<http://mercurial.selenic.com/>`_. To generate the setup, `Inno Setup
<http://www.jrsoftware.org/isinfo.php>`_ is required.

Compiling TeXnicCenter
----------------------

1. Create a directory called ``build`` (or alike), ideally somewhere outside the
   TeXnicCenter source directory.
2. Open the command prompt in the previously created directory, and execute ::

     cmake <source-dir>

3. Finally, open the newly generated TeXnicCenter solution and start building.

For further information, visit `TeXnicCenter website <http://www.texniccenter.org/>`_. Don't hesitate to ask us, if a problem occurs.

Contributing
------------

If you want to contribute to the TeXnicCenter project, feel free to contact
`Tino Weinkauf <tino@TeXnicCenter.org>`_, the TeXnicCenter maintainer.
