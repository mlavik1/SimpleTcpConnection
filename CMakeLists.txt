cmake_minimum_required(VERSION 3.3)

project(SimpleTcpConnection)

set(CMAKE_BUILD_TYPE Debug)

set(BUILD_ROOT "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

set(SourceDir ${PROJECT_SOURCE_DIR}/src)

file(GLOB SOURCES "src/*")

include_directories(include)


add_executable(SimpleTcpConnection ${SOURCES})


SET(LIB_DIR "${CMAKE_SOURCE_DIR}/lib/x86")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	TARGET_LINK_LIBRARIES(SimpleTcpConnection ${LIB_DIR}/SDL2.lib)
	TARGET_LINK_LIBRARIES(SimpleTcpConnection ${LIB_DIR}/SDL2main.lib)
	TARGET_LINK_LIBRARIES(SimpleTcpConnection ${LIB_DIR}/SDL2_net.lib)

else("")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	if (${CMAKE_GENERATOR} MATCHES "Visual Studio 14")
		 set (DLL_TO_DBG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
		 set (DLL_TO_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	else()
		 set (DLL_TO_DBG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		 set (DLL_TO_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif()

	file(GLOB_RECURSE nativeLibs ${LIB_DIR}/*.dll)
	foreach(AddonFile ${nativeLibs})
		get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
		string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/${LIB_DIR}"
				"" OUT_DIR ${OUT_DIR})
	
		file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${OUT_DIR})
		add_custom_command(TARGET GET SimpleTcpConnection PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E
				copy ${AddonFile} ${DLL_TO_DBG}/)
	endforeach(AddonFile)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

