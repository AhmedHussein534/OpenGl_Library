# CMakeList.txt : CMake project for SnakeGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.1)
set(CMAKE_BUILD_TYPE Release)

add_executable (SnakeGame
                "SnakeGame.cpp"
				"main.cpp"
)

target_include_directories(SnakeGame PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(SnakeGame
	GLEngine
)

add_custom_command(TARGET SnakeGame PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/SnakeGame/res/ $<TARGET_FILE_DIR:SnakeGame>/res)
