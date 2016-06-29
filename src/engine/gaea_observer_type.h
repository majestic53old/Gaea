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

#ifndef GAEA_OBSERVER_TYPE_H_
#define GAEA_OBSERVER_TYPE_H_

namespace gaea {

	namespace engine {

		namespace observer {

			#define GAEA_OBSERVER_HEADER "[GAEA::ENGINE::OBSERVER]"
#ifndef NDEBUG
			#define GAEA_OBSERVER_EXCEPTION_HEADER GAEA_OBSERVER_HEADER " "
#else
			#define GAEA_OBSERVER_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				GAEA_OBSERVER_EXCEPTION_HANDLER_NOT_FOUND = 0,
				GAEA_OBSERVER_EXCEPTION_INVALID_HANDLER,
				GAEA_OBSERVER_EXCEPTION_INVALID_CONTEXT,
				GAEA_OBSERVER_EXCEPTION_INVALID_EVENT,
				GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
				GAEA_OBSERVER_EXCEPTION_MISSING_KEY,
				GAEA_OBSERVER_EXCEPTION_REGISTERED,
			};

			#define GAEA_OBSERVER_EXCEPTION_MAX GAEA_OBSERVER_EXCEPTION_REGISTERED

			static const std::string GAEA_OBSERVER_EXCEPTION_STR[] = {
				GAEA_OBSERVER_EXCEPTION_HEADER "Event handler does not exist",
				GAEA_OBSERVER_EXCEPTION_HEADER "Invalid event handler",
				GAEA_OBSERVER_EXCEPTION_HEADER "Invalid context",
				GAEA_OBSERVER_EXCEPTION_HEADER "Invalid event context",
				GAEA_OBSERVER_EXCEPTION_HEADER "Invalid event type",
				GAEA_OBSERVER_EXCEPTION_HEADER "Missing key mapping",
				GAEA_OBSERVER_EXCEPTION_HEADER "Event handler is registered",
				};

			#define GAEA_OBSERVER_EXCEPTION_STRING(_TYPE_) \
				((_TYPE_) > GAEA_OBSERVER_EXCEPTION_MAX ? GAEA_OBSERVER_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
				STRING_CHECK(GAEA_OBSERVER_EXCEPTION_STR[_TYPE_]))

			#define THROW_GAEA_OBSERVER_EXCEPTION(_EXCEPT_) \
				THROW_EXCEPTION(GAEA_OBSERVER_EXCEPTION_STRING(_EXCEPT_))
			#define THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // GAEA_OBSERVER_TYPE_H_
