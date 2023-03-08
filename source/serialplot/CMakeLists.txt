#
# Copyright © 2021 Hasan Yavuz Özderya
#
# This file is part of serialplot.
#
# serialplot is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# serialplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
#

project(serialplot)

set(PROGRAM_NAME ${CMAKE_PROJECT_NAME} CACHE STRING "Output program name")
set(PROGRAM_DISPLAY_NAME "SerialPlot" CACHE STRING "Display name (menus etc) of the program")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# add local path for cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Find the libraries
find_package(Qt5
  REQUIRED COMPONENTS 
      Widgets
      SerialPort
      Network
      Svg
      PrintSupport
)

# If set, cmake will download Qwt over SVN, build and use it as a static library.
set(BUILD_QWT true CACHE BOOL "Download and build Qwt automatically.")
if (BUILD_QWT)
  include(BuildQwt)
else (BUILD_QWT)
    find_package(Qwt 6.1 REQUIRED)
endif (BUILD_QWT)

# includes
include_directories("./src"
  ${QWT_INCLUDE_DIR}
  )

# wrap UI and resource files
qt5_wrap_ui(UI_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/portcontrol.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/about_dialog.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/snapshotview.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/commandpanel.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/commandwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/dataformatpanel.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/plotcontrolpanel.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/recordpanel.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/numberformatbox.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/endiannessbox.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/binarystreamreadersettings.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/asciireadersettings.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/framedreadersettings.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/demoreadersettings.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/updatecheckdialog.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/datatextview.ui
)

if (WIN32)
  qt5_add_resources(RES_FILES ${CMAKE_CURRENT_LIST_DIR}/misc/icons.qrc ${CMAKE_CURRENT_LIST_DIR}/misc/winicons.qrc)
else (WIN32)
  qt5_add_resources(RES_FILES ${CMAKE_CURRENT_LIST_DIR}/misc/icons.qrc)
endif (WIN32)

add_executable(${PROGRAM_NAME} WIN32
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/portcontrol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/plot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/zoomer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scrollzoomer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scrollbar.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/hidabletabwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scalepicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scalezoomer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/portlist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/snapshot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/snapshotview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/snapshotmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/plotsnapshotoverlay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/commandpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/commandwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/commandedit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/dataformatpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/plotcontrolpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/recordpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/datarecorder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tooltipfilter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sneakylineedit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/stream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/streamchannel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/channelinfomodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ringbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ringbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/indexbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/linindexbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/readonlybuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/fftplotmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/fftstream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/fftringbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/framebufferseries.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/numberformatbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/endiannessbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/abstractreader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/binarystreamreader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/binarystreamreadersettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/asciireader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/asciireadersettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/demoreader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/demoreadersettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/framedreader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/framedreadersettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/plotmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/plotmenu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/barplot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/barchart.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/barscaledraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/numberformat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/updatechecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/versionnumber.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/updatecheckdialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/samplepack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/source.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sink.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/samplecounter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ledwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/datatextview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/bpslabel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/qcustomplot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/misc/windows_icon.rc
    ${UI_FILES}
    ${RES_FILES}
)

# include FFTW shared library
target_include_directories(${PROGRAM_NAME}
    PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}/fftw
      ${CMAKE_CURRENT_LIST_DIR}/src
)

set(fftw3_shared "${CMAKE_CURRENT_LIST_DIR}/fftw/libfftw3-3.dll")
set(fftw3f_shared "${CMAKE_CURRENT_LIST_DIR}/fftw/libfftw3f-3.dll")
set(fftw3l_shared "${CMAKE_CURRENT_LIST_DIR}/fftw/libfftw3l-3.dll")

configure_file(${fftw3_shared} ${CMAKE_CURRENT_BINARY_DIR}/fftw/libfftw3-3.dll COPYONLY)
configure_file(${fftw3f_shared} ${CMAKE_CURRENT_BINARY_DIR}/fftw/libfftw3f-3.dll COPYONLY)
configure_file(${fftw3l_shared} ${CMAKE_CURRENT_BINARY_DIR}/fftw/libfftw3l-3.dll COPYONLY)

# Use the Widgets module from Qt 5.
target_link_libraries(${PROGRAM_NAME}
    ${QWT_LIBRARY}
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Network
    Qt5::Svg
    Qt5::PrintSupport
    ${fftw3_shared}
    ${fftw3f_shared}
    ${fftw3l_shared}
)

if (BUILD_QWT)
  add_dependencies(${PROGRAM_NAME} QWT)
endif ()

# default version
set(VERSION_STRING "0.12.0")
set(VERSION_REVISION "0")

# get revision number from mercurial and parse version string
include(GetVersion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_STRING=\\\"${VERSION_STRING}\\\" ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_MAJOR=${VERSION_MAJOR} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_MINOR=${VERSION_MINOR} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_PATCH=${VERSION_PATCH} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_REVISION=\\\"${VERSION_REVISION}\\\" ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROGRAM_NAME=\\\"${PROGRAM_NAME}\\\" ")

# add make run target
add_custom_target(run
    COMMAND ${PROGRAM_NAME}
    DEPENDS ${PROGRAM_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# installing
install(TARGETS ${PROGRAM_NAME} DESTINATION bin)

# for windows put libraries to install directory
if (WIN32)
  file(GLOB WINDOWS_INSTALL_LIBRARIES
    "${CMAKE_BINARY_DIR}/windows_install_libraries/*.*")
  install(FILES ${WINDOWS_INSTALL_LIBRARIES} DESTINATION bin)
endif (WIN32)

# prepare menu item and icon
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/misc/program_name.desktop.in"
  "${CMAKE_BINARY_DIR}/${PROGRAM_NAME}.desktop")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/misc/program_name.png"
  "${CMAKE_BINARY_DIR}/${PROGRAM_NAME}.png" COPYONLY)

set(DESKTOP_FILE ${CMAKE_BINARY_DIR}/${PROGRAM_NAME}.desktop)
set(ICON_FILE ${CMAKE_BINARY_DIR}/${PROGRAM_NAME}.png)

# install menu item and icon
if (UNIX)
  install(FILES ${DESKTOP_FILE} DESTINATION share/applications/)
  install(FILES ${ICON_FILE} DESTINATION share/icons/hicolor/256x256/apps/)
endif (UNIX)

# uninstalling
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY)

if (UNIX)
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif (UNIX)

# testing
set(ENABLE_TESTS false CACHE BOOL "Build tests.")
if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()

# packaging
include(BuildLinuxAppImage)

if (UNIX)
  set(CPACK_GENERATOR "DEB")
elseif (WIN32)
  set(CPACK_GENERATOR "NSIS")
endif (UNIX)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROGRAM_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Small and simple software for plotting data from serial port")
set(CPACK_PACKAGE_CONTACT "Hasan Yavuz Özderya <hy@ozderya.net>")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_STRIP_FILES TRUE)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5 (>= 5.2.1), libqt5svg5 (>= 5.2.1), libqt5serialport5 (>= 5.2.1), libc6 (>= 2.19)")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Small and simple software for plotting data from serial port
 Supports binary data formats ([u]int8, [u]int16, [u]int32, float)
 and ASCII (as CSV). Captured waveforms can be exported in CSV format.
 Can also send simple user defined commands to serial port device.")

if (NOT QWT_USE_STATIC)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libqwt6-qt5 (>= 6.1.1)")
endif (NOT QWT_USE_STATIC)

if (UNIX)
  set(CPACK_PACKAGE_EXECUTABLES "${PROGRAM_NAME}")
elseif (WIN32)
  set(CPACK_PACKAGE_EXECUTABLES "${PROGRAM_NAME};${PROGRAM_DISPLAY_NAME}")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROGRAM_NAME}")
  set(CPACK_CREATE_DESKTOP_LINKS "${PROGRAM_NAME}")
  set(CPACK_NSIS_MODIFY_PATH "ON")
  set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/misc/serialplot.bmp")
  string(REPLACE "/" "\\\\" CPACK_PACKAGE_ICON ${CPACK_PACKAGE_ICON})
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
  set(CPACK_NSIS_MENU_LINKS
    "https://hg.sr.ht/~hyozd/serialplot/" "SerialPlot source code on sourcehut.org")
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL true)
endif (UNIX)

if (UNIX)
  # set debian package name
  string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_PACKAGE_NAME_LOWERCASE)
  find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
  if(DPKG_PROGRAM)
    execute_process(
      COMMAND ${DPKG_PROGRAM} --print-architecture
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CPACK_PACKAGE_FILE_NAME
      "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
  else(DPKG_PROGRAM)
    set(CPACK_PACKAGE_FILE_NAME
      "${CPACK_PACKAGE_NAME_LOWERCASE}_${PROJECT_VERSION}-${PROJECT_VERSION_REVISION}_${CMAKE_SYSTEM_NAME}")
  endif(DPKG_PROGRAM)
endif (UNIX)

include(CPack)
