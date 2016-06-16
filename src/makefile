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
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_SRC=./
LIB=libgaea.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)gaea.o $(DIR_BUILD)gaea_exception.o $(DIR_BUILD)gaea_gl.o $(DIR_BUILD)gaea_object.o \
		$(DIR_BUILD)gaea_uid.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: gaea.o gaea_exception.o gaea_gl.o gaea_object.o gaea_uid.o

### GAEA ###

gaea.o: $(DIR_SRC)gaea.cpp $(DIR_INC)gaea.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gaea.cpp -o $(DIR_BUILD)gaea.o

gaea_exception.o: $(DIR_SRC)gaea_exception.cpp $(DIR_INC)gaea_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gaea_exception.cpp -o $(DIR_BUILD)gaea_exception.o

### GAEA::[...] ###

gaea_gl.o: $(DIR_SRC)gaea_gl.cpp $(DIR_INC)gaea_gl.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gaea_gl.cpp -o $(DIR_BUILD)gaea_gl.o

gaea_object.o: $(DIR_SRC)gaea_object.cpp $(DIR_INC)gaea_object.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gaea_object.cpp -o $(DIR_BUILD)gaea_object.o

gaea_uid.o: $(DIR_SRC)gaea_uid.cpp $(DIR_INC)gaea_uid.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gaea_uid.cpp -o $(DIR_BUILD)gaea_uid.o
