# CMakeList.txt : CMake project for SnakeGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(BOOST_ROOT "C:\\local\\boost_1_83_0")
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)
find_library(GLFW_LIBRARY glfw3 REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}//external//glfw")
find_library(GLEW_LIBRARY glew32s REQUIRED  HINTS "${CMAKE_CURRENT_SOURCE_DIR}//external//glew")

message(${GLEW_LIBRARY})

add_definitions(-DGLEW_STATIC)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include/"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine"
	"${Boost_INCLUDE_DIRS}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# Add source to this project's executable.
add_executable (SnakeGame 
                "SnakeGame.cpp"
				"engine/Renderer.cpp"
				"engine/VertexBuffer.cpp"
				"engine/IndexBuffer.cpp"
				"engine/VertexArray.cpp" 
				"engine/Shader.cpp"
				"engine/Layout.cpp"
				"engine/Elements/Square.cpp"
				"external/stb_image/stb_image.cpp" "engine/Elements/Texture.hpp" "engine/Elements/Texture.cpp" "StbTexture.cpp")

target_link_libraries(SnakeGame
	${OPENGL_LIBRARY}
	${GLFW_LIBRARY}
	${GLEW_LIBRARY}
)


set_target_properties(SnakeGame PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/")
