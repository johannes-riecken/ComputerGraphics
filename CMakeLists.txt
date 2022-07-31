cmake_minimum_required(VERSION 3.10)

# set the project name
project(ComputerGraphics)

find_path(GLEW_INCLUDE_DIR GL/glew.h)

find_library(GLEW_LIBRARY GLEW)
find_library(FREEGLUT3_LIBRARY glut)
find_library(Z_LIBRARY z)
find_library(JPEG_LIBRARY jpeg)
find_library(PNG_LIBRARY png)
find_library(GL_LIBRARY GL)

set(MATH src/vec2.cpp src/vec3.cpp src/vec4.cpp src/mat2.cpp src/mat3.cpp src/mat4.cpp src/quat.cpp src/plane.cpp src/bbox.cpp src/randUtils.cpp src/eig3x3/eig3.cpp src/kdop.cpp)
set(TEXT src/Texture.cpp src/TexImage.cpp src/TgaEncoder.cpp src/TgaDecoder.cpp src/DdsDecoder.cpp src/BmpDecoder.cpp src/JpegDecoder.cpp src/PngDecoder.cpp src/Resources.cpp src/FileSystem.cpp src/ZipFileSystem.cpp src/TexFormat.cpp)
set(WRAP src/Data.cpp src/Program.cpp src/VertexBuffer.cpp src/Framebuffer.cpp src/stringUtils.cpp src/glUtilities.cpp  src/UbAccessor.cpp src/Sampler.cpp src/ScreenQuad.cpp src/Sync.cpp src/Query.cpp src/TransformFeedback4.cpp src/Log.cpp src/Pipeline.cpp src/GlutWindow.cpp  src/BasicMesh.cpp src/Camera.cpp)

# add the executable
add_executable(wood src/wood.cpp ${MATH} ${TEXT} ${WRAP})

include_directories(${GLEW_INCLUDE_DIR})

target_link_libraries(wood PUBLIC ${GLEW_LIBRARY} ${FREEGLUT3_LIBRARY} ${Z_LIBRARY} ${JPEG_LIBRARY} ${PNG_LIBRARY} ${GL_LIBRARY} pthread)

target_compile_features(wood PUBLIC cxx_std_17)
