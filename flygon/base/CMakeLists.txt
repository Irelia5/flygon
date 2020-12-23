add_library(flygon_base
	Thread.cc	
	CurrentThread.cc
	CountDownLatch.cc
	Exception.cc
)

target_link_libraries(flygon_base
	pthread
)

if(FLYGON_BUILD_EXAMPLES)
	add_subdirectory(tests)
endif()