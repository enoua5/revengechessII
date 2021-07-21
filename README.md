# revengechessII
Web + C++ version of revenge chess. Work in progress.

## Downloading

This project uses git modules which will _not_ be downloaded if you use github's "download zip" feature. Instead, use `git clone --recursive https://github.com/enoua5/revengechessII.git`.

## Building

Build all versions of the project: `make all` or simply `make`

### Command line version

Build just the exe version: `make native`

Build and run: `make test`

Build and run memory leak analysis: `make memcheck`

### Web version

Build just the web version: `make web`

Build the web version, skipping wasm: `make webui`
