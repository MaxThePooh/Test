# Install script for directory: /Users/maxroschupkin/Downloads/SFML-2.5.1/examples

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/ftp/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/sockets/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/voip/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/sound/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/sound_capture/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/window/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/joystick/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/opengl/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/shader/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/island/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/cmake_install.cmake")
  include("/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/pong/cmake_install.cmake")

endif()
