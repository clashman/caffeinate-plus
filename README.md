# caffeinate+
Small macOS command-line utility to keep your MacBook awake after you close the lid.
Exit with `Ctrl+C` or `SIGTERM`.
If the lid is still closed, the MacBook is put to sleep.

## Requirements
- macOS on a MacBook with a lid sensor
- A C compiler and `make`, available through the Xcode Command Line Tools

## Build
```sh
make
```

## Usage
```sh
sudo ./caffeinate+
```
