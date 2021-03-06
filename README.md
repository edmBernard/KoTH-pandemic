# KoTH: A world-wide pandemic

A simple King of The Hill game. The goal is to allow user to create bot that play the game. Example of of problem can be found here [King of the hill](https://codegolf.stackexchange.com/questions/tagged/king-of-the-hill).

The game implemented on this [repository](https://github.com/edmBernard/KoTH-pandemic) is a reimplementation of this [game](https://codegolf.stackexchange.com/questions/70135/koth-a-world-wide-pandemic).

## Add a Bot

To add a new bot. You just have to create new `*.cpp` file inside `bots` directory. There is some bots example in the `bots/basic.cpp` file. Keep one bots per file. The game can be run locally with the `koth-pandemic` executable otherwise the game is run with github action on each push on the `main` branch and on each pull request.

## Compilation

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

This project depends on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- [spdlog](https://github.com/gabime/spdlog): Very fast, header-only/compiled, C++ logging library


```
./vcpkg install spdlog cxxopts fmt
```

### Compilation

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
cmake.exe --build . --config Release
```

the server executable is named `koth-pandemic`

## Disclaimer

It's a toy project. So if you spot error, improvement comments are welcome.
