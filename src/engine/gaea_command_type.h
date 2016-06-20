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

#ifndef GAEA_COMMAND_TYPE_H_
#define GAEA_COMMAND_TYPE_H_

namespace gaea {

	namespace engine {

		namespace command {

			#define GAEA_COMMAND_HEADER "[GAEA::ENGINE::COMMAND]"
#ifndef NDEBUG
			#define GAEA_COMMAND_EXCEPTION_HEADER GAEA_COMMAND_HEADER " "
#else
			#define GAEA_COMMAND_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_COMMAND_EXCEPTION_ALLOCATED = 0,
				GAEA_COMMAND_EXCEPTION_DUPLICATE_ENTRY,
				GAEA_COMMAND_EXCEPTION_DUPLICATE_OBSERVER,
				GAEA_COMAMND_EXCEPTION_ENTRY_NOT_FOUND,
				GAEA_COMMAND_EXCEPTION_INITIALIZED,
				GAEA_COMMAND_EXCEPTION_INTERNAL,
				GAEA_COMMAND_EXCEPTION_INVALID,
				GAEA_COMMAND_EXCEPTION_OBSERVER_NOT_FOUND,
				GAEA_COMMAND_EXCEPTION_UNINITIALIZED,
			};

			#define GAEA_COMMAND_EXCEPTION_MAX GAEA_COMMAND_EXCEPTION_UNINITIALIZED

			static const std::string GAEA_COMMAND_EXCEPTION_STR[] = {
				GAEA_COMMAND_EXCEPTION_HEADER "Failed to allocate command manager",
				GAEA_COMMAND_EXCEPTION_HEADER "Duplicate command entry",
				GAEA_COMMAND_EXCEPTION_HEADER "Duplicate command observer",
				GAEA_COMMAND_EXCEPTION_HEADER "Command entry does not exist",
				GAEA_COMMAND_EXCEPTION_HEADER "Command manager is initialized",
				GAEA_COMMAND_EXCEPTION_HEADER "Internal exception",
				GAEA_COMMAND_EXCEPTION_HEADER "Invalid command type",
				GAEA_COMMAND_EXCEPTION_HEADER "Command observer does not exist",
				GAEA_COMMAND_EXCEPTION_HEADER "Command manager is uninitialized"
				};

			#define GAEA_COMMAND_EXCEPTION_STRING(_TYPE_) \
				((_TYPE_) > GAEA_COMMAND_EXCEPTION_MAX ? GAEA_COMMAND_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_COMMAND_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_COMMAND_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_COMMAND_EXCEPTION_STRING(_EXCEPT_))
			#define THROW_GAEA_COMMAND_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_COMMAND_TYPE_H_
