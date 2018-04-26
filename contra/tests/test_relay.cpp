//------------------------------------------------------------------------------
// contra -- a lightweigth transport library for conduit data
//
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualisation Group.
//------------------------------------------------------------------------------
//                                 License
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

#include "catch/catch.hpp"

#include "contra/relay.hpp"

#include "utilities/conduit_helpers.hpp"
#include "utilities/conduit_node_helper.hpp"

SCENARIO("Data gets transported", "[contra][contra::Relay][.]") {
  contra::Relay relay("relay.contra");

  relay.Send(test_utilities::ANY_NODE);

  conduit::Node received{relay.Receive()};

  REQUIRE_THAT(received, Equals(test_utilities::ANY_NODE));
}
