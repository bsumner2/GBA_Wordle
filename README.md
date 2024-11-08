# A Wordle Clone or the GBA!

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
I snagged them from his prepo on dagshub](https://dagshub.com/arjvik/wordle-wordlist).
