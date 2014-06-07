# Module for locating OpenCL.
#
# Customizable variables:
#   OPENCL_ROOT_DIR
#     Specifies OpenCL's root directory. The find module uses this variable to
#     locate OpenCL. The variable will be filled automatically unless explicitly
#     set using CMake's -D command-line option. Instead of setting a CMake
#     variable, an environment variable called OCLROOT can be used.
#     While locating the root directory, the module will try to detect OpenCL
#     implementations provided by AMD's Accelerated Parallel Processing SDK,
#     NVIDIA's GPU Computing Toolkit and Intel's OpenCL SDK by examining the
#     AMDAPPSDKROOT, CUDA_PATH and INTELOCLSDKROOT environment variables,
#     respectively.
#
# Read-only variables:
#   OPENCL_FOUND
#     Indicates whether OpenCL has been found.
#
#   OPENCL_INCLUDE_DIRS
#     Specifies the OpenCL include directories.
#
#   OPENCL_LIBRARIES
#     Specifies the OpenCL libraries that should be passed to
#     target_link_libararies.
#
#
# Copyright (c) 2012 Sergiu Dotenco
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTOPENCLLAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

INCLUDE (FindPackageHandleStandardArgs)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_OPENCL_POSSIBLE_LIB_SUFFIXES lib/Win64 lib/x86_64 lib/x64)
ELSE (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_OPENCL_POSSIBLE_LIB_SUFFIXES lib/Win32 lib/x86)
ENDIF (CMAKE_SIZEOF_VOID_P EQUAL 8)

LIST (APPEND _OPENCL_POSSIBLE_LIB_SUFFIXES lib/nvidia-current)

FIND_PATH (OPENCL_ROOT_DIR
  NAMES OpenCL/cl.h
        CL/cl.h
        include/CL/cl.h
        include/nvidia-current/CL/cl.h
  HINTS ${CUDA_TOOLKIT_ROOT_DIR}
  PATHS ENV OCLROOT
        ENV AMDAPPSDKROOT
        ENV CUDA_PATH
        ENV INTELOCLSDKROOT
  PATH_SUFFIXES cuda
  DOC "OpenCL root directory")

FIND_PATH (OPENCL_INCLUDE_DIR
  NAMES OpenCL/cl.h CL/cl.h
  HINTS ${OPENCL_ROOT_DIR}
  PATH_SUFFIXES include include/nvidia-current
  DOC "OpenCL include directory")

FIND_LIBRARY (OPENCL_LIBRARY
  NAMES OpenCL
  HINTS ${OPENCL_ROOT_DIR}
  PATH_SUFFIXES ${_OPENCL_POSSIBLE_LIB_SUFFIXES}
  DOC "OpenCL library")

SET (OPENCL_INCLUDE_DIRS ${OPENCL_INCLUDE_DIR})
SET (OPENCL_LIBRARIES ${OPENCL_LIBRARY})

