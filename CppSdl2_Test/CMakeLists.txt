project(CppSdl2_Test
	DESCRIPTION
		"Test the project CppSdl2 using Catch2"
	LANGUAGES
		CXX
)

find_package(GTest CONFIG)

if (GTest_FOUND)
	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
	endif ()

	add_executable(CppSdl2_Test
		src/tests.cpp
	)
	
	target_link_libraries(CppSdl2_Test
		PRIVATE
			CppSdl2
			GTest::gtest GTest::gtest_main
	)

	set_target_properties(CppSdl2_Test
		PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS NO
	)

	if (MSVC)
		target_compile_options(CppSdl2
			PRIVATE
				"/permissive-"
		)
	endif ()

	include(GoogleTest)
	gtest_discover_tests(CppSdl2_Test)

else ()
	message(WARNING "Catch2 not found, CppSdl2_Test not created")
endif ()
