# Strict Priority Queuing (SPQ) & Deficit Round Robin (DRR)

## Purpose
This project is used to help test different quality of service (QoS) packet scheduler implementations. Each implementation is built on top of an abstract DiffServ class. This class could easily be used to implement other packet scheduling algorithms. 

SQP and DRR have been implemented with both byte mode and packet mode scheduling, and are drop tail queues.

The project also comes with ns-3 applications that allow for testing and validation of the SPQ and DRR implementations. These can be found in `$ cd ns-3-allinone/ns-3-dev/scratch` in `spq.cc` and `drr.cc` respectively. Testing uses a 3 node topology for each implementation, with a two clients and a router node in between.

## Installation
The best way to install this NS3 project is to clone this repository, and then go through the [NS3 installation guide](https://www.nsnam.org/wiki/Installation)

The only supported platform for this project in Linux, Ubuntu LTS versions 16.04 or 18.04 are the reccomended distribution.
Running Ubuntu on a virtual machine installed on a macOS or Windows machine is also a viable option.

## How to Run
Once properly installed, follow these steps to run the program.
- Navigate to the ns-3-dev directory `$ cd ns-3-allinone/ns-3-dev`
- Configure the waf `$ ./waf configure`
- Build the executable `$ ./waf build`
- From here there are 2 option, run and SQP or DRR simulation
    - SQP `./waf --run "spq --i=scratch/spq.json"`
    - DRR `./waf --run "drr --i=scratch/drr.json"`

## Additional Options when Running
Additionally, there is a configuration file for both the SPQ and DRR simulations.
- Navigate to `$ cd ns-3-allinone/ns-3-dev/scratch` and in this directory you can find `spq.json` and `drr.json`
- `spq.json` can be configured to increase the number of queues, and the priority for each queue
- `drr.json` can be configured to increase the number of queues, and the quantum (or weight) given to packets in each queue
    - Note: The number of queues must match the number of priorities or quantum’s declared.

## Results
SPQ
![alt text](https://raw.githubusercontent.com/saa-software/cs621-p2/master/post-spq.png)

DRR
![alt text](https://raw.githubusercontent.com/saa-software/cs621-p2/master/DRR%20plot.png)

## Add-ons
- Json parsing from [JsonCPP](https://github.com/open-source-parsers/jsoncpp)
