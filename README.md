# KoTH: A world-wide pandemic

A simple King of The Hill game. The goal is to allow user to create bot that play the game. Example of of problem can be found here [King of the hill](https://codegolf.stackexchange.com/questions/tagged/king-of-the-hill).

The game implemented on this [repository](https://github.com/edmBernard/KoTH-pandemic) is a reimplementation of this [game](https://codegolf.stackexchange.com/questions/70135/koth-a-world-wide-pandemic)


## Compilation, Tests and Documentations

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

This project depends on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- [spdlog](https://github.com/gabime/spdlog): Very fast, header-only/compiled, C++ logging library


```
./vcpkg install spdlog cxxopts fmt
```

### Compile , Test

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
make
```

the server executable is named `koth-pandemic`

## Disclaimer

It's a toy project. Mainly used to learn uwebsocket library.
I also try to make a more data oriented design.
So if you spot error, improvement comments are welcome.
