# sst-bksim

## Instructions
Follow the instructions [here](http://sst-simulator.org/SSTPages/SSTBuildAndInstall10dot1dot0SeriesDetailedBuildInstructions) to install SST-core.

Installing SST-Bksim is done exactly like SST-Elements:
``` bash
$ cd $HOME/scratch/src
$ git clone https://github.com/nicmcd/sst-bksim
$ export SST_BKSIM_HOME=$HOME/local/sst-bksim
$ export SST_BKSIM_ROOT=$HOME/scratch/src/sst-bksim
$ cd $HOME/scratch/src/sst-bksim
$ ./autogen.sh
$ ./configure --with-sst-core=$SST_CORE_HOME --prefix=$SST_BKSIM_HOME
$ make all
$ make install
```
