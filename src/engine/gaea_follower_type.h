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

#ifndef GAEA_FOLLOWER_TYPE_H_
#define GAEA_FOLLOWER_TYPE_H_

namespace gaea {

	namespace engine {

		namespace follower {

			#define GAEA_FOLLOWER_HEADER "[GAEA:ENGINE::FOLLOWER]"
#ifndef NDEBUG
			#define GAEA_FOLLOWER_EXCEPTION_HEADER GAEA_FOLLOWER_HEADER " "
#else
			#define GAEA_FOLLOWER_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_FOLLOWER_EXCEPTION_INVALID_CONTEXT = 0,
				GAEA_FOLLOWER_EXCEPTION_INVALID_EVENT,
				GAEA_FOLLOWER_EXCEPTION_INVALID_TYPE,
			};

			#define GAEA_FOLLOWER_EXCEPTION_MAX GAEA_FOLLOWER_EXCEPTION_INVALID_TYPE

			static const std::string GAEA_FOLLOWER_EXCEPTION_STR[] = {
				GAEA_FOLLOWER_EXCEPTION_HEADER "Invalid follower context",
				GAEA_FOLLOWER_EXCEPTION_HEADER "Invalid follower event context",
				GAEA_FOLLOWER_EXCEPTION_HEADER "Invalid follower type",
				};

			#define GAEA_FOLLOWER_EXCEPTION_STRING(_TYPE_) \
				((_TYPE_) > GAEA_FOLLOWER_EXCEPTION_MAX ? GAEA_FOLLOWER_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_FOLLOWER_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_FOLLOWER_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_FOLLOWER_EXCEPTION_STRING(_EXCEPT_))
			#define THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_FOLLOWER_TYPE_H_
