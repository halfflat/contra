// -----------------------------------------------------------------------------
// contra -- a lightweight transport library for conduit data
//
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
// -----------------------------------------------------------------------------
//                                  License
//
// The license of the software changes depending on if it is compiled with or
// without ZeroMQ support. See the LICENSE file for more details.
// -----------------------------------------------------------------------------
//                          Apache License, Version 2.0
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
// -----------------------------------------------------------------------------
// Contra is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Contra is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Contra.  If not, see <https://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#include "contra/zmq/zeromq_transport.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "zmq.hpp"

namespace contra {

ZMQTransport::ZMQTransport(const Type type, const std::string& address,
                           bool wait_for_messages)
    : context_(1),
      socket_(context_, ZMQ_DEALER),
      wait_for_messages_(wait_for_messages) {
  if (type == ZMQTransport::Type::SERVER) {
    socket_.bind(address);
  } else if (type == ZMQTransport::Type::CLIENT) {
    socket_.connect(address);
  }
}

void ZMQTransport::Send(const Packet& packet) {
  serialized_buffer_.push_back(SerializePacket(packet));
  auto size = serialized_buffer_.at(next_to_be_sent_).size();
  zmq::message_t message(size);
  memcpy(message.data(), serialized_buffer_.at(next_to_be_sent_).data(), size);

  if (!wait_for_messages_) {
    if (!socket_.send(message, ZMQ_DONTWAIT)) {
      std::cout << "WARNING: No client available! Data is Lost!" << std::endl;
    }
  } else {
    socket_.send(message);
  }
  if (serialized_buffer_.size() > max_buffer_size_) {
    serialized_buffer_.erase(serialized_buffer_.begin());
  } else {
    next_to_be_sent_++;
  }
}

std::vector<Packet> ZMQTransport::Receive() {
  std::vector<Packet> packets;
  zmq::message_t received_message;
  if (!wait_for_messages_) {
    while (socket_.recv(&received_message, ZMQ_DONTWAIT)) {
      std::vector<uint8_t> message(received_message.size());
      std::memcpy(message.data(), received_message.data(),
                  received_message.size());
      packets.push_back(DeserializePacket(message));
      received_message.rebuild();
    }
  } else {
    socket_.recv(&received_message);
    std::vector<uint8_t> message(received_message.size());
    std::memcpy(message.data(), received_message.data(),
                received_message.size());
    packets.push_back(DeserializePacket(message));
    received_message.rebuild();
  }
  return packets;
}

void ZMQTransport::SetWaitForMessages(const bool SetWaitForMessages) {
  wait_for_messages_ = SetWaitForMessages;
}

bool ZMQTransport::GetWaitForMessages() const { return wait_for_messages_; }

}  // namespace contra
