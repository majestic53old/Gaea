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

#ifndef GAEA_ENTITY_TYPE_H_
#define GAEA_ENTITY_TYPE_H_

namespace gaea {

	namespace engine {

		namespace entity {

			#define GAEA_ENTITY_HEADER "[GAEA::ENGINE::ENTITY]"
#ifndef NDEBUG
			#define GAEA_ENTITY_EXCEPTION_HEADER GAEA_ENTITY_HEADER " "
#else
			#define GAEA_ENTITY_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_ENTITY_EXCEPTION_ALLOCATED = 0,
				GAEA_ENTITY_EXCEPTION_DUPLICATE,
				GAEA_ENTITY_EXCEPTION_INITIALIZED,
				GAEA_ENTITY_EXCEPTION_INVALID,
				GAEA_ENTITY_EXCEPTION_NOT_FOUND,
				GAEA_ENTITY_EXCEPTION_UNINITIALIZED,
			};

			#define GAEA_ENTITY_EXCEPTION_MAX GAEA_ENTITY_EXCEPTION_UNINITIALIZED

			static const std::string GAEA_ENTITY_EXCEPTION_STR[] = {
				GAEA_ENTITY_EXCEPTION_HEADER "Failed to allocate entity manager",
				GAEA_ENTITY_EXCEPTION_HEADER "Duplicate entity",
				GAEA_ENTITY_EXCEPTION_HEADER "Entity manager is initialized",
				GAEA_ENTITY_EXCEPTION_HEADER "Invalid entity type",
				GAEA_ENTITY_EXCEPTION_HEADER "Entity does not exist",
				GAEA_ENTITY_EXCEPTION_HEADER "Entity manager is uninitialized",
				};

			#define GAEA_ENTITY_EXCEPTION_STR(_TYPE_) \
				((_TYPE_) > GAEA_ENTITY_EXCEPTION_MAX ? GAEA_ENTITY_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_ENTITY_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_ENTITY_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_ENTITY_EXCEPTION_STR(_EXCEPT_))
			#define THROW_GAEA_ENTITY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_STR(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_ENTITY_TYPE_H_
