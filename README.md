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

### g. 调试启动超时优化

**问题现象**  
首次按 `Ctrl+F5` 启动调试时，cortex-debug 插件弹出错误提示：

```
Failed to generate gdb commands: Error: Could not start gdb, no response from gdb
```

关闭错误窗口后再次启动调试，则能正常进入调试状态。

**原因分析**  
`arm-none-eabi-gdb` 加载包含完整调试信息的 ELF 文件（尤其是大工程）所需时间较长，而 cortex-debug 插件内部等待 GDB 响应的时间固定且不可配置，导致首次启动时超时。第二次启动时，操作系统已将 ELF 文件内容缓存到内存，加载速度显著提升，因此成功。

**解决方案**  
在正式调试前，通过 `preLaunchTask` 执行一次 GDB 预热任务，提前加载 ELF 符号表，使文件进入缓存，从而缩短正式启动时的加载时间。

**具体配置步骤**

1. 修改 `.vscode/launch.json`，将 `preLaunchTask` 指向组合任务：
   ```json
   "preLaunchTask": "PreLaunch with Warmup"
   ```

2. 在 `.vscode/tasks.json` 中新增以下两个任务（置于已有 `"Build Debug"` 任务之后）：

   ```json
   {
       "label": "Warmup GDB",
       "type": "shell",
       "command": "arm-none-eabi-gdb",
       "args": [
           "--batch",
           "-ex", "set verbose on",
           "-ex", "file ${workspaceFolder}/build/Debug/ttsy-oscillator.elf",
           "-ex", "quit"
       ],
       "presentation": {
           "echo": true,
           "reveal": "always",
           "focus": false,
           "panel": "shared",
           "clear": false
       },
       "problemMatcher": []
   },
   {
       "label": "PreLaunch with Warmup",
       "dependsOrder": "sequence",
       "dependsOn": ["Build Debug", "Warmup GDB"]
   }
   ```

   - `Warmup GDB` 任务以 `--batch` 模式运行 GDB，加载指定的 ELF 文件后立即退出；`set verbose on` 会输出符号加载过程的详细信息。  
   - `PreLaunch with Warmup` 组合任务按顺序执行构建和预热，确保在调试启动前完成所有准备工作。

**效果**  
预热任务执行时，终端面板会自动弹出并显示 GDB 加载符号的进度（例如 `Reading symbols from ...`）。预热完成后，正式调试启动时 GDB 因文件已在缓存中而快速响应，不再出现超时报错。

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

---

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

---

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

---

### 2.4 ST7789的注意点

ST7789要求先发RGB565数据的高字节, 再发低字节, 因此u16的颜色数据在强转为u8发送前, 要先人为
更改u16色彩的前后字节顺序.

---

### 2.5 队列传输指针与内容混淆导致 HardFault（UART 任务）

**现象描述**  
拨动 `KEY_UP` 按键后，系统立即进入 HardFault。通过调试器追踪，发现故障发生在 `uart_send` 函数调用 `strlen(pc_string)` 时，此时 `pc_string` 的值是一个非法地址（如 `0x5F79656B`），CPU 访问该地址触发总线错误。系统其他任务均停止响应。

---

**调试过程与关键观察**

1. **初步定位故障点**  
   在 `uart_send` 入口处设置断点，观察传入参数 `pc_string`。发现其值为 `0x5F79656B`，显然不是预期字符串 `"key_up_pressed\n"` 的地址（预期应为 `0x20000040` 或 Flash 中的地址）。这说明在调用 `uart_send` 之前，指针值已被破坏。

2. **回溯发送端数据流**  
   - 在 `key_up_callback` 中设置断点，观察调用 `uart_action(&key_up_msg[0])` 时传入的参数：  
     ```c
     char key_up_msg[] = "key_up_pressed\n";  // 全局数组
     ```  
     此时 `&key_up_msg[0]` 的值为 `0x20000040`，指向字符串首地址，数据正确。  
   - 进入 `uart_action` 函数，其实现为：  
     ```c
     void uart_action(const char* pc_msg) {
         xQueueSend(uart_que, pc_msg, portMAX_DELAY);
     }
     ```  
     观察形参 `pc_msg` 的值，仍为 `0x20000040`，数据未变。  
   - 继续跟踪进入 `xQueueGenericSend` 内部。在调用 `prvCopyDataToQueue` 前，观察 `pvItemToQueue` 参数，仍然为 `0x20000040`。

3. **检查队列复制操作**  
   `prvCopyDataToQueue` 内部执行关键复制操作：  
   ```c
   memcpy(pxQueue->pcWriteTo, pvItemToQueue, pxQueue->uxItemSize);
   ```  
   此时各参数为：  
   - `pvItemToQueue` = `0x20000040`（待复制数据的来源地址）  
   - `pxQueue->uxItemSize` = `0x4`（队列项大小为 4 字节）  
   - `pxQueue->pcWriteTo` = 队列内部存储区地址（如 `0x200014E8`）

   **此处为关键分歧点**：我们预期的行为是“将指针值 `0x20000040` 复制到队列”，但实际执行时，`memcpy` 从地址 `0x20000040` 处读取数据，而该地址正是字符串 `"key_up_pressed\n"` 的起始位置，因此复制了前 4 个字节：`'k'`、`'e'`、`'y'`、`'_'`，即十六进制 `0x6B 0x65 0x79 0x5F`。  
   在 `memcpy` 执行后查看队列存储区 `pxQueue->pcWriteTo` 的内容，确认为 `6B 65 79 5F`，验证了这一判断。

4. **追踪接收端数据恢复**  
   `uart_task` 中的接收逻辑为：  
   ```c
   char* pc_msg = NULL;
   if (xQueueReceive(uart_que, &pc_msg, portMAX_DELAY) == pdPASS) {
       uart_send(pc_msg);
   }
   ```  
   在 `xQueueReceive` 内部，关键操作为：  
   ```c
   memcpy(pvBuffer, pxQueue->u.xQueue.pcReadFrom, pxQueue->uxItemSize);
   ```  
   此时各参数为：  
   - `pvBuffer` = `&pc_msg`（指针变量本身的地址，如 `0x20000A6C`）  
   - `pxQueue->u.xQueue.pcReadFrom` = 队列读取位置（即之前写入的 `0x200014E8`）  
   - `pxQueue->uxItemSize` = `0x4`  

   执行 `memcpy` 后，将队列存储区中的 `6B 65 79 5F` 复制到 `pc_msg` 变量中。因此在 `xQueueReceive` 返回后，`pc_msg` 的值变为 `0x5F79656B`（小端序下 `6B 65 79 5F` 组成的 32 位整数）。随后调用 `uart_send(pc_msg)`，将该值作为地址传入 `strlen`，触发 HardFault。

5. **辅助观察：断点行为的说明**  
   在调试过程中，尝试在 `xQueueReceive` 内部设置断点时，调试器有时会显示异常或无法正常查看 `pc_msg` 的值。这是因为断点处 `memcpy` 刚执行完毕，`pc_msg` 已被赋值为 `0x5F79656B`，调试器在解析该地址时会尝试读取无效内存区域，导致变量显示异常，但这不影响程序运行逻辑，HardFault 发生在断点取消后继续执行到 `strlen` 时才触发。

---

**根本原因分析**  

问题根源在于对 FreeRTOS 队列 API 的语义理解偏差：

- `xQueueSend` 的第二个参数 `pvItemToQueue` 是一个**指向待复制数据的指针**，复制长度由 `uxItemSize` 决定。该函数会将 `pvItemToQueue` 所指向的内存区域中的内容复制到队列存储区，而非复制指针本身。  
- 原始代码中，`uart_action` 直接传入数组首地址 `&key_up_msg[0]`（即 `0x20000040`）。`xQueueSend` 将该地址视为数据来源，复制了该地址起始的 4 个字节（字符串内容），而非地址值本身。  
- 接收端期望从队列中恢复出一个有效的指针值，但实际拿到的是字符串内容片段，被错误地解释为指针地址，导致非法访问。

从类型系统角度看，`key_up_msg[]` 是一个数组，其名称退化为首地址，`&key_up_msg[0]` 在数值上等于首地址，但它指向的是字符数据而非指针变量。因此“取地址”操作实际获取的是“字符数据的地址”，而非“存储指针的变量的地址”。

---

**解决方案**  

将消息字符串声明为**全局指针变量**，使其拥有独立的存储空间来存放字符串地址，而非直接存储字符串内容：

```c
// 修改前（错误）
char key_up_msg[] = "key_up_pressed\n";

// 修改后（正确）
const char* key_up_msg = "key_up_pressed\n";
```

同时修改 `uart_action` 的接口，使其接受**指针的指针**，明确表达“传递指针变量地址”的意图：

```c
// .h 文件中
void uart_action(const char** pc_msg);

// .c 文件中
void uart_action(const char** pc_msg) {
    xQueueSend(uart_que, pc_msg, portMAX_DELAY);
}
```

调用处相应修改为：

```c
static void key_up_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        uart_action(&key_up_msg);  // 传入指针变量的地址
        buzzer_beep();
    }
}
```

---

**验证方法**  

修复后，在调试器中再次跟踪数据流：

1. 发送端：观察 `pvItemToQueue` 的值，此时应为 `&key_up_msg` 的地址（如 `0x20000050`，即指针变量本身的地址）。  
2. `memcpy` 执行后查看队列存储区 `pxQueue->pcWriteTo` 的内容，应为 `0x0800ABCD`（字符串常量在 Flash 中的地址），而非 ASCII 字符。  
3. 接收端：`xQueueReceive` 返回后，`pc_msg` 的值恢复为 `0x0800ABCD`，指向 Flash 中的字符串常量。  
4. `uart_send` 调用 `strlen(pc_string)` 时正常访问 Flash 区域，串口输出 `"key_up_pressed\n"`，HardFault 不再出现。

---

**经验总结与扩展**  

- 使用 FreeRTOS 队列传递指针数据时，**必须明确区分“传递指针值”与“传递指针指向的内容”**。若需要传递指针值（如字符串地址），应将指针变量的地址作为源数据传入，而非数组首地址。  
- 全局数组名并非变量，它没有独立的存储空间用于存储地址值，`&array` 在数值上等同于 `array`，但类型为数组指针，可能引起语义混淆。使用显式的指针变量可避免此类问题。  
- 调试此类数据传递问题时，**查看队列存储区的十六进制原始内容**是最直接有效的方法——它能清晰反映 `memcpy` 实际复制了什么，而非依赖变量窗口的字符串显示。  
- 对于其他按键（如 `KEY_1`、`KEY_DOWN` 等），若使用相同的 `uart_action` 模式，需同步修改为指针变量方案，保持一致性。  
- 该问题与硬件无关，纯属软件逻辑层面的指针/数组语义混淆，在嵌入式 RTOS 开发中具有一定典型性。

---

### 2.6 ADC 注入组中断仅触发一次的调试过程

**问题现象**

在实现数字万用表功能时，ADC1 的注入组配置为 TIM2 TRGO 硬件触发，中断回调 `HAL_ADCEx_InjectedConvCpltCallback` 始终无法进入。通过 GDB 调试发现断点完全不命中。

**初步排查：回调函数名误写为常规组版本（如 `HAL_ADC_ConvCpltCallback`），导致首次中断也无法进入。修正为 `HAL_ADCEx_InjectedConvCpltCallback` 后方可进入第一次中断。**

进入第一次中断后，继续运行时发现 TIM2 后续的 TRGO 信号无法再次触发中断。暂停程序，通过 GDB 观察 ADC 状态寄存器：

```gdb
p/x ADC1->CR1
```

发现 `CR1` 寄存器的值为 `0x00000000`，即 `JEOCIE`（注入组转换结束中断使能位）为 0，导致后续中断被硬件屏蔽。

---

**调试过程**

**第一步：定位中断使能位被清零**

在 `HAL_ADCEx_InjectedConvCpltCallback` 入口处设置断点，首次进入时观察 `ADC1->CR1`：

```gdb
p/x ADC1->CR1
# 输出: 0x00000020  (JEOCIE 位为 1，中断使能正常)
```

让程序继续运行，等待 TIM2 的下一次触发，但断点不再命中。暂停程序再次查看：

```gdb
p/x ADC1->CR1
# 输出: 0x00000000  (JEOCIE 位被清零)
```

**结论**：`JEOCIE` 在第一次中断处理过程中被某处代码主动清除。

---

**第二步：追踪 HAL 库源码**

中断向量入口为 `ADC1_2_IRQHandler`，其调用链为：

```
ADC1_2_IRQHandler()
  └── HAL_ADC_IRQHandler(&hadc1)
        ├── 处理注入组转换完成标志 (JEOC)
        ├── 条件判断是否禁用 JEOCIE
        │   └── if (...) { __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC); }
        └── HAL_ADCEx_InjectedConvCpltCallback(hadc)
```

关键代码位于 `stm32f1xx_hal_adc.c` 中（约 1862 行）：

```c
/* Determine whether any further conversion upcoming on group injected */
if (ADC_IS_SOFTWARE_START_INJECTED(hadc)                     ||
    (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO) &&
     (ADC_IS_SOFTWARE_START_REGULAR(hadc)        &&
      (hadc->Init.ContinuousConvMode == DISABLE)   )        )   )
{
    /* Disable ADC end of conversion interrupt on group injected */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);

    /* Set ADC state */
    CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);

    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
    {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
    }
}
```

---

**第三步：分析条件判断逻辑**

需要理解两个关键宏的定义：

| 宏 | 含义 | 判定条件 |
|---|---|---|
| `ADC_IS_SOFTWARE_START_INJECTED(hadc)` | 注入组是否为软件触发 | `CR2` 的 `JEXTTRIG` 位为 0 |
| `ADC_IS_SOFTWARE_START_REGULAR(hadc)` | 常规组是否为软件触发 | `CR2` 的 `EXTTRIG` 位为 0 |

**用户的配置**（`CubeMX` 中常规组保持默认，仅配置注入组）：

| 条件项 | 实际值 | 结果 |
|---|---|---|
| `ADC_IS_SOFTWARE_START_INJECTED` | TIM2 TRGO 触发，`JEXTTRIG=1` | **FALSE** |
| `HAL_IS_BIT_CLR(CR1, ADC_CR1_JAUTO)` | 未开启自动注入，`JAUTO=0` | **TRUE** |
| `ADC_IS_SOFTWARE_START_REGULAR` | 常规组默认软件触发，`EXTTRIG=0` | **TRUE** |
| `ContinuousConvMode == DISABLE` | 常规组非连续模式 | **TRUE** |

代入判断表达式：

```c
if (FALSE || (TRUE && (TRUE && TRUE)))
=> if (FALSE || TRUE)
=> if (TRUE)   // 条件成立，执行清除 JEOCIE
```

**结论**：HAL 库判定当前为“单次软件触发模式”，转换完成后主动关闭注入组中断。

---

**第四步：对比教程配置的差异**

教程在第 5 步的注释中明确指出：

> **由于 CubeMX 本身存在 BUG，在单独使用注入序列的时候常规序列不得选择软件触发。**

教程的做法是将常规组的触发方式从默认的 `Software` 改为**外部触发**（如 Timer 3 TRGO），或开启 `Continuous Conversion Mode`。此时：

| 条件项 | 教程配置后的值 | 结果 |
|---|---|---|
| `ADC_IS_SOFTWARE_START_INJECTED` | `JEXTTRIG=1` | **FALSE** |
| `HAL_IS_BIT_CLR(CR1, ADC_CR1_JAUTO)` | `JAUTO=0` | **TRUE** |
| `ADC_IS_SOFTWARE_START_REGULAR` | `EXTTRIG=1`（硬件触发） | **FALSE** |
| `ContinuousConvMode == DISABLE` | 无关（短路求值） | — |

代入表达式：

```c
if (FALSE || (TRUE && (FALSE && ...)))
=> if (FALSE || FALSE)
=> if (FALSE)   // 条件不成立，JEOCIE 被保留
```

**结论**：教程通过“欺骗”HAL 库的条件判断，使注入组中断得以持续触发。这是一种依赖 HAL 库内部实现细节的“技巧”，而非标准用法。

---

**第五步：理解注入组的设计意图**

STM32 参考手册（RM0008）指出，注入组（Injected Group）区别于常规组（Regular Group）的核心特性：

- 注入组**没有**像常规组那样的连续转换模式（`CONT` 位）。
- 每次注入转换完成后，ADC 硬件自动进入空闲状态，**必须通过软件或下一次硬件触发才能启动新一轮转换**。
- 注入组被设计用于“高优先级突发采样”，而非连续数据流。

因此，**正确的做法应当是在每次转换完成后，由软件重新“武装”ADC，使其等待下一次触发**。

---

**第六步：最终修复方案**

在 `HAL_ADCEx_InjectedConvCpltCallback` 中，读取数据后、发送到队列之前，显式调用 `HAL_ADCEx_InjectedStart_IT` 重新使能中断并进入等待触发状态：

```c
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (hadc->Instance == ADC1) {
        // 1. 读取 ADC 转换值
        float readings = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1)
                         / 4095.0f * 3.3f;

        // 2. 计算万用表测量结果
        dmm_handle_t handle = {0};
        dmm_get_range(&handle.range);
        dmm_calc_data(&handle, readings);

        // 3. 【修复点】重新武装 ADC，等待下一次 TIM2 TRGO 触发
        HAL_ADCEx_InjectedStart_IT(&hadc1);

        // 4. 将结果通过邮箱发送给其他任务
        xQueueOverwriteFromISR(dmm_mail, &handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
```

`HAL_ADCEx_InjectedStart_IT` 内部会重新置位 `CR1` 的 `JEOCIE` 位，使 ADC 再次响应后续的硬件触发信号。

---

**根本原因总结**

1. **函数名误写**：首次回调函数名写为常规组版本 `HAL_ADC_ConvCpltCallback`，导致中断无法进入。修正为 `HAL_ADCEx_InjectedConvCpltCallback` 后恢复正常。

2. **HAL 库的设计预设**：HAL 库假定注入组仅用于单次突发测量，因此在转换完成后自动禁用中断（`JEOCIE` 清零）。这与“定时器周期性触发注入组”的使用场景冲突。

3. **标准解决方案**：在中断回调末尾显式调用 `HAL_ADCEx_InjectedStart_IT`，由软件重新武装 ADC 以等待下一次硬件触发。这是符合 STM32 参考手册的标准做法。

---

**经验教训**

- **使用 HAL 库时注意回调函数命名规范**：
  - 常规组回调：`HAL_ADC_ConvCpltCallback`
  - 注入组回调：`HAL_ADCEx_InjectedConvCpltCallback`
  
- **HAL 库的“Bug”本质是设计预设偏差**：HAL 库的行为基于特定使用场景假设，当实际需求偏离该假设时，需要理解其内部逻辑并采取标准应对方案。

- **硬件手册优先级高于 HAL 库实现**：当 HAL 库行为与参考手册描述不一致时，应优先遵循手册规定的硬件操作方式。

- **选择标准做法而非“技巧”**：教程的“修改常规组配置”方案依赖于 HAL 库的内部实现细节（`EXTTRIG` 位的判定），更换 HAL 库版本或芯片型号后可能失效。在中断回调中显式调用 `Start_IT` 是符合硬件手册的标准做法，具有更好的可移植性和可维护性。

---

**验证方法**

修复后，在 GDB 中持续观察：

```gdb
# 设置观察点监控 CR1 寄存器的变化
watch *(uint32_t*)0x40012404

# 或定时打印 CR1 值
p/x ADC1->CR1
```

`JEOCIE` 位（第 5 位）将始终保持为 1，TIM2 每次溢出时均能正常进入中断回调。万用表数值将以 10Hz 频率稳定更新。

---

