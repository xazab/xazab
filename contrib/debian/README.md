
Debian
====================
This directory contains files used to package xazabd/xazab-qt
for Debian-based Linux systems. If you compile xazabd/xazab-qt yourself, there are some useful files here.

## xazab: URI support ##


xazab-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install xazab-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your xazab-qt binary to `/usr/bin`
and the `../../share/pixmaps/xazab128.png` to `/usr/share/pixmaps`

xazab-qt.protocol (KDE)

