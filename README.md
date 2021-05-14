# sst-fooprojbar

## Instructions
Follow the instructions [here](https://sst-simulator.org/SSTPages/SSTBuildAndInstall_11dot0dot0_SeriesDetailedBuildInstructions/) to install SST-core.

Installing SST-FooProjBar is done exactly like SST-Elements:
``` bash
$ cd $HOME/scratch/src
$ git clone https://github.com/nicmcd/sst-fooprojbar
$ export SST_FOOPROJBAR_HOME=$HOME/local/sst-fooprojbar
$ export SST_FOOPROJBAR_ROOT=$HOME/scratch/src/sst-fooprojbar
$ cd $HOME/scratch/src/sst-fooprojbar
$ ./autogen.sh
$ ./configure --with-sst-core=$SST_CORE_HOME --prefix=$SST_FOOPROJBAR_HOME
$ make all
$ make install
```
