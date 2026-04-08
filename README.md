<div align="center">

# pocketwalker - An H8/38606F emulator built for the PokéWalker

[![Release](https://img.shields.io/github/release/h4lfheart/pocketwalker?color=CCC)]()
[![Downloads](https://img.shields.io/github/downloads/h4lfheart/pocketwalker/total?color=CCC)]()

<img alt="PocketWalker" src=".github/images/cover.gif" width="400" />

</div>

---

## Features

### H8/300H CPU
- Full instruction set used by the PokéWalker
- Support for interrupts

### H8/38606F
- Memory Bus with 48KB ROM, 2KB RAM, and IO
- Synchronous Serial Communication Unit (SSU)
- Serial Communication Interface 3 (SCI3)
- Timer B1
- Timer W
- Real-Time Clock (RTC)
- A/D Converter (ADC)

### PokéWalker
- Accelerometer (BMA150)
- EEPRom (M95512)
- LCD (SSD1854)
- Buzzer
- IR Communications over TCP with [melonDS-IR](https://github.com/DaveuRrr/melonDS-IR)

## Planned Features

### Emulation
- Synthetic Step Accumulation
- Watt/Step Insertion
- Save States

### Application

#### General Settings
- Theme (Light/Dark)
- Default ROM Path
- Default Save Path
- Boot on Launch

#### Emulation Settings
- Custom Palettes
- Emulation Speed Multiplier
- RTC Sync Mode

#### Control Settings
- Keybinds for Main Buttons
- Speedup Button with Hold/Toggle

#### IR Settings
- IR Communication Mode (TCP, UDP, Serial)
- Client/Server Mode
- Host IP
- Port

## Building From Source

To build pocketwalke from source, first clone the repository and all of its submodules.

```
git clone https://github.com/h4lfheart/pocketwalker --recursive
```

Then run the build script at the root of the repository. Please ensure you have a compiler installed with C++23 support.

```
./build.sh
```

The output binary will be located in `./release/`.