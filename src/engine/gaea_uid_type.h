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

#ifndef GAEA_UID_TYPE_H_
#define GAEA_UID_TYPE_H_

namespace gaea {

	namespace uid {

		#define GAEA_UID_HEADER "[GAEA::UID]"
#ifndef NDEBUG
		#define GAEA_UID_EXCEPTION_HEADER GAEA_UID_HEADER " "
#else
		#define GAEA_UID_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			GAEA_UID_EXCEPTION_ALLOCATED = 0,
			GAEA_UID_EXCEPTION_DUPLICATE,
			GAEA_UID_EXCEPTION_FULL,
			GAEA_UID_EXCEPTION_INITIALIZED,
			GAEA_UID_EXCEPTION_NOT_FOUND,
			GAEA_UID_EXCEPTION_UNINITIALIZED,
		};

		#define GAEA_UID_EXCEPTION_MAX GAEA_UID_EXCEPTION_UNINITIALIZED

		static const std::string GAEA_UID_EXCEPTION_STR[] = {
			GAEA_UID_EXCEPTION_HEADER "Failed to allocate uid manager",
			GAEA_UID_EXCEPTION_HEADER "Duplicate uid",
			GAEA_UID_EXCEPTION_HEADER "Uid manager is full",
			GAEA_UID_EXCEPTION_HEADER "Uid manager is initialized",
			GAEA_UID_EXCEPTION_HEADER "Uid does not exist",
			GAEA_UID_EXCEPTION_HEADER "Uid manager is uninitialized",
			};

		#define GAEA_UID_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > GAEA_UID_EXCEPTION_MAX ? GAEA_UID_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(GAEA_UID_EXCEPTION_STR[_TYPE_]))

		#define THROW_GAEA_UID_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(GAEA_UID_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_GAEA_UID_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(GAEA_UID_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // GAEA_UID_TYPE_H_
