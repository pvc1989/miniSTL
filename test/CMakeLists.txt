add_executable(forward_list_test forward_list_test.cc)
target_link_libraries(forward_list_test gtest_main)
add_test(NAME ForwardListTest COMMAND forward_list_test)

add_executable(vector_test vector_test.cc)
target_link_libraries(vector_test gtest_main)
add_test(NAME VectorTest COMMAND vector_test)
