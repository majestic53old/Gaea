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
#include "gaea_observer_type.h"

namespace gaea {

	namespace engine {

		namespace observer {

			_base::_base(void)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				)
			{
				return;
			}

			_base::~_base(void)
			{
				unregister_all_handlers();
			}

			_base &
			_base::operator=(
				__in const _base &other
				)
			{

				if(this != &other) {
					unregister_all_handlers();
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
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::const_iterator iter;

				result << "OBS[" << object.m_handler.size() << "]";

				if(!object.m_handler.empty()) {
					result << "={";

					for(iter = object.m_handler.begin(); iter != object.m_handler.end(); ++iter) {

						if(iter != object.m_handler.begin()) {
							result << "; ";
						}

						result << EVENT_STRING(iter->first)
							<< " (" << SCALAR_AS_HEX(uintptr_t, iter->second.first)
							<< ", CONT=" << SCALAR_AS_HEX(uintptr_t, iter->second.second) << ")";
					}

					result << "}";
				}

				return result.str();
			}

			std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator 
			_base::find(
				__in gaea::event_t type
				)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator result;

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				result = m_handler.find(type);
				if(result == m_handler.end()) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_HANDLER_NOT_FOUND,
						"%x", type);
				}

				return result;
			}

			bool 
			_base::is_handler_registered(
				__in gaea::event_t type
				)
			{

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				return (m_handler.find(type) != m_handler.end());
			}

			void 
			_base::register_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				if(!handler) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_HANDLER,
						"%p", handler);
				}

				if(m_handler.find(type) != m_handler.end()) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_REGISTERED,
						"[%s (%x)] %p(%p)", EVENT_STRING(type), type, handler, context);
				}

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& !instance.contains_handler(handler, type, context)) {
						instance.register_handler(handler, type, context);
					}
				}

				m_handler.insert(std::pair<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>(
					type, std::pair<gaea::engine::event::handler_cb, void *>(handler, context)));
			}

			size_t 
			_base::size(void)
			{
				return m_handler.size();
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::observer::base::as_string(*this, verbose);
			}

			void 
			_base::unregister_all_handlers(void)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized()) {

						for(iter = m_handler.begin(); iter != m_handler.end(); ++iter) {

							if(!instance.contains_handler(iter->second.first, iter->first, iter->second.second)) {
								continue;
							}

							instance.unregister_handler(iter->second.first, iter->first, iter->second.second);
						}
					}
				}

				m_handler.clear();
			}

			void 
			_base::unregister_handler(
				__in gaea::event_t type
				)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				iter = find(type);

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& instance.contains_handler(iter->second.first, type, iter->second.second)) {
						instance.unregister_handler(iter->second.first, type, iter->second.second);
					}
				}

				m_handler.erase(iter);
			}
		}
	}
}
