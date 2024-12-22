# A Wordle Clone or the GBA!

This started out as a little hobby project adfter wanting to create a Wordle clone
that isn't just limited to one game per day. It has now come to be one of my most
impressive projects to date. All assets (aside from the Wordle lettering sprite tiles)
have been generated using my own home-built tools: the font parser and the huffman 
compression tools, which you can see [here](https://github.com/bsumner2/GBA_Tools).

## Recent Additions

- Changed the build system from Make to CMake. See below how to compile using CMake
- Added 
- (TODO; Future Addition) Finally implement ability to have multiple save profiles
  that you can switch between
- Compressed the word and answer lists using Huffman Compression, formatted with
  appropriate header metadata for compatibility, specifically, with the 
  GBA's Huffman Decompression supervisor call, pre-provided in BIOS
  on all authentic GBAs. You can find the command line tool at my [GBA Tools repo](https://github.com/bsumner2/GBA_Tools)
  here on github!

## Build ROM

```shell
mkdir build ; cd build
cmake ..
cmake --build .
```

## Build ROM and Test ROM with mgba-qt

```shell
mkdir build ; cd build
cmake ..
cmake --build . --target test
```

## Special Thanks To Arjun Vikram

Thanks for the plaintext files with Wordle's word and answers DB!
I snagged them from his [repo on dagshub](https://dagshub.com/arjvik/wordle-wordlist).
