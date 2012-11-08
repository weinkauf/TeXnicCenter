# Simplified bundle setup for CMake
#
# SETUP_BUNDLE (TARGET target DESTINATION destination
#   [COMPONENT component] [NAME name] [OUTPUT output] [SUFFIX suffix]
#   [RUNTIME_LIBRARY_DIRS dir1 [dir2 ...]] [LINK_LIBRARIES lib1 [lib2 ...]]
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
# FITNESS FOR A PARTOPENCVLAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

INCLUDE (BundleUtilities)
INCLUDE (CMakeParseArguments)

FUNCTION (SETUP_BUNDLE ARGS)
  SET (_SINGLE TARGET EXECUTABLE DESTINATION COMPONENT OUTPUT SUFFIX NAME)
  SET (_MULTI RUNTIME_LIBRARY_DIRS LINK_LIBRARIES)
  SET (_OPTIONS)

  CMAKE_PARSE_ARGUMENTS (_ARGS "${_OPTIONS}" "${_SINGLE}" "${_MULTI}" ${ARGS}
    ${ARGN})

  # Error checking and handling of default parameters

  IF (DEFINED _ARGS_TARGET)
    # TARGET signature
    IF (NOT DEFINED _ARGS_TARGET)
      MESSAGE (FATAL_ERROR "Missing setup bundle TARGET name")
    ENDIF (NOT DEFINED _ARGS_TARGET)

    IF (NOT TARGET ${_ARGS_TARGET})
      MESSAGE (FATAL_ERROR "Cannot setup the bundle for a non-existent target")
    ENDIF (NOT TARGET ${_ARGS_TARGET})

    IF (NOT DEFINED _ARGS_OUTPUT)
      GET_TARGET_PROPERTY (_ARGS_OUTPUT ${_ARGS_TARGET} OUTPUT_NAME)

      IF (NOT _ARGS_OUTPUT)
        SET (_ARGS_OUTPUT "${_ARGS_TARGET}")
      ENDIF (NOT _ARGS_OUTPUT)
    ENDIF (NOT DEFINED _ARGS_OUTPUT)
  ELSEIF (DEFINED _ARGS_EXECUTABLE)
    # EXECUTABLE signature
    SET (_ARGS_OUTPUT "${_ARGS_EXECUTABLE}")
  ELSE (DEFINED _ARGS_TARGET)
    MESSAGE (FATAL_ERROR "SETUP_BUNDLE requires either the TARGET or the "
      "EXECUTABLE parameter to be specified")
  ENDIF (DEFINED _ARGS_TARGET)

  IF (NOT DEFINED _ARGS_DESTINATION)
    SET (_ARGS_DESTINATION bin)
  ENDIF (NOT DEFINED _ARGS_DESTINATION)

  IF (NOT DEFINED _ARGS_NAME)
    SET (_ARGS_NAME .)
  ENDIF (NOT DEFINED _ARGS_NAME)

  IF (NOT DEFINED _ARGS_SUFFIX)
    IF (NOT _ARGS_EXECUTABLE)
      GET_TARGET_PROPERTY (_ARGS_SUFFIX ${_ARGS_TARGET} SUFFIX)
    ELSE (NOT _ARGS_EXECUTABLE)
      UNSET (_ARGS_SUFFIX)
    ENDIF (NOT _ARGS_EXECUTABLE)

    IF (NOT _ARGS_SUFFIX)
      IF (WIN32)
          SET (_ARGS_SUFFIX .exe)
      ELSEIF (APPLE)
          SET (_ARGS_SUFFIX .app)
      ENDIF (WIN32)
    ENDIF (NOT _ARGS_SUFFIX)
  ENDIF (NOT DEFINED _ARGS_SUFFIX)

  # Parameter construction

  SET (_INSTALL_ARGS
    TARGETS ${_ARGS_TARGET} BUNDLE DESTINATION "${_ARGS_NAME}")

  UNSET (_COMPONENT)

  IF (NOT "${_ARGS_COMPONENT}" STREQUAL "")
    SET (_COMPONENT COMPONENT "${_ARGS_COMPONENT}")
    LIST (APPEND _INSTALL_ARGS ${_COMPONENT})
  ENDIF (NOT "${_ARGS_COMPONENT}" STREQUAL "")

  LIST (APPEND _INSTALL_ARGS RUNTIME DESTINATION "${_ARGS_DESTINATION}")

  IF (_COMPONENT)
    LIST (APPEND _INSTALL_ARGS ${_COMPONENT})
  ENDIF (_COMPONENT)

  INSTALL (${_INSTALL_ARGS})

  IF (NOT UNIX OR APPLE)
    INSTALL (CODE
        "
        include (BundleUtilities)

        MACRO (GET_APPS APPS)
          IF (WIN32)
            SET (\${APPS}
              \"\${CMAKE_INSTALL_PREFIX}/${_ARGS_DESTINATION}/${_ARGS_OUTPUT}${_ARGS_SUFFIX}\")
          ELSEIF (APPLE)
            SET (\${APPS}
              \"\${CMAKE_INSTALL_PREFIX}/${_ARGS_NAME}/${_ARGS_OUTPUT}${_ARGS_SUFFIX}\")

            IF (NOT EXISTS \"\${\${APPS}}\")
              SET (\${APPS}
                \"\${CMAKE_INSTALL_PREFIX}/${_ARGS_NAME}/${_ARGS_OUTPUT}\")
            ENDIF (NOT EXISTS \"\${\${APPS}}\")
          ELSE (WIN32)
            SET (\${APPS}
              \"\${CMAKE_INSTALL_PREFIX}/${_ARGS_DESTINATION}/${_ARGS_OUTPUT}\")
          ENDIF (WIN32)
        ENDMACRO (GET_APPS)

        get_apps (APPS)

        fixup_bundle (\"\${APPS}\" \"${_ARGS_LINK_LIBRARIES}\"
          \"${_ARGS_RUNTIME_LIBRARY_DIRS}\")
        ")
  ENDIF (NOT UNIX OR APPLE)
ENDFUNCTION (SETUP_BUNDLE)
