find_package(SDL2 REQUIRED)
find_package(AshleyCPP REQUIRED)
find_package(GLM REQUIRED)
add_definitions(-DGLM_FORCE_RADIANS)

# TODO: add cmake for finding tcod

if (NOT TCOD_INCLUDE_DIR)
	if ( APPLE )
	   set(TCOD_INCLUDE_DIR "/usr/local/include/TCOD")
	elseif ( MSVC )
		message(ERROR "MSVC not supported for tcod autodetection")
	else ()
		set(TCOD_INCLUDE_DIR "/usr/local/include/libtcod")
	endif ()
endif ()

if (NOT TCOD_LIBRARIES)
	if ( MSVC )
		message(ERROR "MSVC not supported for tcod autodetection")
	else ()
		set(TCOD_LIBRARIES "-ltcod -ltcodxx")
	endif ()
endif ()

if( MSVC )
	set(dialect "")
	set(warnings "")
	set(flags "/EHsc /MP")

	set(DEBUG_FLAGS "/Od")
	set(RELEASE_FLAGS "/Ox")
else ()
	set(dialect "-std=c++1y")
	set(warnings "-Wall -Wextra")
	
	set(DEBUG_FLAGS "-O0 -g3 -DDEBUG")
	set(RELEASE_FLAGS "-O3 -g0 -march=native")
	
	if ( DEFINED WIN32 )
		# MinGW-w64
		set(flags "-mwindows -mconsole")
		set(OS_LIBS Version Imm32 winmm mingw32 gdi32)
	elseif ( APPLE )
		set(flags " -Wno-unused-parameter")
		set(OS_LIBS "-framework CoreFoundation -framework IOKit -framework Carbon -framework Cocoa")
		include_directories("${PROJECT_SOURCE_DIR}/osx")
		file(GLOB_RECURSE OS_SOURCES ${PROJECT_SOURCE_DIR}/osx/*.m)
		file(GLOB_RECURSE OS_HEADERS ${PROJECT_SOURCE_DIR}/osx/*.h)
	else ()
		# GCC/Clang
		set(flags " -Wno-unused-parameter")
		set(OS_LIBS "")
	endif ()
endif()

set(LIBS ${SDL2_LIBRARY} ${TCOD_LIBRARIES} ${OS_LIBS})

set(CMAKE_CXX_FLAGS_BASE "${INITIAL_FLAGS} ${dialect} ${warnings} ${flags}")

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${DEBUG_FLAGS}")
	set(LIBS "${LIBS} ${AshleyCPP_DEBUG_LIBRARY}")
else ()
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${RELEASE_FLAGS}")
	set(LIBS "${LIBS} ${AshleyCPP_LIBRARY}")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_BASE}")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}
		${SDL2_INCLUDE_DIR} ${AshleyCPP_INCLUDE_DIR} ${TCOD_INCLUDE_DIR}
		${GLM_INCLUDE_DIRS})

set(EXECUTABLE_NAME "rgl")
message("CXX flags: ${CMAKE_CXX_FLAGS}")

file(COPY "${PROJECT_SOURCE_DIR}/terminal.png" DESTINATION ${PROJECT_BINARY_DIR})

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS} ${OS_SOURCES} ${OS_HEADERS})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS})

install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
