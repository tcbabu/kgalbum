#!/bin/bash
install -m 755 -s kgalbum /usr/bin/
mkdir -p /usr/share/kgalbum
install -m 644  kgalbum.png /usr/share/kgalbum/kgalbum.png
install -m 644  kgalbum.desktop /usr/share/applications
