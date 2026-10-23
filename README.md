# caffeinate+
Small macOS command-line utility to keep your MacBook awake after you close the lid.
Exit with `Ctrl+C` or `SIGTERM`.
If the lid is still closed, the MacBook is put to sleep.

## Build
```sh
make
```

## Usage
```sh
sudo ./caffeinate+
```

> [!WARNING]
> Be careful when packing away your running MacBook, as it may overheat depending on the workload.
