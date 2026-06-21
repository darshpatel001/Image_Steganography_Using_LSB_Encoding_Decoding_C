# Image Steganography Using LSB Encoding & Decoding in C

## Overview

This project implements **Image Steganography** using the **Least Significant Bit (LSB)** technique in the C programming language.

The application allows users to:

- Hide a secret text file inside a BMP image.
- Extract the hidden text file from the stego image.
- Preserve the visual appearance of the original image.
- Perform encoding and decoding through command-line arguments.

---

## Features

- BMP Image Support
- LSB-Based Data Hiding
- Secret File Encoding
- Secret File Decoding
- Magic String Verification
- Automatic File Extension Recovery
- Command Line Interface
- Pure C Implementation

---

## Project Structure

```
.
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── test_encode.c
├── beautiful.bmp
├── secret.txt
├── decodeit.bmp
└── README.md
```

---

## Working Principle

### Encoding

1. Read source BMP image.
2. Read secret text file.
3. Verify image capacity.
4. Copy BMP header.
5. Embed:
   - Magic String (`#*`)
   - Secret file extension
   - Secret file size
   - Secret file data
6. Generate stego image.

### Decoding

1. Open stego BMP image.
2. Verify magic string.
3. Extract file extension.
4. Extract file size.
5. Recover hidden data.
6. Generate decoded output file.

---

## Magic String

The project uses a magic string to verify whether hidden data exists.

```c
#define MAGIC_STRING "#*"
```

---

## Build Instructions

### Compile

```bash
gcc test_encode.c encode.c decode.c -o steg
```

### Run Encoding

```bash
./steg -e beautiful.bmp secret.txt output.bmp
```

Example:

```bash
./steg -e beautiful.bmp secret.txt stego.bmp
```

---

## Run Decoding

```bash
./steg -d stego.bmp decoded.txt
```

Example:

```bash
./steg -d stego.bmp
```

---

## Sample Workflow

### Secret File

```text
Hello Darsh Patel!
Welcome to Image Steganography.
```

### Encoding

```bash
./steg -e beautiful.bmp secret.txt stego.bmp
```

### Decoding

```bash
./steg -d stego.bmp output.txt
```

### Output

```text
Hello Darsh Patel!
Welcome to Image Steganography.
```

---

## Concepts Used

- File Handling
- Bit Manipulation
- BMP File Format
- Data Encoding
- Data Decoding
- LSB Steganography
- Structures
- Command Line Arguments
- Modular Programming

---

## Future Improvements

- PNG Support
- AES Encryption Before Encoding
- GUI Application
- Multiple File Support
- Password Protected Steganography
- Audio & Video Steganography

---

## Author

**Darsh Patel**

Electronics & Communication Engineer  
Firmware & Embedded Systems Enthusiast

GitHub: https://github.com/darshpatel001

---

## License

This project is licensed under the MIT License.
