import json
import sys
import os
import re

def remove_comments(json_str):
    pattern = r'(//[^\n]*)|(/\*[\s\S]*?\*/)'
    return re.sub(pattern, '', json_str)

def generate_header(json_path, header_path):
    try:
        with open(json_path, 'r') as f:
            raw_content = f.read()

        # Clean comments before parsing
        clean_content = remove_comments(raw_content)
        data = json.loads(clean_content)
    except Exception as e:
        print(f"Error reading JSON: {e}")
        sys.exit(1)

    # --- Extract Data ---
    matrix = data.get("matrix", {})
    row_pins = matrix.get("row_pins", [])
    col_pins = matrix.get("col_pins", [])
    direction = matrix.get("diode_direction", "COL2ROW")

    # meta/usb sections
    meta = data.get("meta", {})
    usb  = data.get("usb", {})

    # Fallback logic: Try 'usb' section first, then 'meta', then defaults
    manufacturer = usb.get("manufacturer", meta.get("manufacturer", "Woyta Factory"))
    product = usb.get("product", meta.get("name", "Woyta Pad"))

    vid = usb.get("vid", "0xCAFE")
    pid = usb.get("pid", "0x4242")
    bcd_device = usb.get("device_ver", "0x0100")

    # --- Generate Content ---
    content = [
        "// AUTO-GENERATED FILE. DO NOT EDIT.",
        "#pragma once",
        "",
        "// Hardware Definitions",
        f"#define MATRIX_ROWS {len(row_pins)}",
        f"#define MATRIX_COLS {len(col_pins)}",
        "",
        f"#define MATRIX_ROW_PINS {{ {', '.join(map(str, row_pins))} }}",
        f"#define MATRIX_COL_PINS {{ {', '.join(map(str, col_pins))} }}",
        "",
        f"#define DIODE_DIRECTION_{direction}",
        "",
        "// USB Identity",
        # Note: We manually add quotes \" around strings for C
        f'#define USB_MANUFACTURER "{manufacturer}"',
        f'#define USB_PRODUCT      "{product}"',
        f'#define USB_VID          {vid}',
        f'#define USB_PID          {pid}',
        f'#define USB_BCD          {bcd_device}',
        ""
    ]

    with open(header_path, 'w') as f:
        f.write('\n'.join(content))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python generate_config.py <input.json> <output.h>")
        sys.exit(1)

    generate_header(sys.argv[1], sys.argv[2])