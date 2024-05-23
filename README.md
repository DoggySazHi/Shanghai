# Desktop Shanghai
Putting Shanghai on your desktop, hopefully.
Because you know you want one.

## Attribution
The original artwork [was found on Pixiv.](https://www.pixiv.net/en/artworks/14618579)

Code is derived from [the Shimeiji application (Java)](https://web.archive.org/web/20160715043803/http://www.group-finity.com/Shimeji/).
I also blame a certain *someone* for making me do this.

## Building
Inherently a Wayland application, so you got to have `wayland-protocols` installed. It'll use `wayland-scanner` to generate the necessary headers.
It's also CMake so you gotta do the usual `cmake . && make` dance.

There is also primitive code for running it on other platforms (running on GLFW), but it's not really tested.
Debug it at your own risk. It handles Windows' z-order and window management, but not much else.
MacOS is also not supported, but it should be possible to run it on macOS with some modifications (AppleScript?).

## Tasks
- [x] Rewrite into C++ from `layer-shell.c`
- [x] Load images as GL textures (uses stb_image)
- [ ] Reverse engineer Shimeji's behavior
  - [ ] Implement the animation/agent state machine
  - [x] Implement animation in EGL
