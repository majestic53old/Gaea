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
#include "gaea_event_type.h"

namespace gaea {

	namespace engine {

		namespace event {

			#define REFERENCE_OFFSET 3

			#define EVENT_CAMERA_STRING(_TYPE_) \
				((_TYPE_) > EVENT_CAMERA_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(EVENT_CAMERA_STR[_TYPE_]))

			#define EVENT_INPUT_STRING(_TYPE_) \
				((_TYPE_) > EVENT_INPUT_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(EVENT_INPUT_STR[_TYPE_]))

			static const std::string EVENT_CAMERA_STR[] = {
				"CLIP", "DIMENSIONS", "DELTA FOV", "FOV", "DELTA POSITION", "POSITION",
				"DELTA ROTATION", "ROTATION", "DELTA UP", "UP",
				};

			static const std::string EVENT_INPUT_STR[] = {
				"BUTTON", "KEY", "MOTION", "WHEEL",
				};

			void 
			notify(
				__in gaea::event_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt size_t length
				)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized()) {
						instance.generate(type, specifier, context, length);
					}
				}
			}

			_base::_base(
				__in gaea::event_t type,
				__in uint32_t specifier,
				__in std::vector<uint8_t> &context
				) :
					gaea::engine::object::base(OBJECT_EVENT, type),
					m_context(context),
					m_specifier(specifier)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::object::base(other),
					m_context(other.m_context),
					m_specifier(other.m_specifier)
			{
				increment_reference();
			}

			_base::~_base(void)
			{
				decrement_reference();
			}

			_base &
			_base::operator=(
				__in const _base &other
				)
			{

				if(this != &other) {
					decrement_reference();
					gaea::engine::object::base::operator=(other);
					m_context = other.m_context;
					m_specifier = other.m_specifier;
					increment_reference();
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

				result << gaea::engine::uid::base::as_string(object, verbose)
					<< " [" << EVENT_STRING((gaea::event_t) object.m_subtype);

				if(object.m_specifier != EVENT_SPECIFIER_UNDEFINED) {
					result << ", ";

					switch(object.m_subtype) {
						case EVENT_CAMERA:
							result << EVENT_CAMERA_STRING(object.m_specifier);
							break;
						case EVENT_INPUT:
							result << EVENT_INPUT_STRING(object.m_specifier);
							break;
						default:
							result << SCALAR_AS_HEX(uint32_t, object.m_specifier);
							break;
					}
				}

				result << "] CTX=" << SCALAR_AS_HEX(uintptr_t, &object.m_context[0])
					<< ", LEN=" << object.m_context.size();

				return result.str();
			}

			const void *
			_base::context(void)
			{
				return &m_context[0];
			}

			void 
			_base::decrement_reference(void)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& instance.contains_event(m_id, (gaea::event_t) m_subtype)) {
						instance.decrement_reference(m_id, (gaea::event_t) m_subtype);
					}
				}
			}

			void 
			_base::increment_reference(void)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& instance.contains_event(m_id, (gaea::event_t) m_subtype)) {
						instance.increment_reference(m_id, (gaea::event_t) m_subtype);
					}
				}
			}

			size_t 
			_base::length(void)
			{
				return m_context.size();
			}

			void 
			_base::set(
				__in std::vector<uint8_t> &context
				)
			{
				m_context = context;
			}

			uint32_t 
			_base::specifier(void)
			{
				return m_specifier;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::event::base::as_string(*this, verbose);
			}

			gaea::event_t 
			_base::type(void)
			{
				return (gaea::event_t) m_subtype;
			}

			_manager *_manager::m_instance = nullptr;

			_manager::_manager(void) :
				m_initialized(false)
			{
				std::atexit(gaea::engine::event::manager::_delete);
			}

			_manager::~_manager(void)
			{
				uninitialize();
			}

			void 
			_manager::_delete(void)
			{

				if(gaea::engine::event::manager::m_instance) {
					delete gaea::engine::event::manager::m_instance;
					gaea::engine::event::manager::m_instance = nullptr;
				}
			}

			void 
			_manager::_thread(void)
			{
				size_t iter, count[EVENT_MAX + 1], total;
				std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator handler_iter;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator event_iter;

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					while(instance.is_initialized()) {
						instance.m_signal.wait();

						if(!instance.is_initialized()) {
							break;
						}

						while(instance.is_initialized()) {
							total = 0;

							for(iter = 0; iter <= EVENT_MAX; ++iter) {
								count[iter] = instance.m_event_queue.at(iter).size();
								total += count[iter];
							}

							if(!total) {
								break;
							}

							for(iter = 0; iter <= EVENT_MAX; ++iter) {

								if(!count[iter]) {
									continue;
								}

								std::lock_guard<std::recursive_mutex> lock(instance.m_lock);
								event_iter = instance.m_event.at(iter).find(instance.m_event_queue.at(iter).front());

								for(handler_iter = instance.m_handler.at(iter).begin(); 
										handler_iter != instance.m_handler.at(iter).end();
										++handler_iter) {
									(*(handler_iter->first))(event_iter->second.first, handler_iter->second);
								}

								instance.m_event.at(iter).erase(event_iter);
								instance.m_event_queue.at(iter).pop();
							}
						}
					}
				}
			}

			_manager &
			_manager::acquire(void)
			{

				if(!gaea::engine::event::manager::m_instance) {

					gaea::engine::event::manager::m_instance = new gaea::engine::event::manager;
					if(!gaea::engine::event::manager::m_instance) {
						THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_ALLOCATED);
					}
				}

				return *gaea::engine::event::manager::m_instance;
			}

			void 
			_manager::clear(void)
			{
				m_event.clear();
				m_event_queue.clear();
				m_handler.clear();
			}

			bool 
			_manager::contains_event(
				__in gaea::uid_t id,
				__in gaea::event_t type
				)
			{

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				std::lock_guard<std::recursive_mutex> lock(m_lock);

				if(type >= m_event.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				return (m_event.at(type).find(id) != m_event.at(type).end());
			}

			bool 
			_manager::contains_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_handler.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				if(!handler) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_HANDLER,
						"%p", handler);
				}

				return (m_handler.at(type).find(std::pair<gaea::engine::event::handler_cb, void *>(handler, context)) 
						!= m_handler.at(type).end());
			}

			size_t 
			_manager::decrement_reference(
				__in gaea::uid_t id,
				__in gaea::event_t type
				)
			{
				size_t result = 0;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator iter;

				std::lock_guard<std::recursive_mutex> lock(m_lock);

				iter = find_event(id, type);
				if(iter->second.second <= REFERENCE_INIT) {
					m_event.at(type).erase(iter);
				} else {
					result = --iter->second.second;
				}

				return result;
			}

			std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator 
			_manager::find_event(
				__in gaea::uid_t id,
				__in gaea::event_t type
				)
			{
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator result;

				std::lock_guard<std::recursive_mutex> lock(m_lock);

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_event.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				result = m_event.at(type).find(id);
				if(result == m_event.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_TYPE_NOT_FOUND,
						"[%s (%x)] %x", EVENT_STRING(type), type, id);
				}

				return result;
			}

			std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator 
			_manager::find_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{
				std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator result;

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_handler.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				if(!handler) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_HANDLER,
						"%p", handler);
				}

				result = m_handler.at(type).find(std::pair<gaea::engine::event::handler_cb, void *>(handler, context));
				if(result == m_handler.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_HANDLER_NOT_FOUND,
						"[%s (%x)] %p(%p)", EVENT_STRING(type), type, handler, context);
				}

				return result;
			}

			gaea::uid_t 
			_manager::generate(
				__in gaea::event_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt size_t length
				)
			{
				std::vector<uint8_t> data;
				gaea::uid_t result = UID_INVALID;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator iter;

				std::lock_guard<std::recursive_mutex> lock(m_lock);

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_event.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				if(!m_handler.at(type).empty()) {

					if((!context && length) || (context && !length)) {
						THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_CONTEXT,
							"%p[%u]", context, length);
					}

					data = std::vector<uint8_t>((uint8_t *) context, ((uint8_t *) context) + length);
					gaea::engine::event::base entry(type, specifier, data);
					result = entry.id();

					if(m_event.at(type).find(entry.id()) != m_event.at(type).end()) {
						THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_DUPLICATE_EVENT, "{%x}[%s (%x), %x]", 
							result, EVENT_STRING(entry.type()), entry.type(), entry.specifier());
					}

					m_event.at(type).insert(std::pair<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>(result, 
						std::pair<gaea::engine::event::base, size_t>(entry, REFERENCE_INIT + REFERENCE_OFFSET)));
					m_event_queue.at(type).push(result);
					m_signal.notify();
				}

				return result;
			}

			size_t 
			_manager::increment_reference(
				__in gaea::uid_t id,
				__in gaea::event_t type
				)
			{
				return ++find_event(id, type)->second.second;
			}

			void 
			_manager::initialize(void)
			{

				if(m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_INITIALIZED);
				}

				m_event.resize(EVENT_MAX + 1, std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>());
				m_event_queue.resize(EVENT_MAX + 1, std::queue<gaea::uid_t>());
				m_handler.resize(EVENT_MAX + 1, std::set<std::pair<gaea::engine::event::handler_cb, void *>>());
				m_initialized = true;
				m_thread = std::thread(gaea::engine::event::manager::_thread);
			}

			bool 
			_manager::is_allocated(void)
			{
				return (gaea::engine::event::manager::m_instance != nullptr);
			}

			bool 
			_manager::is_initialized(void)
			{
				return m_initialized;
			}

			size_t 
			_manager::reference_count(
				__in gaea::uid_t id,
				__in gaea::event_t type
				)
			{
				return find_event(id, type)->second.second;
			}

			void 
			_manager::register_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{
				std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_handler.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				if(!handler) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_HANDLER,
						"%p", handler);
				}

				iter = m_handler.at(type).find(std::pair<gaea::engine::event::handler_cb, void *>(handler, context));
				if(iter != m_handler.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_DUPLICATE_HANDLER, "[%s (%x)] %p(%p)", 
						EVENT_STRING(type), type, handler, context);
				}

				m_handler.at(type).insert(std::pair<gaea::engine::event::handler_cb, void *>(handler, context));
			}

			std::string 
			_manager::to_string(
				__in_opt bool verbose
				)
			{
				size_t count, iter = 0;
				std::stringstream result;
				std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator handler_iter;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator event_iter;

				std::lock_guard<std::recursive_mutex> lock(m_lock);

				result << GAEA_EVENT_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

				if(m_initialized) {
					result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

					if(verbose) {

						for(; iter <= EVENT_MAX; ++iter) {

							if(m_event.at(iter).empty() && m_handler.at(iter).empty()) {
								continue;
							}

							result << std::endl << "[" << EVENT_STRING(iter) 
								<< " (" << SCALAR_AS_HEX(gaea::event_t, iter) << ")]"
								<< "[EVT=" << m_event.at(iter).size()
								<< ", QUE=" << m_event_queue.at(iter).size()
								<< ", HLD=" << m_handler.at(iter).size() << "]";

							for(count = 0, event_iter = m_event.at(iter).begin(); event_iter != m_event.at(iter).end();
									++count, ++event_iter) {
								result << std::endl << "[" << count << "]" 
									<< " " << event_iter->second.first.to_string(verbose)
									<< ", REF=" << event_iter->second.second;
							}

							for(count = 0, handler_iter = m_handler.at(iter).begin(); 
									handler_iter != m_handler.at(iter).end();
									++count, ++handler_iter) {
								result << std::endl << "[" << count << "]"
									<< " HDL=" << SCALAR_AS_HEX(uintptr_t, handler_iter->first)
									<< ", CTX=" << SCALAR_AS_HEX(uintptr_t, handler_iter->second);
							}
						}
					}
				}

				return result.str();
			}

			void 
			_manager::uninitialize(void)
			{

				if(m_initialized) {
					m_initialized = false;
					m_signal.notify();

					if(m_thread.joinable()) {
						m_thread.join();
					}

					clear();
				}
			}

			void 
			_manager::unregister_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{
				std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_handler.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				m_handler.at(type).erase(find_handler(handler, type, context));
			}
		}
	}
}
