﻿cmake_minimum_required (VERSION 3.10)

add_executable (main "main.cpp" )

target_link_libraries(main PRIVATE ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib )
target_link_libraries(main PRIVATE ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib )
target_link_libraries(main PRIVATE ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib )

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_target_properties(
	main
	PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
			   LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE"
)

if (MSVC)
	target_compile_options(main PUBLIC /W4 /permissive)
else()
	target_compile_options(main PUBLIC -Wall -Wextra -pedantic)
endif()