project (hunspell)

add_definitions (-DHUNSPELL_STATIC)

if (MSVC)
  # inconsistent dll linkage
  add_definitions (-wd4273)
endif (MSVC)

add_subdirectory (src)
