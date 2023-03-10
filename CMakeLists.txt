project(xxtea-files)
cmake_minimum_required(VERSION 2.6)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_executable(xxtea-files src/xtteaFiles.cpp src/encryptFiles.cpp src/decodeFiles.cpp)
else()
  add_executable(xxtea-files src/xtteaFiles.cpp src/encryptFiles.cpp src/decodeFiles.cpp)
endif()
