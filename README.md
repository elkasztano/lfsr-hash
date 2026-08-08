# lfsr-hash

Generate hash values from bytes via stdin. `lfsr-hash` is a lightweight C command-line utility developed as a toy project. **Do not use it for cryptographical purposes.**

## Features

- **Alternating step generator** for hash value generation
- **stdin/stdout I/O**: Simple, composable pipeline behavior
- **Configurable output length**: Specify the number of bytes in the output
- **Colorful output**: Optional colorful command line output
- **Lightweight**: Minimal dependencies, fast execution

## Limitations

- **Not cryptographically secure**: This is a toy project and should not be used for security-sensitive applications.

---

## Building

### Prerequisites

- **C compiler** — `gcc`, `clang`, or compatible (set via `CC` environment variable)
- **GNU Make**

### Compile

Navigate to the `lfsr-hash` directory.
```bash
make
```
This produces the binary at `target/lfsr-hash`.

### Install

The currently preferred way is to create a symlink in a directory in your `PATH`.
```bash
ln -s $PWD/target/lfsr-hash ~/bin
```

### Remove build artifacts

```bash
make clean
```

---
## Usage

```bash
lfsr-hash [options]
```

### Options

| Option | Long Form      | Argument | Description                          |
| ------ | -------------- | -------- | ------------------------------------ |
| `-n`   | `--length`     | `BYTES`  | Output length of generated data in bytes. |
| `-c`   | `--colorful`   | —        | Trigger colorful command line output. |
| `-h`   | `--help`       | —        | Show help and exit.                  |
| `-V`   | `--version`    | —        | Show version and exit.               |

### Examples

- Generate a hash from stdin with default length:
```bash
echo -n "test" | lfsr-hash
```
- Generate a 16-byte hash:
```bash
echo -n "test" | lfsr-hash -n 16
```
- Generate a colorful hash output:
```bash
echo -n "test" | lfsr-hash -c
```

---
## Algorithm

### Alternating Step Generator

The core of `lfsr-hash` is an **alternating step generator**, a simple yet effective method for generating pseudo-random hash values from input bytes. This implementation is designed for educational and experimental purposes.

