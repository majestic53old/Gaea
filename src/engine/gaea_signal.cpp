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

#include "../../include/gaea.h"
#include "gaea_signal_type.h"

namespace gaea {

	namespace engine {

		namespace signal {

			_base::_base(
				__in_opt bool notified
				) :
					m_notified(notified)
			{
				return;
			}

			_base::_base(
				__in _base &other
				) :
					m_notified(other.m_notified)
			{
				return;
			}

			_base::~_base(void)
			{
				return;
			}

			_base &
			_base::operator=(
				__in _base &other
				)
			{

				if(this != &other) {
					m_notified = other.m_notified;
				}

				return *this;
			}

			std::string 
			_base::as_string(
				__in const _base &object,
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				result << (object.m_notified ? "NOTIFIED" : "CLEAR");

				return result.str();
			}

			void 
			_base::clear(void)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				m_notified = false;
				m_condition.notify_all();
			}

			bool 
			_base::is_notified(void)
			{
				return m_notified;
			}

			void 
			_base::notify(void)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				m_notified = true;
				m_condition.notify_all();
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::signal::base::as_string(*this, verbose);
			}

			void 
			_base::wait(void)
			{
				std::unique_lock<std::mutex> lock(m_mutex);

				m_condition.wait(lock, [this] { return m_notified; });
				m_notified = false;
			}
		}
	}
}
