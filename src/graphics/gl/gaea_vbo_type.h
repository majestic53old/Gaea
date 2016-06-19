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

#ifndef GAEA_VBO_TYPE_H_
#define GAEA_VBO_TYPE_H_

namespace gaea {

	namespace graphics {

		namespace gl {

			namespace vbo {

				#define GAEA_VBO_HEADER "[GAEA::GRAPHICS::GL::VBO]"
#ifndef NDEBUG
				#define GAEA_VBO_EXCEPTION_HEADER GAEA_VBO_HEADER " "
#else
				#define GAEA_VBO_EXCEPTION_HEADER
#endif // NDEBUG

				enum {
					GAEA_VBO_EXCEPTION_NONE = 0,
				};

				#define GAEA_VBO_EXCEPTION_MAX GAEA_VBO_EXCEPTION_NONE

				static const std::string GAEA_VBO_EXCEPTION_STR[] = {
					GAEA_VBO_EXCEPTION_HEADER "",
					};

				#define GAEA_VBO_EXCEPTION_STRING(_TYPE_) \
					((_TYPE_) > GAEA_VBO_EXCEPTION_MAX ? GAEA_VBO_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
					STRING_CHECK(GAEA_VBO_EXCEPTION_STR[_TYPE_]))

				#define THROW_GAEA_VBO_EXCEPTION(_EXCEPT_) \
					THROW_EXCEPTION(GAEA_VBO_EXCEPTION_STRING(_EXCEPT_))
				#define THROW_GAEA_VBO_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
					THROW_EXCEPTION_FORMAT(GAEA_VBO_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
			}
		}
	}
}

#endif // GAEA_VBO_TYPE_H_
