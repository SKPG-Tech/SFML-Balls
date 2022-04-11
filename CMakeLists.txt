cmake_minimum_required (VERSION 3.10)

project ("Balls")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(MSVC)
	set_property(GLOBAL PROPERTY VS_GLOBAL_EnableCppCoreCheck true)
	set_property(GLOBAL PROPERTY VS_GLOBAL_CodeAnalysisRuleSet CppCoreCheckRules.ruleset)
	set_property(GLOBAL PROPERTY VS_GLOBAL_RunCodeAnalysis true)
	
	set(CompilerFlags
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_RELEASE
	)
	foreach(CompilerFlag ${CompilerFlags})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	
	string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

add_subdirectory ("main")
