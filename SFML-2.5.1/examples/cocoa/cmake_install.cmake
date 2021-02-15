# Install script for directory: /Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xexamplesx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/SFML/examples/cocoa/cocoa.app")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/SFML/examples/cocoa" TYPE DIRECTORY FILES "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/cocoa.app" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/usr/local/share/SFML/examples/cocoa/cocoa.app/Contents/MacOS/cocoa" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/share/SFML/examples/cocoa/cocoa.app/Contents/MacOS/cocoa")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/maxroschupkin/Downloads/SFML-2.5.1/extlibs/libs-osx/Frameworks"
      "$ENV{DESTDIR}/usr/local/share/SFML/examples/cocoa/cocoa.app/Contents/MacOS/cocoa")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xexamplesx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/SFML/examples/cocoa/CocoaAppDelegate.h;/usr/local/share/SFML/examples/cocoa/CocoaAppDelegate.mm;/usr/local/share/SFML/examples/cocoa/NSString+stdstring.h;/usr/local/share/SFML/examples/cocoa/NSString+stdstring.mm;/usr/local/share/SFML/examples/cocoa/main.m")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/SFML/examples/cocoa" TYPE FILE FILES
    "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/CocoaAppDelegate.h"
    "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/CocoaAppDelegate.mm"
    "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/NSString+stdstring.h"
    "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/NSString+stdstring.mm"
    "/Users/maxroschupkin/Downloads/SFML-2.5.1/examples/cocoa/main.m"
    )
endif()

