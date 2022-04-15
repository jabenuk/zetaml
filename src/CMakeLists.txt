add_library(${PROJECT_NAME}
	"utils.c"
	"vector.c"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")

if (X86_BUILD)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ZML_32_BIT)
endif()

option(ZML_USE_FLOATS "Use floats instead of doubles." OFF)
if (ZML_USE_FLOATS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC __floating=float)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ZML_USING_FLOATS)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC __floating=double)
endif()

# link to C math library
target_link_libraries(${PROJECT_NAME} m)