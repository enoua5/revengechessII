# revengechessII
Web + C++ version of revenge chess. Work in progress.

## Downloading

This project uses git modules which will _not_ be downloaded if you use github's "download zip" feature. Instead, use `git clone --recursive https://github.com/enoua5/revengechessII.git`.

## Building

Build all versions of the project: `make all` or simply `make`

### Command line version

Build the native command-line version: `make native`

Build and run: `make test`

Build and run memory leak analysis: `make memcheck`

### Web version

Build the web version: `make web`

Build the web version, skipping WASM: `make webui`

Only compile the WASM files: `make js_exe`

### Server

Build the server: `make server`

Build just the server executable, skipping config files: `make server_exe`

Build the server, enabling tls: `make server_tls`

Build just the server executable, enabling tls, skipping config files: `make server_tls_exe`

Only copy the default configuation file to the server directory: `make server_conf`

