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

#ifndef GAEA_SURFACE_TYPE_H_
#define GAEA_SURFACE_TYPE_H_

namespace gaea {

	namespace engine {

		namespace surface {

			#define GAEA_SURFACE_HEADER "[GAEA::ENGINE::SURFACE]"
#ifndef NDEBUG
			#define GAEA_SURFACE_EXCEPTION_HEADER GAEA_SURFACE_HEADER " "
#else
			#define GAEA_SURFACE_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_SURFACE_EXCEPTION_NONE = 0,
			};

			#define GAEA_SURFACE_EXCEPTION_MAX GAEA_SURFACE_EXCEPTION_NONE

			static const std::string GAEA_SURFACE_EXCEPTION_STR[] = {
				"",
				};

			#define GAEA_SURFACE_EXCEPTION_STRING(_TYPE_) \
				((_TYPE_) > GAEA_SURFACE_EXCEPTION_MAX ? GAEA_SURFACE_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_SURFACE_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_SURFACE_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_SURFACE_EXCEPTION_STRING(_EXCEPT_))
			#define THROW_GAEA_SURFACE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THORW_EXCEPTION_FORMAT(GAEA_SURFACE_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_SURFACE_TYPE_H_
