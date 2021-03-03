# KoTH: A world-wide pandemic

The goal is to allow user to create bot that play the game. And in the futur be able to made a [King of the hill](https://codegolf.stackexchange.com/questions/tagged/king-of-the-hill) game with bot ranking.

https://codegolf.stackexchange.com/questions/70135/koth-a-world-wide-pandemic

version grpc
    bots are grpc worker that expose one / several fonction
        grpc allow python/Node/C++
    bots can call function from engine to get information ? maybe in more evolved game
    how to register new bots folder name to prefix function ?
version stdin stdout
    bots are executable script that take input as stdin and output as stdout
    how to register each folder contain new bots, the bot entrypoint is call run ?

infra:
    sur optiplex:
        pull each hour and run all bot
    then update score board:
        score board github page ?
        score board on my ebernard.fr
        where is database for score board that why github page is cool
            I directly push result

proof of concept
    [ ]: grpc test with c++ / python / node
    [ ]: comment register un nouveau bot
        - script automatic qui génére un fichier .h
        - script qui génére un ficher .json
        - un serveur grpc par bot ?
    [ ]: tester avec un stdin / stdout
        - un docker qui contient tout les Python / Node / C++
    [ ]: automtic pull crontab
    [ ]: automtic build
    [ ]: automatic push
    [ ]: github page with score


## Compilation, Tests and Documentations

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

KoTH-snake depends on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- spdlog
- grpc

```
./vcpkg install uWebSocket cxxopts fmt Catch2
```

### Compile , Test

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
make
```

the server executable is named `koth-snake-server`

## API

To be able to interact with the server there is few API implemented :

* `GET` : `/api/register/:name/:color` : Register a new player with its color
    * `curl http://<my-server-address>/api/register/XXXX/Y`
* `GET` : `/api/status/:key` : Get status (color, coordinate) of a player
    * `curl http://<my-server-address>/api/status/XXXX`
* `POST` : `/api/move/:key/:direction` : Send a new direction order to the server for this player
    * `curl -d "" http://<my-server-address>/api/move/XXXX/Y`
* `GET` : `/api/map/buffer` : Get the colored map as a vectorized image
    * `curl http://<my-server-address>/api/map/buffer`
* `GET` : `/api/map/size` : Get the map size
    * `curl http://<my-server-address>/api/map/size`
* `GET` : `/api/players/buffer` : Get the map indicating all player positions
    * `curl http://<my-server-address>/api/players/buffer`

## Disclaimer

It's a toy project. Mainly used to learn uwebsocket library.
I also try to make a more data oriented design.
So if you spot error, improvement comments are welcome.
