import sys
if len(sys.argv) != 3:
    print("Usage: python3 convert_model.py model.tflite model.cc")
    sys.exit(1)
    
with open(sys.argv[1], "rb") as f:
    data = f.read()

with open(sys.argv[2], "w") as f:
    f.write("#include <stdint.h>\n\n")
    f.write(f"const unsigned char g_model[] = {{\n")
    for i, b in enumerate(data):
        f.write(f"0x{b:02x},")
        if (i + 1) % 12 == 0:
            f.write("\n")
    f.write("\n};\n")
    f.write(f"const int g_model_len = {len(data)};\n")

print("generated")