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
	else ()
		# GCC/Clang
		set(flags " -Wno-unused-parameter")
		set(OS_LIBS "")
	endif ()
endif()

set(CMAKE_CXX_FLAGS_BASE "${INITIAL_FLAGS} ${dialect} ${warnings} ${flags}")

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${DEBUG_FLAGS}")
else ()
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${RELEASE_FLAGS}")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_BASE}")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)

include_directories("${PROJECT_SOURCE_DIR}/include" ${PROJECT_BINARY_DIR})

set(LIBS "-lAshleyCPP-d -ltcod -ltcodxx -lSDL2 -lSDL2main")

set(EXECUTABLE_NAME "rgl")
message("CXX flags: ${CMAKE_CXX_FLAGS}")

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS})

install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)