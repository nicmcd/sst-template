/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * - Neither the name of prim nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior
 * written permission.
 *
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "sst/fooprojbar/Dummy.h"

namespace SST {
namespace FooProjBar {

void Dummy::Event::serialize_order(
    SST::Core::Serialization::serializer& _serializer) {
  SST::Event::serialize_order(_serializer);
  _serializer & payload;
};

Dummy::Dummy(SST::ComponentId_t _id, SST::Params& _params)
    : SST::Component(_id) {
  // Configures output.
  output_.init("[@t] FooProjBar." + getName() + ": ", 0, 0,
               SST::Output::STDOUT);

  // Retrieves parameter values.
  initial_events_ = _params.find<uint32_t>("initial_events", 1);
  output_.verbose(CALL_INFO, 2, 0, "initial_events=%u\n", initial_events_);
  total_events_ = _params.find<uint32_t>("total_events", 1);
  output_.verbose(CALL_INFO, 2, 0, "total_events=%u\n", initial_events_);
  sst_assert(total_events_ >= initial_events_, CALL_INFO, -1, "ERROR\n");

  // Seeds the random number generator.
  random_.seed(12345678 + getId());

  // Configures the links for all ports.
  int port_num = 0;
  while (true) {
    std::string port_name = "port_" + std::to_string(port_num);
    if (!isPortConnected(port_name)) {
      break;
    }
    SST::Link* link = configureLink(
        port_name, new SST::Event::Handler<Dummy, int>(
            this, &Dummy::handleEvent, port_num));
    if (!link) {
      output_.fatal(CALL_INFO, -1, "unable to configure link %u\n", port_num);
    }
    links_.push_back(link);
    port_num++;
  }
  sst_assert(links_.size() > 0, CALL_INFO, -1, "ERROR\n");

  // Registers the statistics.
  event_count_ = 0;
  event_count_stat_ = registerStatistic<uint64_t>("event_count");

  // Sets the time base.
  registerTimeBase("1ns");

  // Tells the simulator not to end without us.
  registerAsPrimaryComponent();
  primaryComponentDoNotEndSim();
}

Dummy::~Dummy() {}

void Dummy::setup() {
  output_.verbose(CALL_INFO, 1, 0, "Setup() ns=%lu\n",
                  getCurrentSimTimeNano());
  for (int ev = 0; ev < initial_events_; ev++) {
    sendNextEvent();
  }
}

void Dummy::finish() {
  output_.verbose(CALL_INFO, 1, 0, "Finish()\n");
}

void Dummy::sendNextEvent() {
  uint64_t link_index = random_.generateNextUInt64() % (links_.size() - 1);
  SST::Link* link = links_.at(link_index);
  Dummy::Event* event = new Dummy::Event();
  event->payload = 'a';
  output_.verbose(CALL_INFO, 3, 0, "Sending event\n");
  link->send(event);
  event_count_++;
  event_count_stat_->addData(1);
}

void Dummy::handleEvent(SST::Event* _event, int _port_num) {
  Dummy::Event* event = dynamic_cast<Dummy::Event*>(_event);
  if (event) {
    output_.verbose(CALL_INFO, 3, 0, "Received event on port %u ns=%lu.\n",
                    _port_num, getCurrentSimTimeNano());
    delete _event;
    if (event_count_ < total_events_) {
      sendNextEvent();
    } else {
      primaryComponentOKToEndSim();
    }
  } else {
    output_.fatal(CALL_INFO, -1, "Received bad event type on port %u.\n",
                  _port_num);
  }
}

}  // namespace FooProjBar
}  // namespace SST
