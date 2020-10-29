# Blinky application with RKH framework built by CMake

## Overview
The goal of Blinky application is to explain how to represent a "flat" 
state machine, how to use the timer services, and how to analyze the 
state machine behaviour by means of the RKH's trace facility. 
This application is the 'hello world' of RKH programming.

The behavior of Blinky is defined by a simple statechart that looks
as follows.

![Blinky state machine](images/state-machine.png)

## This tutorial contains:

[1\. Description](#1-description)

[2\. What RKH is?](#2-what-rkh-is)

[3\. Toolchain installation](#3-toolchain-installation)

[4\. Eclipse CDT project](#4-eclipse-cdt-project)

## 1\. Description
...

## 2\. What RKH is?
RKH is a flexible, efficient, highly portable, and freely available 
open-source state machine framework providing the infrastructure for quickly 
and safely developing reactive applications for real-time embedded systems.

![rkh-features](images/rkh-features.png)

RKH provides not only an unusual, efficient and straightforward method for implementing and executing state machines, but also the needed infrastructure to build reactive applications in embedded systems. It is composed of modules, procedures, and supporting tools; such as a method for implementing and executing flat state machines and statecharts, asynchronous messaging, cross-platform abstraction, run time tracing, time management, dynamic memory mechanism to deal with fragmentation, unit-test harness, plus others.

RKH allows developers to verify and validate a reactive application’s behaviour at runtime by means of the framework’s built-in tracer. It can utilize any traditional OS/RTOS or work without one. It also encourages the embedded software community to apply best principles and practices of software engineering for building flexible, maintainable and reusable software.

RKH is open source and licensed under the GNU v3.0\. You can find the [source code on GitHub](https://github.com/vortexmakes/RKH).

If you want to learn more about the benefits of this flexible, efficient and highly portable state machine framework read on [here](https://blogs.itemis.com/en/rkh-state-machine-framework-for-reactive-and-real-time-embedded-systems).

## 3\. Toolchain installation
### 3.1 CMake
[CMake](https://cmake.org/) is an open-source, cross-platform family of tools designed to build, test and package software.
There are several ways to install CMake, depending on your platform. Follow [this](https://cliutils.gitlab.io/modern-cmake/chapters/intro/installing.html) instructions to do that.

### 3.3 Trazer tool
RKH allows developers to verify and validate a reactive application's behaviour at runtime by means of its built-in tracer. In addition, RKH provides a very simple but powerful console application, called Trazer, to visualize the trace events' output in a legible manner. It can be downloaded and installed as follows.

1.  Download Trazer for Linux 64-bits from its [official repository](https://github.com/vortexmakes/Trazer/releases/download/3.2/RC_trazer_3_2_lnx64b.tar.gz)
2.  Copy downloaded file to a folder and extract it
3.  Change the directory to previous folder
4.  Check it is alright by executing ./trazer

## 4\. CMake project
### 4.1 Build
Open a console and follow the instructions below, which are the classic CMake build procedure:
1. `cd path/to/rkh-examples/blinky.cmake/build`
2. `cmake .. -DRKH_PLATFORM="__LNXGNU__" -DGIT_SUBMODULE=ON`
3. `make`

Alternatively, if you are using a newer version of CMake, you can instead do this:
1. `cd path/to/rkh-examples/blinky.cmake`
2. `cmake -S . -B build -DRKH_PLATFORM="__LNXGNU__" -DGIT_SUBMODULE=ON`
3. `cmake --build build`

### 4.2 Importing CMake project in Eclipse CDT
Run CMake using the Eclipse generator.
1. `cd path/to/rkh-examples/blinky.cmake`
2. `mkdir ../build`
3. `cd ../build`
4. `cmake ../blinky.cmake -DRKH_PLATFORM="__LNXGNU__" -DGIT_SUBMODULE=ON -G"Eclipse CDT4 - Unix Makefiles"`

Import this in Eclipse do the following:
1. Select 'File > Import...' to bring up the Import wizard. 
2. Choose 'Existing Project into Workspace' and click the 'Next' button.
3. Select the 'path/to/rkh-examples/build' project directory.
4. Click the 'Finish' button to import the selected project into the workspace. 

### 4.2 Project structure
#### _model_
It contains PulseCounter and PulseCounterMgr state machines

#### _src_
It includes both application code and BSP (Board Support Package) code. The most important files and directories are listed below:

*   _signals.h_: defines signals as enumerated constants, which are used as state machine triggers.
*   _events.h_: defines events types, which are derived from RKH framework types.
*   _priorities.h_: defines active object priorities as enumerated constants.
*   _PulseCounterMgr.h/.c_: specifies and implements the PulseCounterMgr active object and its paramterized state machine (PulseCounter). Please correlate this implementation with the state diagrams shown above.
*   _main.c_: contains the main() function, which initializes both BSP and PulseCounterMgr active object, then executes the RKH framework in order to orchestrates this reactive application.
*   _rkhcfg.h_: adapts and configures RKH in compile time.

#### _bsp_
It contains the source code of BSP for Linux platform. It emulates interrupts, implements the communication with Trazer tool and a simple event-loop, which is a non-preemptive cooperative scheduler.

#### _RKH_
Here is located the RKH framework's source code.

### 4.3 Build
*   Right-click on project 'Parameterized' in the 'Project Explorer'
*   Choose 'Build Project'

### 4.4 Run and debug
*   Open a console, change the directory where you previously downloaded Trazer, and run it by executing the following command line: `./trazer -t 6602`
*   Right-click on project 'Parameterized' in the Eclipse 'Project Explorer'
*   Choose 'Run As > Local C/C++ Application'

The embedded Eclipse console shows up and the application starts

![console-output](images/console-output.png)

In order to debug the example

*   Open a console, change the directory where you previously downloaded Trazer, and run it by executing the following command line: `./trazer -t 6602`
*   Right-click on project 'Parameterized' in the Eclipse 'Project Explorer'
*   Choose 'Debug As > Local C/C++ Application'

You will now see the debug perspective with the Parameterized application window open. The C/C++ editor repositions in 
the perspective.

![debug-snapshot](images/debug-snapshot.png)

### 4.5 Verify and validate
While the application is running, you can validate and verify its behaviour through the trace events showed on the Trazer output.
Each trace event includes a time stamp and additional information associated with it. A capture of Trazer output is shown below. 

![trazer-output](images/trazer-output.png)

It shows the trace records when the PulseCounterMgr dispatches a `evActive` trigger to a PulseCounter component, and then it get into the `Setup` state. After `ACT_MIN_TIME` seconds it goes to `Active` state and stay there until it receives `evInactive` trigger, causing it goes to `Inactive` state. Finally, after `INACT_MAX_TIME` seconds it returns `ACT_MIN_TIME` state. 

Since RKH can generate more than 100 different trace events during its execution, its trace module allow you to filter one or more of them in runtime, so you can choose the traces that you need.
