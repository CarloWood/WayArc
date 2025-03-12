WayArc is a Wayland compositor inspired by the aesthetic and floating
window management of Fluxbox, with a focus on robust C++20 design
principles—emphasizing encapsulation, type-safety, and maintainability—while
still being a work in progress.

SETTING UP THE BUILD ENVIRONMENT
================================

On Archlinux you need to following packages:

```
  sudo pacman -S git clang ninja pkgconfig cmake wlroots wayland-protocols wayland ccache
```

The last one is not absolutely required, but you really want to install it.

You also need packages that are on github, but not provided by Arch (or typically any distribution).
The project automatically clones, configures, compiles and installs those packages using `gitache`.
For `gitache` to work you need to specify a directory that it can use for all of the above; this
directory must be writable by you (and preferably empty when we start).

For example, if you use `/opt/gitache`, you will need:

```
  export GITACHE_ROOT=/opt/gitache
  sudo mkdir $GITACHE_ROOT
  sudo chown $(id -u):$(id -g) $GITACHE_ROOT
```

Of course, you can put it also in your home directory if you think the partition of your
home directory is large enough.

Usually it is necessary to run `./autogen.sh` after cloning a repository for the first time.
In order for that script to only set things up for cmake, and not automake, you want to
set the following environment variable:

```
  export AUTOGEN_CMAKE_ONLY=1
```
or just set it while running autogen.sh, like so: `AUTOGEN_CMAKE_ONLY=1 ./autogen.sh`.

Finally add the following file as `~/.libcwdrc` in your home directory:
```
  silent = on
  channels_default = off

  channels_on = warning,debug,notice

  gdb = /usr/bin/gdb -x $REPOBASE/.gdbinit $GDBCOMMANDFILE
  xterm = konsole --name "attach_gdb" --hide-menubar --nofork --workdir "$PWD" --geometry 165x24-0-0 -e %s
```
or, if you already have that file, make sure that the debug channels `warning`, `debug` and `notice` are on.
Note how this uses `$REPOBASE`. Therefore, in order to make `attach_gdb()` work from within the
program you should also add to your environment:
```
  export REPOBASE=/full/path/to/WayArc
```
Using the directory name of where you cloned this project. Namely, the repository contains a `.gdbinit` which then will be loaded.

After all of this is set up, you can continue with getting the source code, configuration and compilation:

GETTING THE SOURCE CODE
=======================

```
  git clone --recursive https://github.com/CarloWood/WayArc.git
  cd WayArc
  ./autogen.sh
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON
  cmake --build build --config Debug --parallel $(nproc --all)
  cd build/src
  ./WayArc
```
