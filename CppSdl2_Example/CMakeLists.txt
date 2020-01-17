cmake_minimum_required(VERSION 3.1...3.14)
project(CppSdl2_Example
	DESCRIPTION
		"A example of using the CppSdl2 library"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
endif ()

message(STATUS "CppSdl2_Example is available to add: -DCppSdl2_Example=1")
option(CppSdl2_Example "Add CppSdl2Test project." OFF)

if (CppSdl2_Example)
	add_executable(CppSdl2_Example
		src/batchtriangles.cpp
		src/batchtriangles.h
		src/batchtrianglesindexes.cpp
		src/batchtrianglesindexes.h
		src/logger.h
		src/testimguiwindow.cpp
		src/testimguiwindow.h
		src/types.h
		src/main.cpp
		src/testshader.h
		src/testshader.cpp
		src/testwindow2.h
		src/testwindow.cpp
		src/testwindow.h
	)
	
	target_link_libraries(CppSdl2_Example
		PRIVATE
			CppSdl2
	)
	
	file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
endif (CppSdl2_Example)
