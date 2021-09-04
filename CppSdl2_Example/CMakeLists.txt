cmake_minimum_required(VERSION 3.14)
project(CppSdl2_Example
	DESCRIPTION
		"An example of using the CppSdl2 library"
	LANGUAGES
		CXX
)

add_executable(CppSdl2_Example
	src/batchtriangles.cpp
	src/batchtriangles.h
	src/batchtrianglesindexes.cpp
	src/batchtrianglesindexes.h
	src/testimguiwindow.cpp
	src/testimguiwindow.h
	src/types.h
	src/main.cpp
	src/testshader.h
	src/testshader.cpp
	src/graphicwindow.h
	src/testwindow.cpp
	src/testwindow.h
)

if (MSVC)
	target_compile_options(CppSdl2_Example
		PRIVATE
			/W3 /WX /permissive-
	)
else()
	target_compile_options(CppSdl2_Example
		PRIVATE
			-Wall -Wextra -Wnon-virtual-dtor -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter
	)
endif()
	
target_link_libraries(CppSdl2_Example
	PRIVATE
		CppSdl2
)

set_target_properties(CppSdl2_Example
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)
	
file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
