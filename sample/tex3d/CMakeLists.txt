project(tex3d)

cmake_minimum_required(VERSION 2.8)

if(WIN32)
  list(APPEND CMAKE_LIBRARY_PATH 
    "../../3rdparty/DevIL-Windows-SDK-1.8.0/DevIL Windows SDK/lib/x64/Release"
    "../../3rdparty/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64"
    "../../3rdparty/glfw-3.2.1/build/src/Release"
    )
  list(APPEND CMAKE_INCLUDE_PATH 
    "../../3rdparty/DevIL-Windows-SDK-1.8.0/DevIL Windows SDK/include"
    "../../3rdparty/glew-2.1.0-win32/glew-2.1.0/include"
    "../../3rdparty/glfw-3.2.1/include"
    )
  set(Boost_USE_STATIC_LIBS ON)
  set(BOOST_ROOT "../../3rdparty/boost_1_66_0")
  set(BOOST_LIBRARYDIR "../../3rdparty/boost_1_66_0/stage/lib")

#  get_filename_component(exeout "." ABSOLUTE)
#  set(EXECUTABLE_OUTPUT_PATH ${exeout})

endif(WIN32)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
if(NOT WIN32)
  find_package(PkgConfig REQUIRED)
endif(NOT WIN32)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS filesystem system regex)

# for FindDevIL.cmake's issue (includepath includes IL)
find_path(IL_INCLUDE_DIR IL/il.h
  PATH_SUFFIXES include
  )
find_package(DevIL REQUIRED)

if(WIN32)
  set(GLFW_INCLUDE_DIRS "../../3rdparty/glfw-3.2.1/include")
  get_filename_component(result "../../3rdparty/glfw-3.2.1/build/src/Release/glfw3dll.lib" ABSOLUTE)
  set(GLFW_LIBRARIES ${result})
  include_directories("../../3rdparty/glm-0.9.8.5")
  include_directories("../../3rdparty/glew-2.1.0-win32/glew-2.1.0/include")
else(WIN32)
  pkg_search_module(GLFW REQUIRED glfw3)
endif(WIN32)

if(NOT WIN32)
  list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(NOT WIN32)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${IL_INCLUDE_DIR})
include_directories(../../src)


set(target "tex3d")

add_executable(${target}
  tex3d.cpp
  ../../src/shader.cpp
  ../../src/geometry.cpp
  ../../src/VAO.cpp
  ../../src/marchingcubes.cpp
  ../../src/error.cpp
)

target_link_libraries(${target}
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${Boost_LIBRARIES}
  ${IL_LIBRARIES}
  ${ILU_LIBRARIES}
)