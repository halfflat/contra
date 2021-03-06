# ------------------------------------------------------------------------------
# contra -- a lightweight transport library for conduit data
#
# Copyright (c) 2018 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
# ------------------------------------------------------------------------------
#                                  License
#
# The license of the software changes depending on if it is compiled with or
# without ZeroMQ support. See the LICENSE file for more details.
# ------------------------------------------------------------------------------
#                          Apache License, Version 2.0
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
# ------------------------------------------------------------------------------
# Contra is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Contra is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Contra.  If not, see <https://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------------

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.hpp)
file(GLOB PYTHON_SOURCES *.py)

if(NOT WITH_TRANSPORT_BOOST_SHARED_MEMORY)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shared_memory_transport_relay.cpp")
endif()

if(NOT WITH_TRANSPORT_ZEROMQ)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zeromq_transport_relay.cpp")
endif()

set(PYCONTRA_OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/../pycontra
  CACHE PATH "Output path for pycontra python module"
  )
  
set(LIBRARIES contra)

if (WITH_TRANSPORT_BOOST_SHARED_MEMORY)
set(LIBRARIES ${LIBRARIES} contra_boost-shmem)
endif (WITH_TRANSPORT_BOOST_SHARED_MEMORY)

if (WITH_TRANSPORT_ZEROMQ)
set(LIBRARIES ${LIBRARIES} contra_zmq)
endif (WITH_TRANSPORT_ZEROMQ)

add_python_module(
  NAME _pycontra
  SOURCES ${SOURCES}
  HEADERS ${HEADERS}
  PYTHON_SOURCES ${PYTHON_SOURCES}
  INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  LINK_LIBRARIES ${LIBRARIES}
    ${CONAN_OR_CMAKE_boost_python}
  OUTPUT_DIRECTORY ${PYCONTRA_OUTPUT_DIR}
  )
