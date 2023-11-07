# Pong

A simple implementation of a pong game using
my own sdl2-based "game dev framework" using the c programming language

## Local deployment

You'll need a fewer requirements first.

- gcc
- make
- sdl2
- sdl2 mixer
- sdl2 ttf
- sdl2 image
- cjson
- git

Now you can continue by executing the next commands in your shell:

```sh
mkdir -pv ~/repo
cd ~/repo
git clone https://github.com/alphatechnolog/pong.git --depth=1 && cd pong
```

Now to build the source code:

```sh
make -j$(nproc)
```

And then the binary will be at `./bin/pong`, but you can use make
to execute it anyways.

```sh
make run
```

Hint: use `make clean` to remove all the compiled files if you need.

## Fun things

Well, fullscreen works too, but you can press f11 again to go
to windowed mode again if needed, and, the right bar of the pong
adapts to the size of the window ;) (yeah, responsive).

## Still to do

- Score displaying
