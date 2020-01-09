include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test_forward_list forward_list.cc)
set_target_properties(test_forward_list PROPERTIES OUTPUT_NAME forward_list)
target_link_libraries(test_forward_list gtest_main)
add_test(NAME TestForwardList COMMAND forward_list)

add_executable(test_vector vector.cc)
set_target_properties(test_vector PROPERTIES OUTPUT_NAME vector)
target_link_libraries(test_vector gtest_main)
add_test(NAME TestVector COMMAND vector)

add_executable(test_list list.cc)
set_target_properties(test_list PROPERTIES OUTPUT_NAME list)
target_link_libraries(test_list gtest_main)
add_test(NAME TestList COMMAND test)

