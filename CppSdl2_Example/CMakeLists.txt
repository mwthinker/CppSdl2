cmake_minimum_required(VERSION 3.14)
project(CppSdl2_Example
	DESCRIPTION
		"A example of using the CppSdl2 library"
	LANGUAGES
		CXX
)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
endif ()

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
	
file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
