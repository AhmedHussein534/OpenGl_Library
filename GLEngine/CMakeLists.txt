# CMakeList.txt : CMake project for SnakeGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.5)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLFW_BUILD_WIN32)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/")

# Add source to this project's executable.
add_library (GLEngine STATIC
            "src/Application.cpp"
            "src/Renderer.cpp"
            "src/VertexBuffer.cpp"
            "src/IndexBuffer.cpp"
            "src/VertexArray.cpp"
            "src/Shader.cpp"
            "src/Layout.cpp"
            "src/Renderer2D.cpp"
            "src/Elements/Rectangle.cpp"
            "src/Elements/Texture.cpp"
            "src/Elements/TextureAsset.cpp"
            "src/Cameras/OrthographicCamera.cpp"
            "src/Cameras/PerspectiveCamera.cpp"
            "external/stb_image/stb_image.cpp"
            "external/glew/src/glew.c"
            "window/Windows/WindowsWindow.cpp"
)

target_include_directories(GLEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(GLEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_directories(GLEngine PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/external/glfw/src/"
)

target_link_libraries(GLEngine PRIVATE
	"${OPENGL_LIBRARY}"
	"glfw"
)
