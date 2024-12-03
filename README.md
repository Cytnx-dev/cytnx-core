# Cytnx-core
    This is the core compoment of the cytnx library

## Develop

### Package Management
We use [uv](https://docs.astral.sh/uv/getting-started/installation/) to manage the package.

```bash
    uv sync --all-extras --dev
```

To trigger recompile of C++ code:

```bash
    uv sync --reinstall
```

In addition, pre-commit tool should be installed

```bash
    uv run pre-commit install
```

Running pytest:

```bash
    uv run pytest
```

### Building Dependency:

- c++ compiler
- cmake 3.15+ (see CMakeList.txt, default 3.20)

* most of the deps should be able to install via pypi.


## Compile directly the C++ package

```bash
   $mkdir build
   $cd build
   $cmake ../ -DCMAKE_INSTALL_PREFIX=<install destination>
```
