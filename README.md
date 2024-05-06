# Desktop Shanghai
Putting Shanghai on your desktop, hopefully.
Because you know you want one.

## Attribution
I'll be honest, I don't know who made the original.
The folder was named `shanghai` and the code was Java, and the closest reference I could find was [some framework named Shimeji](https://web.archive.org/web/20160715043803/http://www.group-finity.com/Shimeji/).
I also blame a certain *someone* for making me do this.

## Building
Inherently a Wayland application, so you got to have `wayland-protocols` installed. It'll use `wayland-scanner` to generate the necessary headers.
It's also CMake so you gotta do the usual `cmake . && make` dance.

## Tasks
- [x] Rewrite into C++ from `layer-shell.c`
- [x] Load images as GL textures (uses stb_image)
- [ ] Reverse engineer Shimiji's behavior
  - [ ] Implement the animation/agent state machine
  - [ ] Implement animation in EGL
