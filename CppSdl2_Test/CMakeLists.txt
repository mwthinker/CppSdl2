cmake_minimum_required(VERSION 3.14)
project(CppSdl2_Test
	DESCRIPTION
		"Test the project CppSdl2 using Catch2"
	LANGUAGES
		CXX
)

find_package(Catch2 CONFIG)

if (Catch2_FOUND)
	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
	endif ()

	add_executable(CppSdl2_Test
		src/main.cpp
		src/speedtest.cpp
		src/tests.cpp
	)
	
	add_compile_definitions(CATCH_CONFIG_ENABLE_BENCHMARKING)

	target_link_libraries(CppSdl2_Test
		PRIVATE
			CppSdl2
			Catch2::Catch2
	)

	set_target_properties(CppSdl2_Example
		PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS NO
	)

	if (MSVC)
		target_compile_options(CppSdl2
			PRIVATE
				"/permissive-"
		)
	endif (MSVC)

else (Catch2_FOUND)
	message(WARNING "Catch2 not found, CppSdl2_Test not created")
endif (Catch2_FOUND)
