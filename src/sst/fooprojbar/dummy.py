"""
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
"""
import argparse
import sst

def main(args):
  print('\nStarting SST FooProjBar')

  # Makes dummys and sets parameters.
  dummys = []
  for dummy_id in range(args.num_dummys):
    dummy = sst.Component('Dummy_{}'.format(dummy_id), 'benchmark.Dummy')
    dummy.addParam('num_dummys', args.num_dummys)
    if args.initial_events != None:
      dummy.addParam('initial_events', args.initial_events)
    if args.stagger_events != None:
      dummy.addParam('stagger_events', args.stagger_events)
    if args.look_ahead != None:
      dummy.addParam('look_ahead', args.look_ahead)
    if args.remote_probability != None:
      dummy.addParam('remote_probability', args.remote_probability)
    if args.num_cycles != None:
      dummy.addParam('num_cycles', args.num_cycles)
    dummys.append(dummy)

  # Connects all dummys to all other dummys via links.
  port_nums = [0] * args.num_dummys
  for dummy_a in range(args.num_dummys):
    for dummy_b in range(dummy_a, args.num_dummys):
      link_name = 'link_{}_{}'.format(dummy_a, dummy_b)
      if dummy_a != dummy_b:
        link = sst.Link(link_name, '1ns')
        link.connect((dummys[dummy_a], 'port_{}'.format(port_nums[dummy_b])),
                     (dummys[dummy_b], 'port_{}'.format(port_nums[dummy_a])))
        port_nums[dummy_a] += 1
        port_nums[dummy_b] += 1

  # Limits the verbosity of statistics to any with a load level from 0-7.
  sst.setStatisticLoadLevel(7)

  # Determines where statistics should be sent.
  sst.setStatisticOutput('sst.statOutputCSV')
  sst.setStatisticOutputOption('filepath', args.stats_file)

  # Enables statistics on both dummys.
  sst.enableAllStatisticsForComponentType('benchmark.Dummy')

if __name__ == '__main__':
  ap = argparse.ArgumentParser()
  ap.add_argument('num_dummys', type=int, default=10,
                  help='Number of dummys.')
  ap.add_argument('stats_file', type=str,
                  help='Output statistics file file name.')
  ap.add_argument('-i', '--initial_events', type=int,
                  help='Number of initial events per dummy.')
  ap.add_argument('-t', '--total_events', type=int,
                  help='Number of total events per dummy.')
  args = ap.parse_args()
  main(args)
