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
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_ENG=./../include/engine/
DIR_INC_GFX=./../include/graphics/
DIR_SRC=./
DIR_SRC_ENG=./engine/
DIR_SRC_GFX=./graphics/
LIB=libgaea.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)gaea.o $(DIR_BUILD)gaea_camera.o $(DIR_BUILD)gaea_cubemap.o $(DIR_BUILD)gaea_entity.o \
		$(DIR_BUILD)gaea_event.o $(DIR_BUILD)gaea_exception.o $(DIR_BUILD)gaea_gl.o $(DIR_BUILD)gaea_object.o $(DIR_BUILD)gaea_program.o \
		$(DIR_BUILD)gaea_shader.o $(DIR_BUILD)gaea_signal.o $(DIR_BUILD)gaea_surface.o $(DIR_BUILD)gaea_texture.o $(DIR_BUILD)gaea_uid.o \
		$(DIR_BUILD)gaea_vao.o $(DIR_BUILD)gaea_vbo.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_engine build_graphics

### BASE ###

build_base: gaea.o gaea_exception.o

gaea.o: $(DIR_SRC)gaea.cpp $(DIR_INC)gaea.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC)gaea.cpp -o $(DIR_BUILD)gaea.o

gaea_exception.o: $(DIR_SRC)gaea_exception.cpp $(DIR_INC)gaea_exception.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC)gaea_exception.cpp -o $(DIR_BUILD)gaea_exception.o

### ENGINE ###

build_engine: gaea_camera.o gaea_entity.o gaea_event.o gaea_object.o gaea_signal.o gaea_surface.o gaea_uid.o

gaea_camera.o: $(DIR_SRC_ENG)gaea_camera.cpp $(DIR_INC_ENG)gaea_camera.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_camera.cpp -o $(DIR_BUILD)gaea_camera.o

gaea_entity.o: $(DIR_SRC_ENG)gaea_entity.cpp $(DIR_INC_ENG)gaea_entity.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_entity.cpp -o $(DIR_BUILD)gaea_entity.o

gaea_event.o: $(DIR_SRC_ENG)gaea_event.cpp $(DIR_INC_ENG)gaea_event.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_event.cpp -o $(DIR_BUILD)gaea_event.o

gaea_object.o: $(DIR_SRC_ENG)gaea_object.cpp $(DIR_INC_ENG)gaea_object.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_object.cpp -o $(DIR_BUILD)gaea_object.o

gaea_signal.o: $(DIR_SRC_ENG)gaea_signal.cpp $(DIR_INC_ENG)gaea_signal.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_signal.cpp -o $(DIR_BUILD)gaea_signal.o

gaea_surface.o: $(DIR_SRC_ENG)gaea_surface.cpp $(DIR_INC_ENG)gaea_surface.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_surface.cpp -o $(DIR_BUILD)gaea_surface.o

gaea_uid.o: $(DIR_SRC_ENG)gaea_uid.cpp $(DIR_INC_ENG)gaea_uid.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_ENG)gaea_uid.cpp -o $(DIR_BUILD)gaea_uid.o

### GRAPHICS ###

build_graphics: gaea_cubemap.o gaea_gl.o gaea_program.o gaea_shader.o gaea_texture.o gaea_vao.o gaea_vbo.o

gaea_cubemap.o: $(DIR_SRC_GFX)gaea_cubemap.cpp $(DIR_INC_GFX)gaea_cubemap.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_cubemap.cpp -o $(DIR_BUILD)gaea_cubemap.o

gaea_gl.o: $(DIR_SRC_GFX)gaea_gl.cpp $(DIR_INC_GFX)gaea_gl.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_gl.cpp -o $(DIR_BUILD)gaea_gl.o

gaea_program.o: $(DIR_SRC_GFX)gaea_program.cpp $(DIR_INC_GFX)gaea_program.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_program.cpp -o $(DIR_BUILD)gaea_program.o

gaea_shader.o: $(DIR_SRC_GFX)gaea_shader.cpp $(DIR_INC_GFX)gaea_shader.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_shader.cpp -o $(DIR_BUILD)gaea_shader.o

gaea_texture.o: $(DIR_SRC_GFX)gaea_texture.cpp $(DIR_INC_GFX)gaea_texture.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_texture.cpp -o $(DIR_BUILD)gaea_texture.o

gaea_vao.o: $(DIR_SRC_GFX)gaea_vao.cpp $(DIR_INC_GFX)gaea_vao.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_vao.cpp -o $(DIR_BUILD)gaea_vao.o

gaea_vbo.o: $(DIR_SRC_GFX)gaea_vbo.cpp $(DIR_INC_GFX)gaea_vbo.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) -c $(DIR_SRC_GFX)gaea_vbo.cpp -o $(DIR_BUILD)gaea_vbo.o
