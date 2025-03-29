## Overview

This Minimum Viable Product (MVP) aims to create a basic framework for connecting robots to machine learning techniques that affect their physical functioning, specifically designed for Raspberry Pi architecture and implemented in C++.

##  Features

- Implementation of an algorithm that allow various controllers to work on indivisual threads.
- Ability to collect data and write to SD card, for use of training.
- ML Model Interface: That is created by implementing interface AiPredictor 
- Data Collection: Implement basic data logging for sensor readings and robot actions. This is performed using the EaiTrainingData interface, and AiGenerateData
- Remote control functionality based on JSON Ryder Robot Protocol (RRP), and using TCP/IP
- State, and environment control provided through manifest.json file, and using interfaces StateIface, and StateManagerIface
- Event queuing and event handling provided through implementation of EventHandler

## Implementation 

for implementation of this software refer to [LD001](https://github.com/Ryder-Robots/ld001)



# BUILD SERVICE

```
mkdir build
cd build
cmake -S .. -B . 
cmake --build . --target install/strip
cmake --build . --target test
cpack -G DEB 
```

## Memory Leak Test

```
cd build
ctest -T memcheck
view Testing/Temporary/MemoryChecker.*.log
```

# PUBLISHING

```
gh release create 1.0.0 --generate-notes ./build/lib/librrobot.so
```

# INSTALLING SERVICE

Before using these instructions, a GitHub token must be created and the environmental variable
GH_TOKEN must be set up.

```
gh release -R git@github.com:Ryder-Robots/fatcnt.git download --pattern '*.deb'
dpkg -i rrobot_*.deb 
```


# RUN SERVICE

```
systemctl start fatcnt
journalctl -u fatcnt -f
```


# DESIGN

```

RRP_COMMAND, STATE, MODE, ENV ->  EaiHandler -> AiPredictor->addFeature()
FC <-  EaiHandler <- AiPredictor.predict()

```