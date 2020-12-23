add_executable(atomic_unittest Atomic_unittest.cc)
add_test(NAME atomic_unittest COMMAND atomic_unittest)

add_executable(thread_test Thread_test.cc)
target_link_libraries(thread_test flygon_base)

add_executable(mutex_test Mutex_test.cc)
target_link_libraries(mutex_test flygon_base)

add_executable(singleton_test Singleton_test.cc)
target_link_libraries(singleton_test flygon_base)

add_executable(exception_test Exception_test.cc)
target_link_libraries(exception_test flygon_base)