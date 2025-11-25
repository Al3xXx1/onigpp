enable_language(RC)

# wdialog.exe
add_executable(wdialog WIN32 dialog.cpp dialog_res.rc)
target_compile_definitions(wdialog PRIVATE UNICODE _UNICODE)
target_link_libraries(wdialog comctl32 onigpp)
if(MSVC)
target_link_options(wdialog PRIVATE "/MANIFEST:NO")
endif()
