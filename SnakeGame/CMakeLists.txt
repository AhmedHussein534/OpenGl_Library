# CMakeList.txt : CMake project for SnakeGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.1)

set(BOOST_ROOT "C:\\local\\boost_1_83_0")
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)


add_definitions(-DGLEW_STATIC)
add_definitions(-DGLFW_BUILD_WIN32)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include/"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine"
	"${Boost_INCLUDE_DIRS}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

link_directories("${CMAKE_BINARY_DIR}/SnakeGame/external/glfw/src/")
message("${CMAKE_BINARY_DIR}/SnakeGame/external/glfw/src/")
# Add source to this project's executable.
add_executable (SnakeGame
                "SnakeGame.cpp"
				"engine/Renderer.cpp"
				"engine/VertexBuffer.cpp"
				"engine/IndexBuffer.cpp"
				"engine/VertexArray.cpp"
				"engine/Shader.cpp"
				"engine/Layout.cpp"
				"engine/Renderer2D.cpp"
				# "engine/Elements/Square.cpp"
				"engine/Elements/Rectangle.cpp"
				# "engine/Elements/Texture.cpp"
				# "engine/Elements/Texture3D.cpp"
				# "engine/Elements/Cube.cpp"
				"engine/Cameras/OrthographicCamera.cpp"
				"engine/Cameras/PerspectiveCamera.cpp"
				"external/stb_image/stb_image.cpp"
				"external/glew/src/glew.c"
				"window/Windows/WindowsWindow.cpp"
				)

target_link_libraries(SnakeGame
	"${OPENGL_LIBRARY}"
	"glfw3"
)

add_custom_command(TARGET SnakeGame PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/SnakeGame/res/ $<TARGET_FILE_DIR:SnakeGame>/res)
