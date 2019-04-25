# QoS - Strict Priority Queuing (SPQ) & Deficit Round Robin (DDR)

## Installation
The best way to install this NS3 project is to clone this repository, and then go through the [NS3 installation guide](https://www.nsnam.org/wiki/Installation)
<br />
The only supported platform for this project in Linux, Ubuntu LTS versions 16.04 or 18.04 are the reccomended distribution.
Running Ubuntu on a virtual machine installed on a macOSor Windows machine is also a viable option.

## How to Run
Once properly installed, follow these steps to run the program.
- Navigate to the ns-3-dev directory `$ cd ns-3-allinone/ns-3-dev`
- Configure the waf `$ ./waf configure`
- Build the executable `$ ./waf build`
- From here there are 2 option, run and SQP or DDR simulation
    - SQP `./waf --run spq`
    - DDR `./waf --run ddr`

## Additional Options when Running
Additionally, there is a configuration file for both the SPQ and DDR simulations.
- Navigate to `$ cd ns-3-allinone/ns-3-dev/scratch` and in this directory you can find `spq.json` and `ddr.json`
- `spq.json` can be configured to increase the number of queues, and the priority for each queue
- `ddr.json` can be configured to increase the number of queues, and the quantum (or weight) given to packets in each queue
    - Note: The number of queues must match the number of prioities or quantums declared.

## Add-ons
- Json parsing from JsonCPP