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

#ifndef GAEA_SIGNAL_H_
#define GAEA_SIGNAL_H_

namespace gaea {

	namespace engine {

		namespace signal {

			typedef class _base {

				public:

					_base(
						__in_opt bool notified = false
						);

					_base(
						__in _base &other
						);

					virtual ~_base(void);

					_base &operator=(
						__in _base &other
						);

					static std::string as_string(
						__in const _base &object,
						__in_opt bool verbose = false
						);

					void clear(void);

					bool is_notified(void);

					void notify(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					void wait(void);

				protected:

					std::condition_variable m_condition;

					std::mutex m_mutex;

					bool m_notified;

			} base;
		}
	}
}

#endif // GAEA_SIGNAL_H_
