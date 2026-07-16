# Cgride Toolchains

Native compiler discovery and command generation for Cgride.

Cgride Toolchains is the compiler toolchain module. It describes native compiler families, discovers compiler executables, and creates compile, archive and link commands as data.

It does not execute commands, schedule work, inspect build graphs or decide whether files are dirty.

## Purpose

This module provides the toolchain-level types used by the Cgride engine:

- source languages
- compiler kinds
- artifact kinds
- toolchain descriptions
- compiler discovery
- compile command creation
- archive command creation
- link command creation
- GCC-like command generation
- MSVC-like command generation

Cgride Toolchains keeps command construction separate from command execution. Higher-level modules can use this module to produce `cgride::core::Command` values, then pass those commands to an executor.

## Requirements

- C++23
- Vix CLI
- Vix.cpp
- Cgride Core

## Build

```bash
vix build --build-target all
```

## Run tests

```bash
vix tests
```

## Release build

```bash
vix build --preset release --build-target all
```

## Build

From the module directory, use the Vix workflow:

```bash
vix build
```

For a release build:

```bash
vix build --preset release
```

## Run tests

```bash
vix check --tests
```

## Install

```bash
vix install
```

The install step exposes the `cgride::toolchains` integration target, public headers, and package metadata.

## Integration

C++ integrations can use the installed module target from their project build configuration.


## Basic example

```cpp
#include <cgride/toolchains/compile_command.hpp>
#include <cgride/toolchains/toolchain.hpp>

int main()
{
  cgride::toolchains::Toolchain toolchain(
      cgride::toolchains::CompilerKind::Gcc,
      "GCC");

  toolchain
      .cxx_compiler("/usr/bin/g++")
      .archiver("/usr/bin/ar")
      .linker("/usr/bin/g++");

  cgride::toolchains::CompileCommandOptions options;
  options.language = cgride::toolchains::Language::Cxx;
  options.source = "src/main.cpp";
  options.object = "build/main.o";
  options.cxx_standard = "c++23";

  auto command = cgride::toolchains::make_compile_command(toolchain, options);

  if (!command)
  {
    return 1;
  }

  return 0;
}
```

## Discovery example

```cpp
#include <cgride/toolchains/discovery.hpp>

int main()
{
  auto result = cgride::toolchains::discover_toolchain(
      cgride::toolchains::CompilerKind::Gcc);

  if (!result)
  {
    return 1;
  }

  auto toolchain = result.value();

  return toolchain.valid() ? 0 : 1;
}
```

## Module boundary

Cgride Toolchains may depend on:

- `cgride::core`

Cgride Toolchains may be used by:

- `cgride::engine`
- `cgride::config`
- `cgride::cli`
- external runtimes
- IDE integrations
- developer tools

Cgride Toolchains must not depend on:

- `cgride::project`
- `cgride::graph`
- `cgride::executor`
- `cgride::cache`
- `cgride::engine`
- `cgride::config`
- `cgride::cli`

## Design rule

This module stores compiler and command construction knowledge only.

It should answer:

```txt
Which compiler executable is available?
Which command should compile this file?
Which command should archive these objects?
Which command should link this artifact?
```

It should not answer:

```txt
Which targets exist?
Which tasks should run first?
Which files are dirty?
How many workers should execute commands?
How should terminal output be printed?
```

Those decisions belong to higher-level Cgride modules.

## License

MIT
