# Gaea
# Copyright (C) 2016 David Jolly
# ----------------------
#
# Gaea is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Gaea is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -O3 -Wall -Werror
CC_FLAGS_EXT=-lGL -lGLU -lGLEW -lSDL2 -lSDL2main -lpng -pthread
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./
DIR_SRC=./src/
EXE=gaea
LIB=libgaea.a

all: exe

### EXECUTABLES ###

exe:
	@echo ''
	@echo '--- BUILDING TOOL --------------------------' 
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) main.cpp $(DIR_BIN)$(LIB) -o $(DIR_BIN)$(EXE)
	@echo '--- DONE -----------------------------------'
	@echo ''
