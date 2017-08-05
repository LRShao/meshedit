# Install script for directory: /home/lowell/asst2_meshedit/CMU462/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/lowell/asst2_meshedit/build/CMU462/src/libCMU462.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CMU462" TYPE FILE FILES
    "/home/lowell/asst2_meshedit/CMU462/src/CMU462.h"
    "/home/lowell/asst2_meshedit/CMU462/src/vector2D.h"
    "/home/lowell/asst2_meshedit/CMU462/src/vector3D.h"
    "/home/lowell/asst2_meshedit/CMU462/src/vector4D.h"
    "/home/lowell/asst2_meshedit/CMU462/src/matrix3x3.h"
    "/home/lowell/asst2_meshedit/CMU462/src/matrix4x4.h"
    "/home/lowell/asst2_meshedit/CMU462/src/quaternion.h"
    "/home/lowell/asst2_meshedit/CMU462/src/complex.h"
    "/home/lowell/asst2_meshedit/CMU462/src/color.h"
    "/home/lowell/asst2_meshedit/CMU462/src/osdtext.h"
    "/home/lowell/asst2_meshedit/CMU462/src/viewer.h"
    "/home/lowell/asst2_meshedit/CMU462/src/base64.h"
    "/home/lowell/asst2_meshedit/CMU462/src/tinyxml2.h"
    "/home/lowell/asst2_meshedit/CMU462/src/renderer.h"
    )
endif()

