# ImageSteganography

# Compilation

```bash
meson build 
meson compile -C build
```
alternatively run for release build

```bash
meson release -Dbuild=release
meson compile -C release
```

# Usage
```
Usage: imagesteganography [OPTION] ...
Supported formats:                       PPM-P6, Windows-BMP

-i, --info                               shows size, last modification date, image size
-e, --encrypt [FILE] "[MESSAGE]"         encrypts file with specified message
-d, --decrypt [FILE]                     decrypts file
-c, --check   [FILE] "[MESSAGE]"         checks whether message can be encrypted into specified file
-h, --help                               shows help
```

To encrypt message into file:
```bash
./image_steganography -e ./sample.bmp "sample message"
```

To decrypt message from file run:
```bash
./image_steganography -d ./sample.bmp 
```
