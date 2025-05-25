# rrobot Overview

**rrobot** is an MVP (Minimum Viable Product) software stack for an AI-driven robot, designed for rapid prototyping and experimentation. The repository provides a modular framework for robotic navigation, with a focus on using ultrasonic sensors as the primary means of environmental perception.

## CAVEATS
* This code is compiled for use on Raspberry Pi 5 and Raspberry Pi 4B platforms.
* It interfaces with an Arduino BLE33 micro-controller.
* The micro-processor can be swapped out for other processors, provided that the
* RRMULTIWII protocol, as defined in this source base, is strictly adhered to.
* CODE is currently in a non-working state

# CONTRIBUTIONS

For contributions to the source base please contact azzmosphere@gmail.com  using the subject line R-Robot.

## Key Features

- **Ultrasonic-Based Navigation:** The robot uses ultrasonic sensors to detect obstacles and navigate its environment.
- **Greedy Algorithm (Vanilla Mode):** By default, rrobot employs a simple greedy algorithm to emulate basic navigation and obstacle avoidance.
- **Extensible AI Control:** The navigation logic can be replaced with more advanced algorithms, such as Reinforcement Learning (RL) or other AI methods compatible with [dlib](http://dlib.net/).
- **Modular Design:** The codebase is structured to allow easy swapping of navigation strategies, making it suitable for research and development.

## Use Cases

- Rapid prototyping of AI navigation algorithms.
- Educational projects involving robotics and AI.
- Benchmarking different navigation strategies in a controlled environment.


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
