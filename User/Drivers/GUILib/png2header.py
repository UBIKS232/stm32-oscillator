# AIGC
# -*- coding: utf-8 -*-

import sys
import os
import re
from PIL import Image

# ---------- 输出格式选择 ----------
# True: 生成 uint16_t 数组（与示例相同，推荐）
# False: 生成 uint8_t 字节数组
OUTPUT_UINT16 = True

# 颜色通道交换（一般不需要，仅当红蓝反色时尝试）
SWAP_RB = False
# --------------------------------

def rgb565_value(r, g, b):
    if SWAP_RB:
        return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3)
    else:
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def generate_header(input_file, output_file=None):
    if not os.path.exists(input_file):
        print(f"Error: {input_file} not found")
        return

    img = Image.open(input_file).convert('RGB')
    width, height = img.size

    # 自下而上扫描
    rgb565_pixels = []
    for y in range(height - 1, -1, -1):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb565_pixels.append(rgb565_value(r, g, b))

    # 居中坐标（屏幕 320×240）
    col_start = (240 - width) // 2
    col_end   = col_start + width - 1
    row_start = (320 - height) // 2
    row_end   = row_start + height - 1

    col_arr = [col_start >> 8, col_start & 0xFF, col_end >> 8, col_end & 0xFF]
    row_arr = [row_start >> 8, row_start & 0xFF, row_end >> 8, row_end & 0xFF]

    base = os.path.splitext(os.path.basename(input_file))[0]
    array_name = re.sub(r'\W|^(?=\d)', '_', base).upper()
    size = width * height * 2

    # 构造头文件头部（避免 format 冲突，全部用拼接）
    content = "#ifndef __" + array_name + "_H__\n"
    content += "#define __" + array_name + "_H__\n\n"
    content += "#include <stdint.h>\n\n"
    content += "#define " + array_name + "_WIDTH  " + str(width) + "\n"
    content += "#define " + array_name + "_HEIGHT " + str(height) + "\n"
    content += "#define " + array_name + "_SIZE   " + str(size) + "\n\n"
    content += "#define " + array_name + "_COL_START  " + str(col_start) + "\n"
    content += "#define " + array_name + "_COL_END    " + str(col_end) + "\n"
    content += "#define " + array_name + "_ROW_START  " + str(row_start) + "\n"
    content += "#define " + array_name + "_ROW_END    " + str(row_end) + "\n\n"
    content += "/* 4‑byte arrays for lcd_send_data(..., 4) */\n"
    content += "#define " + array_name + "_COL_ARRAY  {" + ", ".join(f"0x{x:02X}" for x in col_arr) + "}\n"
    content += "#define " + array_name + "_ROW_ARRAY  {" + ", ".join(f"0x{x:02X}" for x in row_arr) + "}\n\n"

    if OUTPUT_UINT16:
        content += "static const uint16_t " + array_name.lower() + "[" + str(width * height) + "] = {\n"
        for i, val in enumerate(rgb565_pixels):
            if i % 8 == 0:
                content += "\n    "
            content += f"0x{val:04X}, "
        content = content.rstrip(", ") + "\n};\n"
    else:
        content += "static const uint8_t " + array_name.lower() + "[" + str(size) + "] = {\n"
        for val in rgb565_pixels:
            # 大端（高字节在前），如需小端可交换下面两行顺序
            content += f"0x{(val >> 8) & 0xFF:02X}, 0x{val & 0xFF:02X}, "
        content = content.rstrip(", ") + "\n};\n"

    content += "\n#endif /* __" + array_name + "_H__ */\n"

    if output_file is None:
        output_file = f"{array_name.lower()}.h"

    with open(output_file, 'w') as f:
        f.write(content)
    print(f"✅ 已生成: {output_file} (宽={width}, 高={height}, 像素数={width*height})")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python png2header.py <图片文件> [输出头文件]")
        sys.exit(1)
    infile = sys.argv[1]
    outfile = sys.argv[2] if len(sys.argv) > 2 else None
    generate_header(infile, outfile)