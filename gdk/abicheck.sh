#! /bin/sh

has_x11_backend=no
has_wayland_backend=no

# GDK_BACKENDS passed to this script from gdk/Makefile.am
for backend in ${GDK_BACKENDS}; do
    case "$backend" in
        x11)
            has_x11_backend=yes
            ;;
        wayland)
            has_wayland_backend=yes
            ;;
    esac
done

cppargs=

if [ $has_x11_backend = "yes" ]; then
    cppargs="$cppargs -DGDK_WINDOWING_X11"
fi

if [ $has_wayland_backend = "yes" ]; then
    cppargs="$cppargs -DGDK_WINDOWING_WAYLAND"
fi

cpp -P $cppargs ${srcdir:-.}/gdk.symbols | sed -e '/^$/d' -e 's/ G_GNUC.*$//' | sort | uniq > expected-abi
nm -D -g --defined-only .libs/libgdk-3.so | cut -d ' ' -f 3 | egrep -v '^(__bss_start|_edata|_end)' | sort > actual-abi
diff -u expected-abi actual-abi && rm -f expected-abi actual-abi
