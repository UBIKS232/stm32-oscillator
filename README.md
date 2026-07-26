# Note

---

## 1. 跨平台框架

### 1.1 配置要求

- 工具: CMake, Ninja, `arm-none-eabi-*`工具链, OpenOCD. 

- VSCode拓展: `llvm-vs-code-extensions.vscode-clangd`, `marus25.cortex-debug`, `ms-vscode.cmake-tools`. 

---

### 1.2 环境搭建

#### a. `.vscode/settings.json` — clangd语法解析配置

```json
{
    "[c]": {
        "editor.wordBasedSuggestions": "off",
        "editor.semanticHighlighting.enabled": true,
        "editor.stickyScroll.defaultModel": "foldingProviderModel",
        "editor.suggest.insertMode": "replace",
        "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
    },
    "clangd.arguments": [
        "--clang-tidy",
        "--all-scopes-completion",
        "--completion-style=detailed",
        "--header-insertion=iwyu",
        "--pch-storage=disk",
        "--log=error",
        "--j=5",
        "--background-index",
        "--compile-commands-dir=${workspaceFolder}/build/Debug"
    ],
    "cmake.configureOnEdit": false,
    "cmake.pinnedCommands": [],
    "C_Cpp.intelliSenseEngine": "disabled",
    "cortex-debug.variableUseNaturalFormat": false
}
```

**关键配置说明：**

| 配置项 | 说明 |
|---|---|
| `C_Cpp.intelliSenseEngine: "disabled"` | 禁用 Microsoft C/C++ 扩展的 IntelliSense，避免与 clangd 冲突 |
| `editor.defaultFormatter: "llvm-vs-code-extensions.vscode-clangd"` | 设置 clangd 为 C 文件的默认格式化工具 |
| `clangd.arguments` | 见下方参数详解 |
| `cmake.configureOnEdit: false` | 禁止在修改 CMakeLists.txt 时自动运行 CMake 配置，避免频繁触发 |

**clangd 参数详解：**

| 参数 | 说明 |
|---|---|
| `--clang-tidy` | 启用 clang-tidy 静态检查 |
| `--all-scopes-completion` | 补全时包含所有作用域的符号 |
| `--completion-style=detailed` | 提供详细的补全信息（类型、参数等） |
| `--header-insertion=iwyu` | 按 Include What You Use 原则自动插入头文件 |
| `--pch-storage=disk` | 预编译头文件存储在磁盘上以节省内存 |
| `--log=error` | 仅记录错误级别日志 |
| `--j=5` | 使用 5 个线程并行处理 |
| `--background-index` | 后台持续索引代码库 |
| `--compile-commands-dir=...` | 指定 compile_commands.json 的位置 |

**工作原理：** STM32CubeMX生成的顶层 CMakeLists.txt 中设置了 `set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)`，CMake 配置阶段会在 `build/Debug/` 下生成 `compile_commands.json`。clangd 读取此文件获得每个源文件的编译参数，从而提供精确的语法分析和补全。

---

#### b. `.vscode/launch.json` — openocd+cortex-debug(插件)的调试配置

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "openocd",
            "interface": "swd",
            "name": "CMSIS-DAP Debug",
            "executable": "${workspaceFolder}/build/Debug/ttsy-oscillator.elf",
            "device": "STM32F103RCTx",
            "configFiles": [
                "interface/cmsis-dap.cfg",
                "target/stm32f1x.cfg"
            ],
            "openOCDLaunchCommands": [
                "transport select swd",
                "adapter speed 1000",
                "reset_config none",
                "cortex_m reset_config sysresetreq"
            ],
            "postOpenOCDResetCommands": [
                "reset halt"
            ],
            "preLaunchTask": "Build Debug",
            "runToEntryPoint": "main",
            "svdFile": "D:/Coding/Keil5v41/packs/Keil/STM32F1xx_DFP/2.4.1/SVD/STM32F103xx.svd",
            "showDevDebugOutput": "raw",
            "gdbTarget": "localhost:50000",
            "postStartSessionCommands": [
                "shell sleep 2"
            ],
            "noDebug": false
        }
    ]
}
```

**关键配置说明：**

| 配置项 | 说明 |
|---|---|
| `servertype` | 使用 OpenOCD 作为 GDB 服务器 |
| `interface` | SWD 调试接口 |
| `configFiles` | OpenOCD 配置文件：CMSIS-DAP 接口 + STM32F1x 目标芯片 |
| `adapter speed` | 设置 SWD 时钟速率为 1000 kHz |
| `preLaunchTask` | 启动调试前自动执行 "Build Debug" 任务，确保固件最新 |
| `runToEntryPoint` | 启动后自动运行到 `main` 函数入口处暂停 |
| `svdFile` | SVD 文件路径，用于外设寄存器可视化查看 |
| `gdbTarget` | GDB 连接地址和端口（与 OpenOCD 默认 50000 端口一致） |

---

#### c. `.vscode/tasks.json` — 构建与烧录

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Debug",
            "type": "shell",
            "command": "cmake --build --preset Debug; if ($?) { arm-none-eabi-size -A -x ./build/Debug/ttsy-oscillator.elf }",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [],
            "detail": "Buid All"
        },
        {
            "label": "Rebuild Debug",
            "type": "shell",
            "command": "cmake --build --preset Debug --clean-first; if ($?) { arm-none-eabi-size -A -x ./build/Debug/ttsy-oscillator.elf }",
            "group": "build",
            "problemMatcher": [
                "$gcc"
            ],
            "detail": "Rebuild All"
        },
        {
            "label": "Flash",
            "type": "shell",
            "command": "openocd",
            "args": [
                "-f", "interface/cmsis-dap.cfg",
                "-f", "target/stm32f1x.cfg",
                "-c", "reset_config none",
                "-c", "cortex_m reset_config sysresetreq",
                "-c", "adapter speed 1000",
                "-c", "program ./build/Debug/ttsy-oscillator.elf verify reset exit"
            ],
            "dependsOn": [ "Build Debug" ],
            "group": {
                "kind": "test",
                "isDefault": true
            },
            "problemMatcher": [],
            "detail": "BuidAll->Flash->AutoReset"
        }
    ]
}
```

---

#### d. `.vscode/keybindings.json`

```json
[
    {
        "key": "ctrl+f7",
        "command": "workbench.action.tasks.runTask"
    },
    {
        "key": "f7",
        "command": "workbench.action.tasks.build",
        "when": "taskCommandsRegistered"
    },
    {
        "key": "f8",
        "command": "workbench.action.tasks.test"
    },
    {
        "key": "ctrl+f5",
        "command": "workbench.action.debug.start"
    }
]
```

---

#### e. `.clang-format`

```yaml
# .clang-format
Language: Cpp
BasedOnStyle: Google

# Indentation
IndentWidth: 4
TabWidth: 4
UseTab: Never
```

---

#### f. `.gitignore` — 版本控制忽略规则

```
!.vscode/
!ref/
build/
.cache/
```

---


### 1.3 `git push`问题

**问题：** `ssh: connect to host github.com port 22: Connection refused`

**解决方案：** 修改 SSH 配置，通过 443 端口连接。

1. 编辑 `C:\Users\<用户名>\.ssh\config`）

```
Host github.com
    Hostname ssh.github.com
    User git
    Port 443
    PreferredAuthentications publickey
    IdentityFile ./id_rsa
```

> `IdentityFile` 指向实际私钥路径。

2. 测试连接

```bash
ssh -T git@github.com
```

输入`yes`。
