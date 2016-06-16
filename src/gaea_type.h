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

#ifndef GAEA_TYPE_H_
#define GAEA_TYPE_H_

namespace gaea {

	#define GAEA_HEADER "[GAEA]"
#ifndef NDEBUG
	#define GAEA_EXCEPTION_HEADER GAEA_HEADER " "
#else
	#define GAEA_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		GAEA_EXCEPTION_ALLOCATED = 0,
		GAEA_EXCEPTION_EXTERNAL,
		GAEA_EXCEPTION_INITIALIZED,
		GAEA_EXCEPTION_STARTED,
		GAEA_EXCEPTION_UNINITALIZED,
	};

	#define GAEA_EXCEPTION_MAX GAEA_EXCEPTION_UNINITALIZED

	static const std::string GAEA_EXCEPTION_STR[] = {
		GAEA_EXCEPTION_HEADER "Failed to allocate manager",
		GAEA_EXCEPTION_HEADER "External exception",
		GAEA_EXCEPTION_HEADER "Manager is initialized",
		GAEA_EXCEPTION_HEADER "Manager is started",
		GAEA_EXCEPTION_HEADER "Manager is uninitialized",
		};

	#define GAEA_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > GAEA_EXCEPTION_MAX ? GAEA_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(GAEA_EXCEPTION_STR[_TYPE_]))

	#define THROW_GAEA_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(GAEA_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_GAEA_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(GAEA_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // GAEA_TYPE_H_
