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

DIR_BIN=./bin/
DIR_BUILD=./build/
DIR_EXE=./test/
DIR_INC=./include/
DIR_LOG=./log/
DIR_ROOT=./
DIR_SRC=./src/
EXE=gaea
JOB_SLOTS=4
LOG_MEM=val_err.log
LOG_STAT=stat_err.log
LOG_CLOC=cloc_stat.log

all: build

build: clean init lib exe

clean:
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_BUILD)
	rm -rf $(DIR_LOG)

exe:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING EXECUTABLES'
	@echo '============================================'
	cd $(DIR_EXE) && make FLAGS_EXT=$(FLAGS_EXT)

init:
	mkdir $(DIR_BIN)
	mkdir $(DIR_BUILD)
	mkdir $(DIR_LOG)

lib: 
	@echo ''
	@echo '============================================'
	@echo 'BUILDING LIBRARIES'
	@echo '============================================'
	cd $(DIR_SRC) && make FLAGS_EXT=$(FLAGS_EXT) build -j $(JOB_SLOTS)
	cd $(DIR_SRC) && make archive

### TESTING ###

test: static mem

mem:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING MEMORY TEST'
	@echo '============================================'
	valgrind --leak-check=full $(DIR_BIN)$(EXE) 2> $(DIR_LOG)$(LOG_MEM)

static:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING STATIC ANALYSIS'
	@echo '============================================'
	cppcheck --enable=all --std=c++11 $(DIR_SRC) 2> $(DIR_LOG)$(LOG_STAT)

### STATISTICS ###

stat: lines

lines:
	@echo ''
	@echo '============================================'
	@echo 'CALCULATING LINE COUNT'
	@echo '============================================'
	cloc $(DIR_ROOT) > $(DIR_LOG)$(LOG_CLOC)
