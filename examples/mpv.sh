#!/bin/sh

APP=$(basename "$0")
APP=${APP%.sh}
make "$APP" || exit

die()     { printf '%s: %s ... abort\n' "$APP" "$*" >&2; exit 3; }
exists()  { command -v "$@" > /dev/null; }

exists 'mpv' || die 'requires mpv media player as a backend'
./"$APP" | mpv --title="$APP"            \
               --really-quiet            \
               --no-correct-pts --fps=12 \
               --loop \
               "$@"  -
