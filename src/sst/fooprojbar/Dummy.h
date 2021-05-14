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
#ifndef SST_FOOPROJBAR_DUMMY_H_
#define SST_FOOPROJBAR_DUMMY_H_

#include <sst/core/component.h>
#include <sst/core/event.h>
#include <sst/core/link.h>
#include <sst/core/output.h>
#include <sst/core/params.h>
#include <sst/core/eli/elementinfo.h>
#include <sst/core/rng/mersenne.h>

#include <cstdint>

namespace SST {
namespace FooProjBar {

class Dummy : public SST::Component {
 private:
  struct Event : public SST::Event {
    Event() = default;
    ~Event() override = default;
    char payload;
    void serialize_order(SST::Core::Serialization::serializer& _serializer)
        override;
    ImplementSerializable(SST::FooProjBar::Dummy::Event);
  };

 public:
  Dummy(SST::ComponentId_t _id, SST::Params& _params);
  ~Dummy() override;

  void setup() override;
  void finish() override;

  SST_ELI_REGISTER_COMPONENT(
      Dummy,
      "fooprojbar",
      "Dummy",
      SST_ELI_ELEMENT_VERSION(1,0,0),
      "FooProjBar Dummy",
      COMPONENT_CATEGORY_UNCATEGORIZED)

  SST_ELI_DOCUMENT_PORTS(
        {"port_%(portnum)d",
         "Links to self or other dummys.",
         {"fooprojbar.Dummy"}})

  SST_ELI_DOCUMENT_PARAMS(
      {"initial_events",
       "Number of initial events created by this dummy.",
       "1"},
      {"total_events",
       "Total number of events to create by this dummy.",
       "1"})

  SST_ELI_DOCUMENT_STATISTICS(
      {"event_count",
       "The count of events generated", "unitless", 1})

 private:
  Dummy(const Dummy&) = delete;
  void operator=(const Dummy&) = delete;

  void sendNextEvent();
  virtual void handleEvent(SST::Event* _event, int _port_num);

  // Output
  SST::Output output_;

  // Parameters
  uint32_t initial_events_;
  uint32_t total_events_;

  // Links
  std::vector<SST::Link*> links_;

  // Random
  SST::RNG::MersenneRNG random_;

  // Statistics
  uint64_t event_count_;
  SST::Statistics::Statistic<uint64_t>* event_count_stat_;
};

}  // namespace FooProjBar
}  // namespace SST

#endif  // SST_FOOPROJBAR_WORKER_H_
