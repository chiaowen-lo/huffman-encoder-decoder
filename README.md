# huffman-encoder-decoder

This is a command-line program that implements **Huffman coding**, a lossless data compression algorithm. It supports both compression and decompression of files in binary mode, and it outputs the Huffman encoding table and compression ratio.

## Features

- Compress files using Huffman coding
- Decompress Huffman-compressed files
- Supports any file format (text, images, etc.)
- Outputs file statistics and encoding table
- Binary file handling
- Command-line interface (CLI)

## How to Use

```bash
# Compress a file
./huffman -c -i input.txt -o compressed.huff

# Decompress a file
./huffman -u -i compressed.huff -o output.txt
