import os
from PIL import Image, ImageFont, ImageDraw

def font_to_glut_18x30_c_header(font_path=None, font_size=24, output_filename="glut_18x30_font.h"):
    """
    Renders printable ASCII characters from a TTF/OTF font into an 18x30 grid
    (ideal for 2x Retina displays), packs the bits bottom-to-top into 3 bytes 
    per row, and outputs a C header file.
    """
    width = 18
    height = 30
    
    # Load font
    if font_path and os.path.exists(font_path):
        try:
            # For 18x30 canvas, a font_size between 20 and 24 usually scales perfectly
            font = ImageFont.truetype(font_path, font_size)
            print(font);
        except IOError:
            print(f"Could not load font from {font_path}. Using fallback default.")
            font = ImageFont.load_default()
    else:
        print("Using Pillow's default internal bitmap font.")
        font = ImageFont.load_default()

    c_code = []
    c_code.append("// Auto-generated Retina-ready (18x30) glBitmap replacement font data")
    c_code.append("#ifndef GLUT_18X30_FONT_H")
    c_code.append("#define GLUT_18X30_FONT_H\n")
    c_code.append("typedef struct {")
    c_code.append("    unsigned char bitmap[30 * 3]; // 30 rows * 3 bytes per row")
    c_code.append("} GlutChar18x30;\n")
    c_code.append("// Matrix of ASCII characters 32 (Space) through 126 (~)")
    c_code.append("static const GlutChar18x30 Font18x30[95] = {")

    for ascii_val in range(32, 127):
        char = chr(ascii_val)
        
        # Create a 1-bit monochrome image canvas
        img = Image.new('1', (width, height), color=0)
        draw = ImageDraw.Draw(img)
        
        # Render character (adjust vertical offset if font cuts off)
        draw.text((0, 0), char, font=font, fill=1)
        
        bytes_list = []
        
        # Unpack from the BOTTOM row up to the TOP row
        for row in reversed(range(height)):
            row_byte1 = 0
            row_byte2 = 0
            row_byte3 = 0
            
            # Byte 1: Columns 0 to 7
            for col in range(8):
                if img.getpixel((col, row)):
                    row_byte1 |= (1 << (7 - col))
            
            # Byte 2: Columns 8 to 15
            for col in range(8, 16):
                if img.getpixel((col, row)):
                    row_byte2 |= (1 << (15 - col))
            
            # Byte 3: Columns 16 and 17 (shift left into the highest bits)
            if img.getpixel((16, row)):
                row_byte3 |= (1 << 7)
            if img.getpixel((17, row)):
                row_byte3 |= (1 << 6)
                
            bytes_list.append(f"0x{row_byte1:02X}")
            bytes_list.append(f"0x{row_byte2:02X}")
            bytes_list.append(f"0x{row_byte3:02X}")
            
        bytes_str = ", ".join(bytes_list)
        
        comment_char = '\\\\' if char == '\\' else char
        if char == "'": comment_char = "\\'"
        
        c_code.append(f"    {{ {{ {bytes_str} }} }}, // '{comment_char}' (ASCII {ascii_val})")
        
    c_code.append("};\n")
    c_code.append("#endif // GLUT_18X30_FONT_H")
    
    with open(output_filename, "w", encoding="utf-8") as f:
        f.write("\n".join(c_code))
    
    print(f"Success! Retina header file saved as: '{output_filename}'")

if __name__ == "__main__":
    FONT_PATH = "static/RobotoMono-Bold.ttf" # not bad
    FONT_PATH = "static/RobotoMono-BoldItalic.ttf" # pixelated edges due to slant
    FONT_PATH = "static/RobotoMono-Regular.ttf"  # best at font_size=24
    font_to_glut_18x30_c_header(font_path=FONT_PATH, font_size=26, output_filename="glut_18x30_font.h")
