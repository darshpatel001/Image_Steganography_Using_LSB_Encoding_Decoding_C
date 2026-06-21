# Image Steganography Using LSB Encoding & Decoding in C

## Overview

Image Steganography is the technique of hiding secret information inside a digital image without noticeably altering its appearance.

This project implements **Least Significant Bit (LSB) Steganography** in the C programming language. The application can securely embed a secret text file inside a BMP image and later recover the hidden information from the generated stego image.

The project demonstrates concepts such as:

* File Handling
* Bit Manipulation
* BMP Image Processing
* Data Encoding & Decoding
* Modular Programming
* Command Line Argument Processing

---

## Features

* Hide text files inside BMP images
* Recover hidden text from stego images
* LSB (Least Significant Bit) based encoding
* Magic string verification
* Automatic file extension recovery
* Command-line interface
* Modular C implementation
* Preserves original image appearance

---

## Technologies Used

| Technology       | Purpose                 |
| ---------------- | ----------------------- |
| C Programming    | Core implementation     |
| GCC Compiler     | Compilation             |
| BMP File Format  | Cover image             |
| Bit Manipulation | Data hiding             |
| Linux            | Development environment |

---

## Project Structure

```text
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
├── README.md
```

---

## Working Principle

### Encoding Process

1. Open source BMP image.
2. Open secret text file.
3. Check image capacity.
4. Copy BMP header.
5. Encode magic string.
6. Encode secret file extension.
7. Encode secret file size.
8. Encode secret file data.
9. Generate stego image.

### Decoding Process

1. Open stego image.
2. Verify magic string.
3. Extract file extension.
4. Extract file size.
5. Recover secret data.
6. Create output file.

---

## Architecture

```text
                 +----------------+
                 | Secret File    |
                 +----------------+
                          |
                          v
                 +----------------+
                 | Encoder Module |
                 +----------------+
                          |
                          v
                 +----------------+
                 | BMP Image      |
                 +----------------+
                          |
                          v
                 +----------------+
                 | Stego Image    |
                 +----------------+
                          |
                          v
                 +----------------+
                 | Decoder Module |
                 +----------------+
                          |
                          v
                 +----------------+
                 | Recovered File |
                 +----------------+
```

---

## LSB Encoding Concept

Each pixel byte in a BMP image contains 8 bits.

Example:

```text
Original Byte : 10110110
Secret Bit    : 1

Modified Byte : 10110111
```

Only the Least Significant Bit is modified, making the change visually imperceptible.

---

## Function Descriptions

### Encoding Module

#### read_and_validate_encode_args()

* Validates command-line arguments.
* Verifies source image and secret file.

#### open_encode_files()

* Opens all files required for encoding.

#### check_capacity()

* Checks whether the image can accommodate the secret data.

#### copy_bmp_header()

* Copies the BMP header unchanged.

#### encode_magic_string()

* Encodes a predefined magic string into the image.

#### encode_secret_file_extn()

* Encodes the secret file extension.

#### encode_secret_file_size()

* Encodes the size of the secret file.

#### encode_secret_file_data()

* Encodes the actual secret data.

#### do_encoding()

* Controls the complete encoding process.

---

### Decoding Module

#### read_and_validate_decode_args()

* Validates decoding arguments.

#### open_decode_files()

* Opens stego image and output file.

#### decode_magic_string()

* Verifies hidden data presence.

#### decode_secret_file_extn()

* Extracts original file extension.

#### decode_secret_file_size()

* Retrieves hidden file size.

#### decode_secret_file_data()

* Extracts hidden information.

#### do_decoding()

* Controls the complete decoding process.

---

## Build Instructions

### Compile

```bash
gcc test_encode.c encode.c decode.c -o steg
```

---

## Usage

### Encoding

```bash
./steg -e beautiful.bmp secret.txt stego.bmp
```

Example:

```bash
./steg -e beautiful.bmp secret.txt output.bmp
```

---

### Decoding

```bash
./steg -d stego.bmp
```

or

```bash
./steg -d stego.bmp output.txt
```

---

## Sample Execution

### Encode

```bash
./steg -e beautiful.bmp secret.txt stego.bmp
```

Output:

```text
INFO: Encoding Started
INFO: Magic String Encoded
INFO: Secret File Size Encoded
INFO: Secret Data Encoded
INFO: Encoding Successful
```

---

### Decode

```bash
./steg -d stego.bmp
```

Output:

```text
INFO: Decoding Started
INFO: Magic String Verified
INFO: Secret Data Recovered
INFO: Decoding Successful
```

---

## Concepts Demonstrated

* File Handling
* Structures
* Enumerations
* Bitwise Operations
* Command Line Arguments
* Modular Design
* BMP File Processing
* Data Security Fundamentals
* Software Architecture

---

## Applications

* Secure Message Sharing
* Digital Watermarking
* Information Protection
* Data Concealment
* Cyber Security Learning

---

## Future Improvements

* Password-Protected Steganography
* AES Encryption Integration
* PNG Support
* GUI Application
* Multiple File Embedding
* Audio Steganography
* Video Steganography

---

## Learning Outcomes

Through this project, I gained practical experience in:

* C Programming
* File Processing
* Bit Manipulation
* Debugging
* Linux Development Environment
* Software Design
* Information Security Fundamentals

---

## Author

**Darsh Patel**

Electronics & Communication Engineer

Firmware Engineer | Embedded Systems | IoT Developer

GitHub: https://github.com/darshpatel001

---

## License

This project is licensed under the MIT License.
