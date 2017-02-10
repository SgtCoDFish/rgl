# rgl
rgl is a tiny roguelike which is built to scratch an itch.

# Building
You'll need to install [AshleyCPP](https://github.com/SgtCoDFish/AshleyCPP) and have SDL2 available.

You'll also (currently) need [LibTCOD](http://roguecentral.org/doryen/libtcod/) which is a roguelike game library. It's available as `libtcod-dev` on recent versions of Ubuntu.

An OSX port is available [on GitHub](https://github.com/podiki/libtcod-mac) but it might take some wrestling to get it working.

If you get it running on Windows, please do tell me about it!

To build:

```
mkdir build
cmake -DCMAKE_BUILD_TYPE=<Debug/Release> ..
make [-j4]
```
