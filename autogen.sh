#!/bin/sh
autoheader --force
aclocal --force
autoconf --force
intltoolize -c -f
aclocal --force
automake -a -f --copy
