# here a survival kit
[installing xmake on Arch](https://xmake.io/guide/quick-start.html#linux-distributions) : 
```
sudo pacman -S xmake
```
building, running (running implies building)
```
xmake build
```
```
xmake run
```
[generating an make/cmake](https://xmake.io/guide/extensions/builtin-plugins.html) project for our dear teachers
```
xmake project -k make
xmake project -k cmake
```
Generate clangd configuration file:
```
xmake project -k compile_commands
```

by default xmake will use the release profil (which is roughly ``-O3``), switching to the Debug profil will enable debug symbols and asan: 
```
xmake f -m debug
```

# Naming conventions and formatting
- all files should be formatted using clangd default settings
- all structs/unions name should be in PascalCase
- constant expression such as ``#define BOARD_WIDTH`` should be in UPPER_CASE
- functions should be written in snake_case
- methods (function associated with a struct definition) should be prefixed by their struct name : ``Struct_foo()``
