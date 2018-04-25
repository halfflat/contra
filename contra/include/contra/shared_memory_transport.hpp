//------------------------------------------------------------------------------
// contra -- a lightweigth transport library for conduit data
//
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
//------------------------------------------------------------------------------
//                                  License
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#ifndef CONTRA_INCLUDE_CONTRA_SHARED_MEMORY_TRANSPORT_HPP_
#define CONTRA_INCLUDE_CONTRA_SHARED_MEMORY_TRANSPORT_HPP_

#include "boost/interprocess/allocators/allocator.hpp"
#include "boost/interprocess/managed_shared_memory.hpp"

namespace contra {

class SharedMemoryTransport {
 public:
  class Create {};
  class Access {};

  struct Packet {
    std::vector<char> schema;
    std::vector<uint8_t> data;
  };

  using ManagedSharedMemory = boost::interprocess::managed_shared_memory;
  using SegmentManager = ManagedSharedMemory::segment_manager;
  using Allocator = boost::interprocess::allocator<Packet, SegmentManager>;
  using PacketStorage = std::vector<Packet, Allocator>;

  SharedMemoryTransport() = delete;
  explicit SharedMemoryTransport(const Create&);
  explicit SharedMemoryTransport(const Access&);
  SharedMemoryTransport(const SharedMemoryTransport&) = delete;
  SharedMemoryTransport(SharedMemoryTransport&&) = delete;
  virtual ~SharedMemoryTransport();

  void Destroy();

  std::size_t GetFreeSize() const;

  void Store(const Packet& packet);
  std::vector<Packet> Read();

  bool IsEmpty() const;

  static constexpr const char* SegmentName() { return "packet-shared-memory"; }
  static constexpr const char* PacketStorageName() { return "PacketStorage"; }
  static constexpr const char* ReferenceCountName() { return "ReferenceCount"; }

  static constexpr std::size_t InitialSize() { return 1073741824u; }

  SharedMemoryTransport& operator=(const SharedMemoryTransport&) = delete;
  SharedMemoryTransport& operator=(SharedMemoryTransport&&) = delete;

  int GetReferenceCount() const;

 private:
  PacketStorage* ConstructPacketStorage();
  int* ConstructReferenceCount();

  PacketStorage* FindPacketStorage();
  int* FindReferenceCount();

  ManagedSharedMemory segment_;
  PacketStorage* packet_storage_;
  int* reference_count_;
};

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_SHARED_MEMORY_TRANSPORT_HPP_
