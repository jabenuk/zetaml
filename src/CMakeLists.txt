add_library(${PROJECT_NAME}
	"internal.h"
	"utils.c"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")

option(ZML_USE_FLOATS "Use floats instead of doubles." OFF)
if (ZML_USE_FLOATS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC __floating=float)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC __floating=double)
endif()