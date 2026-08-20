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

**关键配置说明: **

| 配置项 | 说明 |
|---|---|
| `C_Cpp.intelliSenseEngine: "disabled"` | 禁用 Microsoft C/C++ 扩展的 IntelliSense, 避免与 clangd 冲突 |
| `editor.defaultFormatter: "llvm-vs-code-extensions.vscode-clangd"` | 设置 clangd 为 C 文件的默认格式化工具 |
| `clangd.arguments` | 见下方参数详解 |
| `cmake.configureOnEdit: false` | 禁止在修改 CMakeLists.txt 时自动运行 CMake 配置, 避免频繁触发 |

**clangd 参数详解: **

| 参数 | 说明 |
|---|---|
| `--clang-tidy` | 启用 clang-tidy 静态检查 |
| `--all-scopes-completion` | 补全时包含所有作用域的符号 |
| `--completion-style=detailed` | 提供详细的补全信息(类型、参数等) |
| `--header-insertion=iwyu` | 按 Include What You Use 原则自动插入头文件 |
| `--pch-storage=disk` | 预编译头文件存储在磁盘上以节省内存 |
| `--log=error` | 仅记录错误级别日志 |
| `--j=5` | 使用 5 个线程并行处理 |
| `--background-index` | 后台持续索引代码库 |
| `--compile-commands-dir=...` | 指定 compile_commands.json 的位置 |

**工作原理: ** STM32CubeMX生成的顶层 CMakeLists.txt 中设置了 `set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)`, CMake 配置阶段会在 `build/Debug/` 下生成 `compile_commands.json`. clangd 读取此文件获得每个源文件的编译参数, 从而提供精确的语法分析和补全. 

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

**关键配置说明: **

| 配置项 | 说明 |
|---|---|
| `servertype` | 使用 OpenOCD 作为 GDB 服务器 |
| `interface` | SWD 调试接口 |
| `configFiles` | OpenOCD 配置文件: CMSIS-DAP 接口 + STM32F1x 目标芯片 |
| `adapter speed` | 设置 SWD 时钟速率为 1000 kHz |
| `preLaunchTask` | 启动调试前自动执行 "Build Debug" 任务, 确保固件最新 |
| `runToEntryPoint` | 启动后自动运行到 `main` 函数入口处暂停 |
| `svdFile` | SVD 文件路径, 用于外设寄存器可视化查看 |
| `gdbTarget` | GDB 连接地址和端口(与 OpenOCD 默认 50000 端口一致) |

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

**问题: ** `ssh: connect to host github.com port 22: Connection refused`

**解决方案: ** 修改 SSH 配置, 通过 443 端口连接. 

1. 编辑 `C:\Users\<用户名>\.ssh\config`)

```
Host github.com
    Hostname ssh.github.com
    User git
    Port 443
    PreferredAuthentications publickey
    IdentityFile ./id_rsa
```

> `IdentityFile` 指向实际私钥路径. 

2. 测试连接

```bash
ssh -T git@github.com
```

输入`yes`. 

---

## 2. 调试与问题排查

### 2.1 典型问题: FreeRTOS 任务卡死在 `vTaskDelay` 中

**现象: **  
程序在 `lcd_reset()` 中执行第二个 `vTaskDelay(pdMS_TO_TICKS(125))` 后无响应, 程序在经过: 

```c
else
	{
		/* *** NOTE ***********************************************************
		If you find your application is crashing here then likely causes are
		listed below.  In addition see https://www.freertos.org/FAQHelp.html for
		more tips, and ensure configASSERT() is defined!
		https://www.freertos.org/a00110.html#configASSERT

			1) Stack overflow -
			   see https://www.freertos.org/Stacks-and-stack-overflow-checking.html
			2) Incorrect interrupt priority assignment, especially on Cortex-M
			   parts where numerically high priority values denote low actual
			   interrupt priorities, which can seem counter intuitive.  See
			   https://www.freertos.org/RTOS-Cortex-M3-M4.html and the definition
			   of configMAX_SYSCALL_INTERRUPT_PRIORITY on
			   https://www.freertos.org/a00110.html
			3) Calling an API function from within a critical section or when
			   the scheduler is suspended, or calling an API function that does
			   not end in "FromISR" from an interrupt.
			4) Using a queue or semaphore before it has been initialised or
			   before the scheduler has been started (are interrupts firing
			   before vTaskStartScheduler() has been called?).
		**********************************************************************/

		for( pxIterator = ( ListItem_t * ) &( pxList->xListEnd ); pxIterator->pxNext->xItemValue <= xValueOfInsertion; pxIterator = pxIterator->pxNext ) /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. *//*lint !e440 The iterator moves to a different value, not xValueOfInsertion. */
		{
			/* There is nothing to do here, just iterating to the wanted
			insertion position. */
		}
	}
```

之后卡在: 

```c
/* Neither the stack nor the TCB were allocated dynamically, so
				nothing needs to be freed. */
				configASSERT( pxTCB->ucStaticallyAllocated == tskSTATICALLY_ALLOCATED_STACK_AND_TCB	);
				mtCOVERAGE_TEST_MARKER();
```

处, 不返回, 系统其他任务也停止运行. 

如同第一段程序给出的提示, 需要检查任务栈大小, 而其中最小的栈是buzzer_task 32Word, 将该值调整为64Word, 程序恢复正常. 

问题分析: 

**堆栈溢出**导致内存污染, 可能破坏系统关键数据(如 `xTickCount`). `buzzer_task` 的栈大小仅为 `32`(在 ARM 上通常指 32 个 **words**, 即 128 字节). 该任务调用了 `buzzer_beep_once()` → `vTaskDelay()`, 其中 `vTaskDelay` 需保存大量上下文(寄存器、临时变量), 加上 HAL 库 `HAL_GPIO_WritePin` 的调用, 栈空间远远不足.   
**栈溢出后破坏了系统节拍计数器 `xTickCount` 或任务控制块, 导致 `vTaskDelay` 永远无法超时, 系统卡死. **

FreeRTOS提供的诊断工具: `configCHECK_FOR_STACK_OVERFLOW`

在 `FreeRTOSConfig.h` 中设置该宏: 

| 值 | 含义 | 适用场景 |
| :---: | :--- | :--- |
| 0 | 禁用检测(默认) | 最终发布产品时可关闭以节省开销 |
| 1 | 方法一: 栈指针检查, 速度快, 但只能检测严重越界 | 对性能要求高时的基本检测 |
| 2 | 方法二: 栈填充标记检查(最严格), 在创建任务时用特定值填充整个栈, 任务切换时从栈底向上检查覆盖情况 | **强烈推荐在开发调试阶段使用** |

**注意: ** 当该宏设为 1 或 2 时, **必须**实现钩子函数: 

```c
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // 一旦触发溢出, 可在此记录日志、点亮 LED, 或死循环等待调试器捕获
    while(1);
}
```

**本例中的具体使用与分析: **

复现 `general_include.h` 中的任务创建代码: 

```c
xTaskCreate(buzzer_task, "buzzer_task", 32, NULL, 1, NULL);
xTaskCreate(lcd_task,   "lcd_task",   256, NULL, 1, NULL);
xTaskCreate(key_task,   "key_task",   128, NULL, 3, NULL);
```

写hook:

```c
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    int i = 0;
    while(1){
        i=-i;
    };
}
```

现象: 

不设置任何断点, 程序运行一段时间之后直接跳转到hook中.

**解决方案: **

1. **增大 `buzzer_task` 栈尺寸**, 至少 64Word: 
   ```c
   xTaskCreate(buzzer_task, "buzzer_task", 64, NULL, 1, NULL);
   ```
   同时检查其他任务, `key_task` 也建议增至 256. 

2. **开启堆栈溢出检测**(调试期): 
   ```c
   #define configCHECK_FOR_STACK_OVERFLOW 2
   ```
   并实现钩子函数, 以便及时捕获溢出. 

3. **利用高水位标记验证栈余量**: (未使用)
   在任务中调用 `uxTaskGetStackHighWaterMark(NULL)` 可获取剩余最小栈空间(单位: words), 确保该值远大于 0. 可将结果通过串口输出或 LED 提示. 

**经验总结: **

- 在 FreeRTOS 中, `vTaskDelay` 卡死最常见的原因是 **SysTick 未正确配置** 或 **任务栈溢出**. 
- 优先检查栈大小是否合理, 尤其对于调用库函数(如 HAL 驱动)的任务, 至少应分配 256 字. 
- 开启 `configCHECK_FOR_STACK_OVERFLOW = 2` 是调试期最有效的手段, 可快速定位栈溢出问题. 
- 生产环境中可关闭检测, 但务必保证各任务栈通过高水位测试留有足够余量. 

### 2.2 LCD 显示异常：绘制缓慢、逐列扫描、旧图像残留

**现象描述**  
- 执行 `lcd_show_image()` 后，LCD 并非瞬间整屏刷新，而是肉眼可见地从屏幕左下方开始，逐列向右上方缓慢扫描出现色块。  
- 仅复位单片机（不断开 LCD 电源）后重新运行程序，旧图像依然保留在屏幕上，新绘制的色块仅覆盖了部分区域。

---

**实际定位的问题**  

1. **`lcd_send_data` 发送字节数错误**  
   代码中调用 `HAL_SPI_Transmit` 的第三个参数误写为 `sizeof(size)`，导致无论 `size` 有多大，实际只发送 2 个字节。  
   - 影响：每次仅向 ST7789 写入 2 个像素（16 位/像素 × 2 字节 = 1 个像素），后续大量数据未发送，LCD 因接收数据不足而长时间处于等待状态，表现为缓慢且断续的逐列扫描式刷新。  
   - 修正：改为 `size`。  

2. **窗口范围仅覆盖屏幕中央区域**  
   代码中通过 `0x2A`（列地址）设置范围为 60~179（宽 120），`0x2B`（行地址）设置范围为 80~239（高 160），仅占全屏（假定 240×320）的一小部分。  
   - 影响：写入的数据只更新该窗口内的像素，其余区域（屏幕边缘）的显存内容未被修改，保留上一次的显示内容。仅复位单片机但是 LCD 不断电，其 GRAM 中旧数据依然存在，因此旧图像残留。  
   - 修正：若需全屏刷新，应将窗口设为 0~239（列）和 0~319（行），并发送对应尺寸的数据。

3. **复位后 LCD GRAM 内容不会自动清除**  (待验证)
   ST7789 的硬件复位（拉低 RESET 引脚）仅复位寄存器状态，**不会清零显存（GRAM）**。单片机复位后，如果 `lcd_rst` 引脚电平未能产生有效的低电平脉冲（或复位瞬间 GPIO 状态不定），LCD 可能未经历复位，GRAM 内容保持不变。即便成功复位，旧数据仍然驻留，必须通过写入新数据覆盖。

---

**其他可能导致或加重该现象的因素**  

- **SPI 时钟频率偏低**：即使修复了发送长度，若 SPI 波特率较低（如 ≤ 1 MHz），38400 字节的传输仍会耗时数百毫秒，人眼仍可察觉缓慢刷新。  
- **阻塞式发送且未利用中断/DMA**：`HAL_SPI_Transmit` 为阻塞传输，传输期间 CPU 空转，无法执行其他任务，影响系统实时性，但不会直接导致逐列扫描现象（该现象主要由数据不足引起）。  
- **窗口设置与数据尺寸不匹配**：若窗口宽高乘积与 `lightPurpleBlock` 的总像素数（160×120）一致，则无误；若误设其他尺寸（如宽高颠倒），则写入的数据量可能超出或不足窗口大小，导致显示错位或异常残留。  
- **LCD 初始化时序不足**：若 `lcd_reset` 后的延时（125 ms）或退出睡眠模式（0x11）后的延时不足，可能导致 LCD 未完全准备就绪，后续指令执行不稳定，偶发数据写入失败或部分刷新。  
- **多任务调度影响**：在 FreeRTOS 环境下，若 `lcd_show_image` 所在任务优先级较低，可能在发送数据过程中被高优先级任务抢占，中断 SPI 传输，导致数据分片，进一步加剧“逐列慢扫”现象。

---

**诊断与修复建议**（摘要）  

| 检查项 | 预期操作 |
|--------|----------|
| `lcd_send_data` 发送长度 | 将 `sizeof(size)` 改为 `size` |
| 窗口地址设定 | 确认是否覆盖目标区域，若需全屏则设置为 0~(width-1) 和 0~(height-1) |
| 清屏或全屏覆盖 | 在显示新内容前，先发送一次全屏黑色数据或调整窗口为全屏再发送 |
| SPI 时钟提高 | 检查 `hspi1.Init.BaudRatePrescaler`，在硬件允许范围内降低分频系数 |
| 任务栈与优先级 | 确保 LCD 任务栈足够（建议 ≥ 512 words），并可临时提升优先级以完成连续发送 |
| 复位时序 | 使用逻辑仪或示波器确认 `lcd_rst` 引脚在复位时产生 ≥ 1 ms 的低电平脉冲 |

### 2.3 编译报错`ceil`相关内容

```bash
.../User/Drivers/GUI -lm  -llcd && cd ."
.../arm-none-eabi/bin/ld.exe: .../User/Drivers/GUI\liblcd.a(lcd.o): in function `draw_char':
lcd.c:(.text.draw_char+0x16e): undefined reference to `ceil'
.../arm-none-eabi/15.2.1/../../../../arm-none-eabi/bin/ld.exe: (ceil): Unknown destination type (ARM/Thumb) in .../User/Drivers/GUI\liblcd.a(lcd.o)
lcd.c:(.text.draw_char+0x16e): dangerous relocation: unsupported relocation
Memory region         Used Size  Region Size  %age Used
             RAM:       19136 B        48 KB     38.93%
           FLASH:       47108 B       256 KB     17.97%
collect2.exe: error: ld returned 1 exit status
ninja: build stopped: subcommand failed.
```

原因分析: 

liblcd.a中使用了libm中的`ceil`, 但是包含`ceil`的libm先于liblcd解析: `-lm  -llcd`, 因此`ceil`标志被忽略, 导致报错.

修改方案: 

将`CMakeLists.text`中引用link库的部分添加`m`, 强制规定解析顺序即可: 

```cmake
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    lcd
    m
    # OneMessage
)
```

### 2.4 ST7789的注意点

ST7789要求先发RGB565数据的高字节, 再发低字节, 因此u16的颜色数据在强转为u8发送前, 要先人为
更改u16色彩的前后字节顺序.

