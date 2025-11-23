# onigpp_test.exe
add_executable(onigpp_test onigpp_test.cpp)
target_compile_definitions(onigpp_test PRIVATE "")
target_include_directories(onigpp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(onigpp_test PRIVATE onigpp)

# onigpp_testw.exe
add_executable(onigpp_testw onigpp_testw.cpp)
target_compile_definitions(onigpp_testw PRIVATE "")
target_include_directories(onigpp_testw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(onigpp_testw PRIVATE onigpp)

# cyclic_replace.exe
add_executable(cyclic_replace cyclic_replace.cpp)
target_compile_definitions(cyclic_replace PRIVATE "")
target_include_directories(cyclic_replace PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cyclic_replace PRIVATE onigpp)
