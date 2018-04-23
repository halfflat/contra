#-------------------------------------------------------------------------------
# contra -- a lightweigth transport library for conduit data
#
# Copyright (c) 2018 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
#-------------------------------------------------------------------------------
#                                  License
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.8.2)

project(contra)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)

include(ConanHelpers)
include(CTest)
include(GenerateExportHeader)
include(rwthvr)

enable_testing()

conan_or_find_package(catch REQUIRED)

conan_or_find_package(cpplint REQUIRED)
include(cpplint)

conan_or_find_package(cppcheck REQUIRED)
include(cppcheck)

add_subdirectory(contra)

get_property(ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
get_property(ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)

add_test_cppcheck(NAME "contra-tests--cppcheck"
    ${ALL_SOURCES}
  )
  
add_test_cpplint(NAME "contra-tests--cpplint"
    ${ALL_SOURCES} ${ALL_HEADERS}
  )
