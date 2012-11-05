# Module for locating Inno Setup.
#
# Read-only variables:
#   INNOSETUP_FOUND
#     Indicates whether Inno Setup has been found.
#
#   INNOSETUP_COMPILER_EXECUTABLE
#     Specifies the path to the compiler executable.
#
#
# Copyright (c) 2012 Sergiu Dotenco
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

INCLUDE (CMakeParseArguments)
INCLUDE (FindPackageHandleStandardArgs)

FIND_PROGRAM (INNOSETUP_COMPILER_EXECUTABLE
  NAME iscc
  HINTS
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Inno Setup 5_is1;InstallLocation]"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Inno Setup 5_is1;InstallLocation]"
  DOC "InnoSetup compiler executable")

IF (INNOSETUP_COMPILER_EXECUTABLE)
  GET_FILENAME_COMPONENT (_OUTPUT
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Inno Setup 5_is1;DisplayVersion]" NAME)
  STRING (REGEX REPLACE "([0-9]+[.][0-9]+([.][0-9]+)?).*$" "\\1"
    _VERSION "${_OUTPUT}")

  STRING (REGEX REPLACE "([0-9]+)[.]([0-9]+)([.]([0-9]+))?" "\\1"
    INNOSETUP_VERSION_MAJOR "${_VERSION}")
  STRING (REGEX REPLACE "([0-9]+)[.]([0-9]+)([.]([0-9]+))?" "\\2"
    INNOSETUP_VERSION_MINOR "${_VERSION}")

  IF (_VERSION MATCHES "[0-9]+[.][0-9]+[.][0-9]+")
    STRING (REGEX REPLACE "([0-9]+)[.]([0-9]+)([.]([0-9]+))?" "\\4"
      INNOSETUP_VERSION_PATCH "${_VERSION}")
    SET (INNOSETUP_VERSION_COUNT 3)
    SET (INNOSETUP_VERSION
      ${INNOSETUP_VERSION_MAJOR}.${INNOSETUP_VERSION_MINOR}.${INNOSETUP_VERSION_PATCH})
  ELSE (_VERSION MATCHES "[0-9]+[.][0-9]+[0-9]+")
    SET (INNOSETUP_VERSION_COUNT 2)
    SET (INNOSETUP_VERSION ${INNOSETUP_VERSION_MAJOR}.${INNOSETUP_VERSION_MINOR})
  ENDIF (_VERSION MATCHES "[0-9]+[.][0-9]+[.][0-9]+")
ENDIF (INNOSETUP_COMPILER_EXECUTABLE)

MARK_AS_ADVANCED (INNOSETUP_COMPILER_EXECUTABLE)

FIND_PACKAGE_HANDLE_STANDARD_ARGS (InnoSetup REQUIRED_VARS
  INNOSETUP_COMPILER_EXECUTABLE VERSION_VAR INNOSETUP_VERSION)
