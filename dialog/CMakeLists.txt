enable_language(RC)

# dialog.exe
add_executable(dialog WIN32 dialog.cpp dialog_res.rc)
target_compile_definitions(dialog PRIVATE UNICODE _UNICODE)
target_link_libraries(dialog comctl32 onigpp)
if(MSVC)
target_link_options(dialog PRIVATE "/MANIFEST:NO")
endif()
