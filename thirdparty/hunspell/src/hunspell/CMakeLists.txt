project (hunspell)

set (PROJECT_CPP
  affentry.cxx
  affixmgr.cxx
  csutil.cxx
  dictmgr.cxx
  filemgr.cxx
  hashmgr.cxx
  hunspell.cxx
  hunzip.cxx
  phonet.cxx
  replist.cxx
  suggestmgr.cxx
  utf_info.cxx
)

set (PROJECT_H
  affentry.hxx
  affixmgr.hxx
  atypes.hxx
  baseaffix.hxx
  csutil.hxx
  dictmgr.hxx
  filemgr.hxx
  hashmgr.hxx
  htypes.hxx
  hunspell.hxx
  hunzip.hxx
  langnum.hxx
  phonet.hxx
  replist.hxx
  suggestmgr.hxx
  w_char.hxx
)

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_H})
