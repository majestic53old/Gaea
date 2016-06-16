/**
 * Gaea
 * Copyright (C) 2016 David Jolly
 * ----------------------
 *
 * Gaea is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gaea is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/gaea.h"

#define INSTANCE_DIM glm::uvec2(1024, 768)

int 
main(void)
{
	int result = 0;
	std::string title;

	title = STRING_CHECK(gaea::version(true));
	std::cout << title << std::endl;

	try {
		gaea::manager &instance = gaea::manager::acquire();
		instance.initialize();
		instance.start(title, INSTANCE_DIM);
		instance.stop();
		instance.uninitialize();
	} catch(gaea::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
