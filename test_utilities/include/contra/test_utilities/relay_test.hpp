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

#ifndef TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_RELAY_TEST_HPP_
#define TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_RELAY_TEST_HPP_

#include <memory>
#include <utility>

#include "catch/catch.hpp"

#include "contra/relay.hpp"
#include "contra/test_utilities/conduit_data.hpp"
#include "contra/test_utilities/conduit_node_matcher.hpp"

namespace test_utilities {
template <typename T, typename ConstructorTuple,
          std::size_t... CONSTRUCTOR_INDICES>
std::unique_ptr<T> MakeUniqueFromTupleHelper(
    ConstructorTuple&& constructor_arguments,
    std::index_sequence<CONSTRUCTOR_INDICES...>) {
  (void)constructor_arguments;  // This is necessary, because otherwise MSVC
                                // thinks that constructor_arguments is not used
  return std::make_unique<T>(
      std::get<CONSTRUCTOR_INDICES>(constructor_arguments)...);
}

template <typename T, typename ConstructorTuple>
std::unique_ptr<T> MakeUniqueFromTuple(
    ConstructorTuple&& constructor_arguments) {
  return MakeUniqueFromTupleHelper<T>(
      std::forward<ConstructorTuple>(constructor_arguments),
      std::make_index_sequence<std::tuple_size<ConstructorTuple>::value>());
}

template <typename SenderTransportType,
          typename ReceiverTransportType = SenderTransportType,
          typename SenderConstructorParameters,
          typename ReceiverConstructorParameters>
void TestTransportRelay(
    SenderConstructorParameters&& sender_parameters = std::make_tuple(),
    ReceiverConstructorParameters&& receiver_parameters = std::make_tuple()) {
  auto sender = MakeUniqueFromTuple<contra::Relay<SenderTransportType>>(
      std::forward<SenderConstructorParameters>(sender_parameters));
  auto receiver = MakeUniqueFromTuple<contra::Relay<ReceiverTransportType>>(
      std::forward<ReceiverConstructorParameters>(receiver_parameters));

  sender->Send(test_utilities::ANY_NODE);
  const auto received_nodes = receiver->Receive();

  REQUIRE(received_nodes.size() == 1);
  REQUIRE_THAT(received_nodes[0], Equals(test_utilities::ANY_NODE));
}
}  // namespace test_utilities

#endif  // TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_RELAY_TEST_HPP_
