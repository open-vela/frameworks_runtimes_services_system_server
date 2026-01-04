# System Server (系统服务)

[ 简体中文 | [English](./README.md) ]

## 简介

**System Server** 是 openvela 操作系统的核心组件，负责启动、管理及协调系统关键服务。它充当了系统的大脑，维护着系统的正常运行状态。

当前版本包含以下核心子服务：

1. 应用管理服务 (Activity Manager)
2. 包管理服务 (Package Manager)
3. 窗口管理服务 (Window Manager)
4. 亮度管理服务 (Brightness Manager)

**图1** 系统服务架构图

![系统服务架构](./docs/SystemServer_Architecture.png)

## 子服务详解

### 1. 应用管理服务 (Activity Manager Service)

应用管理服务是负责管理应用程序的生命周期和任务栈。它包括以下功能：

- 管理应用生命周期，如启动、停止、暂停和恢复应用程序。
- 管理应用程序任务栈，如任务栈的创建、销毁、更新等。
- 管理应用程序的内存使用情况，如监测内存使用，低内存警告等。

### 2. 包管理服务 (Package Manager Service)

包管理服务是负责管理应用程序的安装、卸载、更新和权限等。它包括以下功能：

- 安装、卸载和更新应用程序。
- 管理应用程序权限，包括权限的授予、撤销等。
- 监听应用程序安装、卸载和更新事件。

### 3. 窗口管理服务 (Window Manager Service)

窗口管理服务是负责管理应用程序窗口的显示和管理。它包括以下功能：

- 管理应用程序窗口，包括创建、显示、隐藏、关闭等。
- 管理窗口的显示属性，如尺寸、位置等。
- 输入事件监听处理，如触摸、按键等。
- 管理窗口的过渡动画。

### 4. 亮度管理服务 (Brightness Manager Service)

亮度管理服务是负责管理屏幕的亮度。它包括以下功能：

- 管理屏幕亮度。
- 监听屏幕亮度变化事件。

## 目录结构

```bash
├── Kconfig              # 构建配置定义文件，包含各子服务的编译开关
└── SystemServer.cpp     # System Server 主程序入口及启动逻辑
```

## 编译与配置

System Server 基于**构建系统**进行编译。在编译前，请确保已正确配置 `Kconfig` 选项以包含所需的子服务。

### 配置选项 (Kconfig)

请在编译配置菜单中开启以下选项：

| 定义名称                    | 描述         | 备注                        |
| :-------------------------- | :----------- | :-------------------------- |
| `SYSTEM_SERVER`             | **主开关**   | 编译 System Server 必须开启 |
| `SYSTEM_ACTIVITY_SERVICE`   | 应用管理服务 | 可选组件                    |
| `SYSTEM_PACKAGE_SERVICE`    | 包管理服务   | 可选组件                    |
| `SYSTEM_WINDOW_SERVICE`     | 窗口管理服务 | 可选组件                    |
| `SYSTEM_BRIGHTNESS_SERVICE` | 亮度管理服务 | 可选组件                    |

## 运行指南

在目标设备上运行 `SystemServer` 需要 **Root 权限**。

可以通过 ADB 调试桥手动启动服务至后台：

```bash
# 注意：生成的二进制文件名假定为 systemd，请根据实际编译输出调整
adb shell systemd &
```
