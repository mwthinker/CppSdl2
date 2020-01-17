cmake_minimum_required(VERSION 3.1...3.14)
project(CppSdl2_Test
	DESCRIPTION
		"Test the project CppSdl2 using Catch2"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
endif ()

find_package(Catch2 CONFIG)
if (Catch2_FOUND)
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
else (Catch2_FOUND)
	message(WARNING "Catch2 not found, CppSdl2_Test not created")
endif (Catch2_FOUND)
