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

#ifndef GAEA_GL_TYPE_H_
#define GAEA_GL_TYPE_H_

namespace gaea {

	namespace gl {

		#define GAEA_GL_HEADER "[GAEA::GL]"
#ifndef NDEBUG
		#define GAEA_GL_EXCEPTION_HEADER GAEA_GL_HEADER " "
#else
		#define GAEA_GL_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			GAEA_GL_EXCEPTION_ALLOCATED = 0,
			GAEA_GL_EXCEPTION_DUPLICATE,
			GAEA_GL_EXCEPTION_EXTERNAL,
			GAEA_GL_EXCEPTION_INITIALIZED,
			GAEA_GL_EXCEPTION_INVALID,
			GAEA_GL_EXCEPTION_NOT_FOUND,
			GAEA_GL_EXCEPTION_UNINITIALIZED,
		};

		#define GAEA_GL_EXCEPTION_MAX GAEA_GL_EXCEPTION_UNINITIALIZED

		static const std::string GAEA_GL_EXCEPTION_STR[] = {
			GAEA_GL_EXCEPTION_HEADER "Failed to allocate gl object manager",
			GAEA_GL_EXCEPTION_HEADER "Duplicate gl object",
			GAEA_GL_EXCEPTION_HEADER "External exception",
			GAEA_GL_EXCEPTION_HEADER "GL object manager is initialized",
			GAEA_GL_EXCEPTION_HEADER "Invalid gl object type",
			GAEA_GL_EXCEPTION_HEADER "GL object does not exist",
			GAEA_GL_EXCEPTION_HEADER "GL object manager is uninitialized",
			};

		#define GAEA_GL_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > GAEA_GL_EXCEPTION_MAX ? GAEA_GL_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(GAEA_GL_EXCEPTION_STR[_TYPE_]))

		#define THROW_GAEA_GL_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(GAEA_GL_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_GAEA_GL_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // GAEA_GL_TYPE_H_
