enable_language(RC)

# u8dialog.exe
add_executable(u8dialog WIN32 dialog.cpp dialog_res.rc)
target_link_libraries(u8dialog comctl32 onigpp)
if(MSVC)
target_link_options(u8dialog PRIVATE "/MANIFEST:NO")
endif()
