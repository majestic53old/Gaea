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

#ifndef GAEA_EVENT_TYPE_H_
#define GAEA_EVENT_TYPE_H_

namespace gaea {

	namespace engine {

		namespace event {

			#define GAEA_EVENT_HEADER "[GAEA::ENGINE::EVENT]"
#ifndef NDEBUG
			#define GAEA_EVENT_EXCEPTION_HEADER GAEA_EVENT_HEADER " "
#else
			#define GAEA_EVENT_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_EVENT_EXCEPTION_ALLOCATED = 0,
				GAEA_EVENT_EXCEPTION_DUPLICATE_EVENT,
				GAEA_EVENT_EXCEPTION_DUPLICATE_HANDLER,
				GAEA_EVENT_EXCEPTION_HANDLER_NOT_FOUND,
				GAEA_EVENT_EXCEPTION_INITIALIZED,
				GAEA_EVENT_EXCEPTION_INVALID_CONTEXT,
				GAEA_EVENT_EXCEPTION_INVALID_HANDLER,
				GAEA_EVENT_EXCEPTION_INVALID_TYPE,
				GAEA_EVENT_EXCEPTION_REGISTERED,
				GAEA_EVENT_EXCEPTION_TYPE_NOT_FOUND,
				GAEA_EVENT_EXCEPTION_UNINITIALIZED,
			};

			#define GAEA_EVENT_EXCEPTION_MAX GAEA_EVENT_EXCEPTION_UNINITIALIZED

			static std::string GAEA_EVENT_EXCEPTION_STR[] = {
				GAEA_EVENT_EXCEPTION_HEADER "Failed to allocate event manager",
				GAEA_EVENT_EXCEPTION_HEADER "Duplicate event",
				GAEA_EVENT_EXCEPTION_HEADER "Duplicate event handler",
				GAEA_EVENT_EXCEPTION_HEADER "Event handler does not exist",
				GAEA_EVENT_EXCEPTION_HEADER "Event manager is initialized",
				GAEA_EVENT_EXCEPTION_HEADER "Invalid event context",
				GAEA_EVENT_EXCEPTION_HEADER "Invalid event handler",
				GAEA_EVENT_EXCEPTION_HEADER "Event handler is registered",
				GAEA_EVENT_EXCEPTION_HEADER "Invalid event type",
				GAEA_EVENT_EXCEPTION_HEADER "Event type does not exist",
				GAEA_EVENT_EXCEPTION_HEADER "Event manager is uninitialized",
				};

			#define GAEA_EVENT_EXCEPTION_STRING(_TYPE_) \
				((_TYPE_) > GAEA_EVENT_EXCEPTION_MAX ? GAEA_EVENT_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_EVENT_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_EVENT_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_EVENT_EXCEPTION_STRING(_EXCEPT_))
			#define THROW_GAEA_EVENT_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_EVENT_TYPE_H_
