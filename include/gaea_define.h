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

#ifndef GAEA_DEFINE_H_
#define GAEA_DEFINE_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace gaea {

	#ifndef __in
	#define __in
	#endif // __in
	#ifndef __in_opt
	#define __in_opt
	#endif // __in_opt
	#ifndef __inout
	#define __inout
	#endif // __inout
	#ifndef __inout_opt
	#define __inout_opt
	#endif // __inout_opt
	#ifndef __out
	#define __out
	#endif // __out
	#ifndef __out_opt
	#define __out_opt
	#endif // __out_opt

#ifndef NDEBUG
	#define _GL_ERROR_CHECK { \
		GLenum result = glGetError(); \
		while(result != GL_NO_ERROR) { \
			std::cerr << STRING_CONCAT(_STR_) << ": " << GL_ERROR_STRING(result) \
				<< " (" << __FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << ")" \
				<< std::endl; \
			result = glGetError(); \
		} \
		}
	#define GL_CHECK(_CMD_, ...) _CMD_(__VA_ARGS__); _GL_ERROR_CHECK
	#define GL_CHECK_RESULT(_RSP_, _CMD_, ...) (_RSP_) = GL_CHECK(_CMD_, __VA_ARGS__); _GL_ERROR_CHECK
#else
	#define GL_CHECK(_CMD_, ...) _CMD_(__VA_ARGS__)
	#define GL_CHECK_RESULT(_RSP_, _CMD_, ...) (_RSP_) = GL_CHECK(_CMD_, __VA_ARGS__)
#endif // NDEBUG

	#define GL_ERROR_CLEAR while(glGetError() != GL_NO_ERROR)
	#define GL_ERROR_STRING(_ERR_) gluErrorString(_ERR_)
	#define GL_ERROR_STRING__LAST GL_ERROR_STRING(glGetError())

	#define OBJECT_COUNT 1

	#define REFERENCE_INIT 1

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define STRING_EMPTY "EMPTY"
	#define STRING_UNKNOWN "UNKNOWN"

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define _STRING_CONCAT(_STR_) # _STR_
	#define STRING_CONCAT(_STR_) _STRING_CONCAT(_STR_)

	#define TUPLE_ENTRY(_TUP_, _IDX_) std::get<_IDX_>(_TUP_)

	#define VERSION_NAME "gaea"
	#define VERSION_MAJOR 0
	#define VERSION_MILESTONE "alpha"
	#define VERSION_MINOR 1
	#define VERSION_REVISION 5
	#define VERSION_WEEK 1625
}

#endif // GAEA_DEFINE_H_
