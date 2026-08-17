# AIGC
# -*- coding: utf-8 -*-
import sys
import os
import re
from PIL import Image

# ========== 配置选项 ==========
OUTPUT_UINT16 = True          # True: uint16_t 数组；False: uint8_t 字节数组
SWAP_RB = False               # 红蓝交换(一般不需要)
SCREEN_WIDTH  = 240           # 目标屏幕宽度(用于居中计算)
SCREEN_HEIGHT = 320           # 目标屏幕高度(用于居中计算)
# ==============================

def rgb565_value(r, g, b):
    """计算 RGB565 原始值(未交换字节)"""
    if SWAP_RB:
        return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3)
    else:
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def generate_header(input_file, var_name=None, output_file=None):
    if not os.path.exists(input_file):
        print(f"Error: {input_file} not found")
        return

    img = Image.open(input_file).convert('RGB')
    width, height = img.size

    # ★ 修改：按 C# 顺序(列优先，行从下到上)
    rgb565_pixels = []
    for x in range(width):
        for y in range(height - 1, -1, -1):
            r, g, b = img.getpixel((x, y))
            raw = rgb565_value(r, g, b)
            swapped = ((raw & 0xFF) << 8) | ((raw >> 8) & 0xFF)
            rgb565_pixels.append(swapped)

    # 变量名：若未指定则使用文件名(替换非法字符)
    if var_name is None:
        base = os.path.splitext(os.path.basename(input_file))[0]
        var_name = re.sub(r'\W|^(?=\d)', '_', base)

    macro_prefix = var_name.upper()
    array_name = var_name

    # ---------- 屏幕居中坐标 ----------
    col_start = (SCREEN_WIDTH  - width)  // 2
    col_end   = col_start + width - 1
    row_start = (SCREEN_HEIGHT - height) // 2
    row_end   = row_start + height - 1

    # 拆成高/低字节(用于 4 字节数组)
    col_arr = [col_start >> 8, col_start & 0xFF,
               col_end   >> 8, col_end   & 0xFF]
    row_arr = [row_start >> 8, row_start & 0xFF,
               row_end   >> 8, row_end   & 0xFF]

    # ---------- 生成头文件 ----------
    content  = f"#ifndef _{macro_prefix}_H_\n"
    content += f"#define _{macro_prefix}_H_\n\n"
    content += "#include <stdint.h>\n\n"


    # # 基本尺寸宏
    # content += f"#define {macro_prefix}_WIDTH  {width}\n"
    # content += f"#define {macro_prefix}_HEIGHT {height}\n"
    # content += f"#define {macro_prefix}_SIZE   {width * height * 2}\n\n"

    # # 坐标宏
    # content += f"#define {macro_prefix}_COL_START  {col_start}\n"
    # content += f"#define {macro_prefix}_COL_END    {col_end}\n"
    # content += f"#define {macro_prefix}_ROW_START  {row_start}\n"
    # content += f"#define {macro_prefix}_ROW_END    {row_end}\n\n"

    # 4 字节数组宏
    content += "/* 4‑byte arrays for lcd_send_data(..., 4) */\n"
    content += f"#define {macro_prefix}_COL_ARRAY  {{" + ", ".join(f"0x{x:02X}" for x in col_arr) + "}\n"
    content += f"#define {macro_prefix}_ROW_ARRAY  {{" + ", ".join(f"0x{x:02X}" for x in row_arr) + "}\n\n"

    # ---------- 图像数据数组 ----------
    if OUTPUT_UINT16:
        content += f"const uint16_t {array_name}[{width} * {height}] = {{"
        for i, val in enumerate(rgb565_pixels):
            if i % 8 == 0:
                content += "\n    "
            content += f"0x{val:04X}"
            if i < len(rgb565_pixels) - 1:
                content += ", "
        content += "\n};\n"
    else:
        size = width * height * 2
        content += f"const uint8_t {array_name}[{size}] = {{"
        for i, val in enumerate(rgb565_pixels):
            if i % 4 == 0:
                content += "\n    "
            content += f"0x{(val >> 8) & 0xFF:02X}, 0x{val & 0xFF:02X}"
            if i < len(rgb565_pixels) - 1:
                content += ", "
        content += "\n};\n"

    content += f"\n#endif /* _{macro_prefix}_H_ */\n"

    if output_file is None:
        output_file = f"{array_name}.h"

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f"✅ 已生成: {output_file} (宽={width}, 高={height}, 像素数={width*height})")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python png2header.py <图片文件> [变量名] [输出头文件]")
        print("  变量名缺省时使用文件名(无扩展名)")
        sys.exit(1)
    infile = sys.argv[1]
    var = sys.argv[2] if len(sys.argv) > 2 else None
    outfile = sys.argv[3] if len(sys.argv) > 3 else None
    generate_header(infile, var, outfile)