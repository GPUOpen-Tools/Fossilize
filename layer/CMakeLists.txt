if (WIN32)
	set(export-file exports.def)
else()
	set(export-file )
endif()

add_library(VK_LAYER_RGA_pipeline_extraction SHARED
	device.cpp
	device.hpp
	instance.cpp
	instance.hpp
	dispatch.cpp
	dispatch_helper.hpp
	dispatch_helper.cpp
	${export-file})
target_include_directories(VK_LAYER_RGA_pipeline_extraction PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(VK_LAYER_RGA_pipeline_extraction PUBLIC ${FOSSILIZE_CXX_FLAGS})
set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES LINK_FLAGS "${FOSSILIZE_LINK_FLAGS}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/layer)

if (WIN32)
	add_custom_command(TARGET VK_LAYER_RGA_pipeline_extraction POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/VK_LAYER_RGA_pipeline_extraction_win32.json ${CMAKE_BINARY_DIR}/layer/VK_LAYER_RGA_pipeline_extraction.json)
	set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES PREFIX "" LIBRARY_OUTPUT_NAME VK_LAYER_RGA_pipeline_extraction)
	if (CMAKE_COMPILER_IS_GNUCXX)
		if (${CMAKE_BUILD_TYPE} MATCHES "Release")
			set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES LINK_FLAGS "${FOSSILIZE_LINK_FLAGS} -Wl,--no-undefined -static -s")
		else()
			set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES LINK_FLAGS "${FOSSILIZE_LINK_FLAGS} -Wl,--no-undefined -static")
		endif()
	endif()
else()
	if (NOT ANDROID)
		add_custom_command(TARGET VK_LAYER_RGA_pipeline_extraction POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/VK_LAYER_RGA_pipeline_extraction.json ${CMAKE_BINARY_DIR}/layer/VK_LAYER_RGA_pipeline_extraction.json)
	endif()
	if (${CMAKE_BUILD_TYPE} MATCHES "Release")
		set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES LINK_FLAGS "${FOSSILIZE_LINK_FLAGS} -Wl,--no-undefined -s")
	else()
		set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES LINK_FLAGS "${FOSSILIZE_LINK_FLAGS} -Wl,--no-undefined")
	endif()
endif()

set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/layer")
set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/layer")
set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/layer")
set_target_properties(VK_LAYER_RGA_pipeline_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/layer")
target_link_libraries(VK_LAYER_RGA_pipeline_extraction fossilize)

if (ANDROID)
	target_link_libraries(VK_LAYER_RGA_pipeline_extraction log)
endif()
