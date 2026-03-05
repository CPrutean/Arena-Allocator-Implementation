#!/bin/bash

#Change these, these are my personal include paths

INCLUDE_PATH="/opt/homebrew/Cellar/cunit/2.1-3/include"
LINK_PATH="/opt/homebrew/Cellar/cunit/2.1-3/lib/"

gcc -o test_add arena.c tests.c -I"${INCLUDE_PATH}" -L"${LINK_PATH}" -lcunit
