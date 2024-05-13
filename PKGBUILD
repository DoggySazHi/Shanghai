# Maintainer: Reimu Hakurei <reimu@williamle.com>
pkgname=shanghai
pkgver=1.0.0
pkgrel=1
epoch=
pkgdesc="Putting Shanghai on your Wayland desktop"
arch=("i686", "x86_64", "aarch64")
url="https://github.com/DoggySazHi/Shanghai"
depends=("wayland")
makedepends=("cmake", "wayland-protocols", "ninja", "pkgconf", "jsoncpp")
license=("MIT")
source=("https://github.com/DoggySazHi/Shanghai/archive/refs/heads/hourai.zip")

prepare() {
	cd "$srcdir"
    unzip "$srcdir/hourai.zip"
}

build() {
	cd "$srcdir/Shanghai-hourai"
    cmake -DCMAKE_BUILD_TYPE= \
        -DCMAKE_INSTALL_PREFIX="/usr" \
        -DLauncher_BUILD_PLATFORM="archlinux" \
        -Bbuild -S.

    cmake --build build
}

package() {
    cd "$srcdir/Shanghai-hourai"
    DESTDIR="$pkgdir" cmake --install build
}
