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

			#define EVENT_STRING(_TYPE_) \
				((_TYPE_) > EVENT_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(EVENT_STR[_TYPE_]))

			#define EVENT_INPUT_STRING(_TYPE_) \
				((_TYPE_) > EVENT_INPUT_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(EVENT_INPUT_STR[_TYPE_]))

			static const std::string EVENT_STR[] = {
				"UNDEFINED", "INPUT",
				};

			static const std::string EVENT_INPUT_STR[] = {
				"BUTTON", "KEY", "MOTION", "WHEEL",
				};

			void 
			signal(
				__in gaea::event_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt size_t length
				)
			{
				gaea::engine::event::base(type, specifier, context, length);
			}

			_base::_base(
				__in gaea::event_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt size_t length
				) :
					gaea::engine::object::base(OBJECT_EVENT, type),
					m_specifier(specifier)
			{

				if(type > EVENT_MAX) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				copy(context, length);
				generate();
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

					switch((gaea::event_t) object.m_subtype) {
						case EVENT_UNDEFINED:
							result << SCALAR_AS_HEX(uint32_t, object.m_specifier);
							break;
						case EVENT_INPUT:
							result << EVENT_INPUT_STRING(object.m_specifier);
							break;
						default:
							result << STRING_UNKNOWN << " (" << SCALAR_AS_HEX(uint32_t, object.m_specifier) << ")";
							break;
					}
				}

				result << "] CNT[" << object.m_context.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &object.m_context[0]);

				return result.str();
			}

			const void *
			_base::context(void)
			{
				return &m_context[0];
			}

			void 
			_base::copy(
				__in void *context,
				__in size_t length
				)
			{
				m_context.clear();

				if((!context && length) || (context && !length)) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_CONTEXT,
						"[%u]=%p", length, context);
				}

				if(context && length) {
					m_context.resize(length, 0);
				}
			}

			void 
			_base::decrement_reference(void)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() && instance.contains(m_id, (gaea::event_t) m_subtype)) {
						instance.decrement_reference(m_id, (gaea::event_t) m_subtype);
					}
				}
			}

			void 
			_base::generate(void)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized()) {
						instance.generate(*this);
					}
				}
			}

			void 
			_base::increment_reference(void)
			{

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() && instance.contains(m_id, (gaea::event_t) m_subtype)) {
						instance.increment_reference(m_id, (gaea::event_t) m_subtype);
					}
				}
			}

			size_t 
			_base::length(void)
			{
				return m_context.size();
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
				size_t iter;
				std::set<gaea::engine::event::handler_cb>::iterator handler_iter;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator entry_iter;

				if(gaea::engine::event::manager::m_instance) {

					gaea::engine::event::manager &instance = *gaea::engine::event::manager::m_instance;
					while(instance.is_initialized()) {
						instance.m_signal.wait();

						if(!instance.is_initialized()) {
							break;
						}

						for(iter = 0; iter <= EVENT_MAX; ++iter) {
							std::queue<gaea::uid_t> &entry_queue = instance.m_entry_queue.at(iter);
							std::set<gaea::engine::event::handler_cb> &handler_set = instance.m_handler.at(iter);

							while(!entry_queue.empty()) {
								entry_iter = instance.find(entry_queue.front(), (gaea::event_t) iter);

								for(handler_iter = handler_set.begin(); handler_iter != handler_set.end();
										++handler_iter) {

									if(!*handler_iter) {
										THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_HANDLER, 
											"%p", *handler_iter);
									}

									(*handler_iter)(entry_iter->second.first);
								}

								instance.decrement_reference(entry_queue.front(), (gaea::event_t) iter);
								entry_queue.pop();
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
				m_entry.clear();
				m_entry_queue.clear();
				m_handler.clear();
			}

			bool 
			_manager::contains(
				__in gaea::uid_t &id,
				__in gaea::event_t type
				)
			{

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				return (m_entry.at(type).find(id) != m_entry.at(type).end());
			}

			bool 
			_manager::contains(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type
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

				return (m_handler.at(type).find(handler) != m_handler.at(type).end());
			}

			size_t 
			_manager::decrement_reference(
				__in gaea::uid_t &id,
				__in gaea::event_t type
				)
			{
				size_t result = 0;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator iter;

				iter = find(id, type);
				if(iter->second.second <= REFERENCE_INIT) {
					m_entry.at(type).erase(iter);
				} else {
					result = --iter->second.second;
				}

				return result;
			}

			std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator 
			_manager::find(
				__in gaea::uid_t &id,
				__in gaea::event_t type
				)
			{
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator result;

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				result = m_entry.at(type).find(id);
				if(result == m_entry.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_EVENT_NOT_FOUND,
						"%x", id);
				}

				return result;
			}

			std::set<gaea::engine::event::handler_cb>::iterator 
			_manager::find(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type
				)
			{
				std::set<gaea::engine::event::handler_cb>::iterator result;

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

				result = m_handler.at(type).find(handler);
				if(result == m_handler.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_HANDLER_NOT_FOUND,
						"%p", handler);
				}

				return result;
			}

			void 
			_manager::generate(
				__in gaea::engine::event::base &object
				)
			{
				gaea::uid_t id;
				gaea::event_t type;

				if(!m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_UNINITIALIZED);
				}

				type = object.type();
				if((type >= m_entry.size()) && (type >= m_entry_queue.size())) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_INVALID_TYPE,
						"%x", type);
				}

				id = object.id();

				if(m_entry.at(type).find(id) != m_entry.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_DUPLICATE_EVENT,
						"%x", id);
				}

				m_entry.at(type).insert(std::pair<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>(
					id, std::pair<gaea::engine::event::base, size_t>(object, REFERENCE_INIT + 1)));
				m_entry_queue.at(type).push(id);
				m_signal.signal();
			}

			size_t 
			_manager::increment_reference(
				__in gaea::uid_t &id,
				__in gaea::event_t type
				)
			{
				return ++find(id, type)->second.second;
			}

			void 
			_manager::initialize(void)
			{

				if(m_initialized) {
					THROW_GAEA_EVENT_EXCEPTION(GAEA_EVENT_EXCEPTION_INITIALIZED);
				}

				m_entry.resize(EVENT_MAX + 1, 
					std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>());
				m_entry_queue.resize(EVENT_MAX + 1, std::queue<gaea::uid_t>());
				m_handler.resize(EVENT_MAX + 1, std::set<gaea::engine::event::handler_cb>());
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
				__in gaea::uid_t &id,
				__in gaea::event_t type
				)
			{
				return find(id, type)->second.second;
			}

			void 
			_manager::register_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type
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

				if(m_handler.at(type).find(handler) != m_handler.at(type).end()) {
					THROW_GAEA_EVENT_EXCEPTION_FORMAT(GAEA_EVENT_EXCEPTION_DUPLICATE_HANDLER,
						"%p", handler);
				}

				m_handler.at(type).insert(handler);
			}

			std::string 
			_manager::to_string(
				__in_opt bool verbose
				)
			{
				size_t count, iter = 0;
				std::stringstream result;
				std::set<gaea::engine::event::handler_cb>::iterator handler_iter;
				std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator entry_iter;

				result << GAEA_EVENT_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

				if(m_initialized) {
					result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

					if(verbose) {

						for(; iter <= EVENT_MAX; ++iter) {
							result << std::endl << EVENT_STRING(iter) << "[" << m_entry.at(iter).size()
								<< ", DEP=" << m_entry_queue.at(iter).size() << "]";

							for(count = 0, entry_iter = m_entry.at(iter).begin(); 
									entry_iter != m_entry.at(iter).end(); ++count, ++entry_iter) {
								result << std::endl << "[" << count << "] " 
									<< gaea::engine::event::base::as_string(entry_iter->second.first, verbose)
									<< ", REF=" << entry_iter->second.second;
							}

							result << std::endl << "HDL[" << m_handler.at(iter).size() << "]";

							for(count = 0, handler_iter = m_handler.at(iter).begin(); 
									handler_iter != m_handler.at(iter).end(); ++count, ++handler_iter) {
								result << std::endl << "[" << count << "] " 
									<< SCALAR_AS_HEX(uintptr_t, *handler_iter);
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
					m_signal.signal();

					if(m_thread.joinable()) {
						m_thread.join();
					}

					clear();
				}
			}

			void 
			_manager::unregister_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type
				)
			{
				m_handler.at(type).erase(find(handler, type));
			}
		}
	}
}
