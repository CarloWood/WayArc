set debuginfod enabled on

#set environment WLR_BACKENDS=wayland
#set environment WAYLAND_DISPLAY=wayland-1
#set environment WLR_RENDER_DRM_DEVICE=/dev/dri/renderD128
#set environment LD_PRELOAD=/usr/local/install/14.2.1-20250128/lib/libmemleak.so

# Allow interrupting gdb with SIGCONT.
handle SIGCONT nopass stop

# Add the BUILDDIR to the search path, for generated source files.
python
import os
gdb.execute('directory $cdir:$cwd:' + os.environ['BUILDDIR'])
end
