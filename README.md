# System Server

[English | [简体中文](./README_zh-cn.md)]

## Overview

`System Server` is one of the core components of the Openvela operating system. It is responsible for starting and managing multiple system services, including:

1. Application management service

2. Package management service

3. Window management service

4. Brightness management service

## System Service Set

The System Service Set currently includes the following system services:

### 1 Application Management Service
The Application Management Service is responsible for managing the lifecycle and task stack of applications. It includes the following features:

- Manage application lifecycle, such as starting, stopping, pausing, and resuming applications.
- Manage the application task stack, such as creating, destroying, updating, etc.
- Manage the memory usage of applications, such as monitoring memory usage, low memory warnings, etc.

### 2 Package Management Service
The Package Management Service is responsible for managing the installation, uninstallation, updating, and permissions of applications. It includes the following features:

- Install, uninstall, and update applications.
- Manage application permissions, including granting and revoking permissions.
- Listen for application installation, uninstallation, and update events.

### 3 Window Management Service
The Window Management Service is responsible for managing the display and management of application windows. It includes the following features:

- Manage application windows, including creating, displaying, hiding, closing, etc.
- Manage window display properties, such as size, position, etc.
- Input event listening and processing, such as touch, key presses, etc.
- Manage window transition animations.

### 4 Brightness Management Service
The Brightness Management Service is responsible for managing the brightness of the screen. It includes the following features:

- Manage the brightness of the screen.
- Listen for screen brightness change events.

## Compilation and Execution

`System Server` uses openvela source code to build the system for compilation and execution. Before compiling the openvela source code, the appropriate compilation tools and dependency libraries need to be installed. When compiling the openvela source code, the option to compile the System Server needs to be selected, and the configuration options for each service also need to be opened before compiling.

The configuration options are as follows:
- `SYSTEM_SERVER`: This option needs to be enabled to compile the system service.
- `SYSTEM_ACTIVITY_SERVICE`: This option needs to be enabled to compile the application management service.
- `SYSTEM_PACKAGE_SERVICE`: This option needs to be enabled to compile the package management service.
- `SYSTEM_WINDOW_SERVICE`: This option needs to be enabled to compile the window management service.
- `SYSTEM_BRIGHTNESS_SERVICE`: This option needs to be enabled to compile the brightness management service.

Running `System Server` on a device requires root privileges. `System Server` can be started in the background using the adb command, as shown below:

```
adb shell systemd &
```