# Time Event with Yakindu and RKH framework on Linux

## Overview
This is a simple example of how to use the RKH framework together with Yakindu Statechart Tool for using Time Event´s in statecharts models.
  
## This tutorial contains:

[1\. TimeEvt Statechart Model](#1-timeevt-statechart-model)

[2\. What RKH is?](#2-what-rkh-is)

[3\. Toolchain installation](#3-toolchain-installation)

[4\. Yakindu project](#4-yakindu-project)

## 1\. TimeEvt Statechart Model

The behavior of timeEvt is defined by a simple statechart that looks as follows.

![state-machine](images/state-machine.png)

## 2\. What RKH is?
RKH is a flexible, efficient, highly portable, and freely available open-source state machine framework providing the infrastructure for quickly and safely developing reactive applications for real-time embedded systems.

![rkh-features](images/rkh-features.png)

RKH provides not only an unusual, efficient and straightforward method for implementing and executing state machines, but also the needed infrastructure to build reactive applications in embedded systems. It is composed of modules, procedures, and supporting tools; such as a method for implementing and executing flat state machines and statecharts, asynchronous messaging, cross-platform abstraction, run time tracing, time management, dynamic memory mechanism to deal with fragmentation, unit-test harness, plus others.

RKH allows developers to verify and validate a reactive application’s behaviour at runtime by means of the framework’s built-in tracer. It can utilize any traditional OS/RTOS or work without one. It also encourages the embedded software community to apply best principles and practices of software engineering for building flexible, maintainable and reusable software.

RKH is open source and licensed under the GNU v3.0\. You can find the [source code on GitHub](https://github.com/vortexmakes/RKH).

If you want to learn more about the benefits of this flexible, efficient and highly portable state machine framework read on [here](https://blogs.itemis.com/en/rkh-state-machine-framework-for-reactive-and-real-time-embedded-systems).

## 3\. Toolchain installation
### 3.1 Install YAKINDU Statechart Tool
YAKINDU Statechart Tools features an intuitive combination of graphical and textual notation for modelling state diagrams. While states, transitions and state hierarchies are graphical elements, all declarations and actions are specified using a textual notation. The usability of the statechart editor is simply fascinating. Form more information please visit:

[Yakindu SCT site](https://www.itemis.com/en/yakindu/state-machine/)

![Yakindu](images/Yakindu.png)

Download Linux installer from here: [Yakinud Download Site](https://info.itemis.com/state-machine/download-yakindu-statechart-tools)

### 3.2 Install RKH code generator
Open Yakindu SCT and goes to:

1.  Help > Install New Software ... from the menu.
2.  Select the Yakindu Labs entry from the Work with: pull down list. If there is no such entry then

1.  Choose Add
2.  Enter `http://updates.yakindu.com/statecharts/labs` in the Location field and optionally choose a name
3.  Press OK

4.  Unfold the top level node and check the RKH generator feature.
5.  Finish the installation wizard by choosing Next > until finished.
6.  Restart.

![RKH-generator](images/RKH-generator.png)

### 3.3 Get RKH framework
In order to build this example you have to download the RKH framework and install the Trazer tool. RKH can be obtained from its official repository by using the following Git commands:

1.  `cd path/to/rkh-examples/`
2.  `git submodule init timeEvt.yakindu.linux/RKH`
3.  `git submodule update`

### 3.4 Install Trazer tool
RKH allows developers to verify and validate a reactive application’s behaviour at runtime by means of its built-in tracer. In addition, RKH provides a very simple but powerful console application, called Trazer, to visualize the trace events’ output in a legible manner. It can be downloaded and installed as follows.

1.  Download Trazer for Linux 64-bits from its [official repository](https://github.com/vortexmakes/Trazer/releases/download/3.2/RC_trazer_3_2_lnx64b.tar.gz)
2.  Copy downloaded file to a folder and extract it
3.  Change the directory to previous folder
4.  Check it is alright by executing ./trazer

## 4\. Yakindu project
### 4.1 Project structure
<dl>

<dt>model</dt>

<dd>contains TimeEvt state machine (timeEvt.sct) and generator model (timeEvt.sgen)</dd>

<dt>src</dt>

<dd>Includes both application code and BSP (Board Support Package) code. The most important files and directories are listed below:</dd>

*   _signals.h_: defines signals as enumerated constants, which are used as state machine triggers.
*   _main.c_: contains the main() function, which initializes both BSP and Blinky active object, then executes the RKH framework in order to orchestrates this reactive application.
*   _rkhcfg.h_: adapts and configures RKH in compile time.
*   _bsp_: this folder contains the source code of BSP for Linux platform. It emulates interrupts, implements the communication with Trazer tool and a simple event-loop, which is a non-preemptive cooperative scheduler. In the Yakindu’s Standard Edition you cannot include header files, but it is possible to create operations, in which the platform specific C code can be called. Within these operations we can call the specific code to interact with the platform, which is located in this folder.

<dt>src-gen</dt>

<dd>Includes the generated C code compatible with RKH framework, which will be automatically be updated when you edit and save the statechart.</dd>

*   _TimeEvt.h/.c_: specifies and implements the state machine of TimeEvt active object. Please correlate this implementation (TimeEvt.c) with the state diagram shown above.
*   _TimeEvt.h/.c_: specifies and implements the state machine actions (entries, exits, effects and guards).
*   _TimeEvtActRequired.h_: in the Yakindu’s Standard Edition you cannot include header files, but it is possible to create operations, in which the Blinky specific c code can be called. Within these operations we can call the specific C code to interact with the platform.

<dt>rkh</dt>

<dd>Here is located the RKH framework’s source code.</dd>

</dl>

### 4.2 Build
*   Right-click on project 'timeEvt.yakindu.linux' in the 'Project Explorer'
*   Choose 'Build Project'

### 4.3 Run and debug
*   Open a console, change the directory where you previously downloaded Trazer, and run it by executing the following command line: `./trazer -t 6602`
*   Right-click on project 'blinky' in the Eclipse 'Project Explorer'
*   Choose 'Run As > Local C/C++ Application'

The embedded Eclipse console shows up and the application starts

![console-output](images/console-output.png)

In order to debug the example

*   Open a console, change the directory where you previously downloaded Trazer, and run it by executing the following command line: `./trazer -t 6602`
*   Right-click on project 'blinky' in the Eclipse 'Project Explorer'
*   Choose 'Debug As > Local C/C++ Application'

You will now see the debug perspective with the blinky application window open. The C/C++ editor repositions in the perspective.

![debug-snapshot](images/debug-snapshot.png)

### 4.4 Verify and validate
While the application is running, you can validate and verify its behaviour through the trace events showed on the Trazer output. Each trace event includes a time stamp and additional information associated with it. A capture of Trazer output is shown below. It shows the trace records when the state machine processes a `evStart` trigger and get into the `Waiting` state. `After 5 secs` goes to `DoneWaiting` state and stay there until `After 10 secs` trigger is preocessed returning to `Off` state. Since RKH can generate more than 100 different trace events during its execution, its trace module allow you to filter one or more of them in runtime, so you can choose the traces that you need.

![trazer-output](images/trazer-output.png)
