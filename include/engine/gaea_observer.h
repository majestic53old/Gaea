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

#ifndef GAEA_OBSERVER_H_
#define GAEA_OBSERVER_H_

namespace gaea {

	namespace engine {

		namespace observer {

			typedef class _base {

				public:

					_base(void);

					_base(
						__in const _base &other
						);

					virtual ~_base(void);

					_base &operator=(
						__in const _base &other
						);

					static std::string as_string(
						__in const _base &object,
						__in_opt bool verbose = false
						);

					bool is_handler_registered(
						__in gaea::event_t type
						);

					void register_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

					size_t size(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					void unregister_all_handlers(void);

					void unregister_handler(
						__in gaea::event_t type
						);

				protected:

					std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator find(
						__in gaea::event_t type
						);

					std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>> m_handler;

			} base;
		}
	}
}

#endif // GAEA_OBSERVER_H_
