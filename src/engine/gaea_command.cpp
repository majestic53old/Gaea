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
#include "gaea_command_type.h"

namespace gaea {

	namespace engine {

		namespace command {

			static const std::string COMMAND_STR[] = {
				"USER", "KEYBOARD", "MOUSE_BUTTON", "MOUSE_MOTION", "MOUSE_WHEEL",
				};

			#define COMMAND_STRING(_TYPE_) \
				((_TYPE_) > COMMAND_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(COMMAND_STR[_TYPE_]))

			enum {
				COMMAND_TUPLE_SPECIFIER = 0,
				COMMAND_TUPLE_CONTEXT,
				COMMAND_TUPLE_LENGTH,
				COMMAND_TUPLE_REFERENCE,
			};

			void 
			notify(
				__in gaea::cmd_type_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt uint32_t length
				)
			{
				gaea::engine::command::base entry(type, specifier, context, length);
			}

			_base::_base(
				__in gaea::cmd_type_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt uint32_t length
				) :
					gaea::engine::object::base(OBJECT_COMMAND, type),
					m_context(context),
					m_length(length),
					m_specifier(specifier)
			{
				generate();
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::object::base(other),
					m_context(other.m_context),
					m_length(other.m_length),
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
					m_length = other.m_length;
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

				result << gaea::engine::object::base::as_string(object, verbose)
					<< " [" << COMMAND_STRING(object.m_subtype);

				if(object.m_specifier != COMMAND_SPECIFIER_UNDEFINED) {
					result << ", " << SCALAR_AS_HEX(uint32_t, object.m_specifier);;
				}

				result << "]";

				if(object.m_context && object.m_length) {
					result<< " CTX=" << SCALAR_AS_HEX(uintptr_t, object.m_context)
						<< ", LEN=" << object.m_length;
				}

				return result.str();
			}

			void *
			_base::context(void)
			{
				return m_context;
			}

			void 
			_base::decrement_reference(void)
			{

				if(gaea::engine::command::manager::is_allocated()) {

					gaea::engine::command::manager &instance = gaea::engine::command::manager::acquire();
					if(instance.is_initialized() && instance.contains(m_id, (gaea::cmd_type_t) m_subtype)) {
						instance.decrement_reference(m_id, (gaea::cmd_type_t) m_subtype);
					}
				}
			}

			void 
			_base::generate(void)
			{

				if(gaea::engine::command::manager::is_allocated()) {

					gaea::engine::command::manager &instance = gaea::engine::command::manager::acquire();
					if(instance.is_initialized()) {
						m_context = instance.generate(m_id, (gaea::cmd_type_t) m_subtype, m_specifier,
							m_context, m_length);
					}
				}
			}

			void 
			_base::increment_reference(void)
			{

				if(gaea::engine::command::manager::is_allocated()) {

					gaea::engine::command::manager &instance = gaea::engine::command::manager::acquire();
					if(instance.is_initialized() && instance.contains(m_id, (gaea::cmd_type_t) m_subtype)) {
						instance.increment_reference(m_id, (gaea::cmd_type_t) m_subtype);
					}
				}
			}

			uint32_t 
			_base::length(void)
			{
				return m_length;
			}

			uint32_t 
			_base::specifier(void)
			{
				return m_specifier;
			}

			gaea::cmd_type_t 
			_base::type(void)
			{
				return (gaea::cmd_type_t) m_subtype;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::command::base::as_string(*this, verbose);
			}

			_manager *_manager::m_instance = nullptr;

			_manager::_manager(void) :
				m_initialized(false)
			{
				std::atexit(gaea::engine::command::manager::_delete);
			}

			_manager::~_manager(void)
			{
				uninitialize();
			}

			void 
			_manager::_delete(void)
			{

				if(gaea::engine::command::manager::m_instance) {
					delete gaea::engine::command::manager::m_instance;
					gaea::engine::command::manager::m_instance = nullptr;
				}
			}

			_manager &
			_manager::acquire(void)
			{

				if(!gaea::engine::command::manager::m_instance) {

					gaea::engine::command::manager::m_instance = new gaea::engine::command::manager;
					if(!gaea::engine::command::manager::m_instance) {
						THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_ALLOCATED);
					}
				}

				return *gaea::engine::command::manager::m_instance;
			}

			void 
			_manager::clear(void)
			{
				std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator entry_iter;
				std::vector<std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>>::iterator type_iter;

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				for(type_iter = m_entry.begin(); type_iter != m_entry.end(); ++type_iter) {

					for(entry_iter = type_iter->begin(); entry_iter != type_iter->end(); ++entry_iter) {
						destroy(entry_iter);
					}

					type_iter->clear();
				}

				m_entry.clear();
				m_entry_queue.clear();
				m_observer.clear();
			}

			bool 
			_manager::contains(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type
				)
			{
				std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator iter;

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INVALID,
						"%x", type);
				}

				return (m_entry.at(type).find(id) != m_entry.at(type).end());
			}

			size_t 
			_manager::decrement_reference(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type
				)
			{
				size_t result = 0;
				std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator iter;

				iter = find(id, type);
				if(TUPLE_ENTRY(iter->second, COMMAND_TUPLE_REFERENCE) <= REFERENCE_INIT) {
					destroy(iter);
					m_entry.at(type).erase(iter);
				} else {
					result = --TUPLE_ENTRY(iter->second, COMMAND_TUPLE_REFERENCE);
				}

				return result;
			}

			void 
			_manager::destroy(
				__in const std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator &entry
				)
			{
				uint8_t *data = nullptr;

				data = (uint8_t *) TUPLE_ENTRY(entry->second, COMMAND_TUPLE_CONTEXT);
				if(data) {
					delete[] data;
					data = nullptr;
				}
			}

			std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator 
			_manager::find(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type
				)
			{
				std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator result;

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INVALID,
						"%x", type);
				}

				result = m_entry.at(type).find(id);
				if(result == m_entry.at(type).end()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMAMND_EXCEPTION_ENTRY_NOT_FOUND,
						"%x", id);
				}

				return result;
			}

			void *
			_manager::generate(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type,
				__in_opt uint32_t specifier,
				__in_opt void *context,
				__in_opt uint32_t length
				)
			{
				uint8_t *result = nullptr;

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				if((type >= m_entry.size()) || (type >= m_observer.size())) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INVALID,
						"%x", type);
				}

				if(m_entry.at(type).find(id) != m_entry.at(type).end()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_DUPLICATE_ENTRY,
						"%x", type);
				}

				if(context && length && !m_observer.at(type).empty()) {

					result = new uint8_t[length];
					if(!result) {
						THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INTERNAL,
							"new failed: %p", result);
					}

					std::memcpy(result, context, length);
					m_entry.at(type).insert(std::pair<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>(
						id, std::make_tuple(specifier, result, result ? length : 0, REFERENCE_INIT)));
				}

				return result;
			}

			size_t 
			_manager::increment_reference(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type
				)
			{
				return ++TUPLE_ENTRY(find(id, type)->second, COMMAND_TUPLE_REFERENCE);
			}

			void 
			_manager::initialize(void)
			{

				if(m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_INITIALIZED);
				}

				m_entry.resize(COMMAND_MAX + 1, std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>());
				m_entry_queue.resize(COMMAND_MAX + 1, std::queue<gaea::uid_t>());
				m_observer.resize(COMMAND_MAX + 1, std::set<gaea::engine::command::observer *>());
				m_initialized = true;
			}

			bool 
			_manager::is_allocated(void)
			{
				return (gaea::engine::command::manager::m_instance != nullptr);
			}

			bool 
			_manager::is_initialized(void)
			{
				return m_initialized;
			}

			size_t 
			_manager::reference_count(
				__in const gaea::uid_t &id,
				__in gaea::cmd_type_t type
				)
			{
				return TUPLE_ENTRY(find(id, type)->second, COMMAND_TUPLE_REFERENCE);
			}

			void 
			_manager::register_observer(
				__in gaea::engine::command::observer &object,
				__in gaea::cmd_type_t type
				)
			{

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INVALID,
						"%x", type);
				}

				if(m_observer.at(type).find(&object) != m_observer.at(type).end()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_DUPLICATE_OBSERVER,
						"%s", STRING_CHECK(object.to_string(true)));
				}

				m_observer.at(type).insert(&object);
			}

			std::string 
			_manager::to_string(
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				// TODO

				return result.str();
			}

			void 
			_manager::uninitialize(void)
			{

				if(m_initialized) {
					clear();
					m_initialized = false;
				}
			}

			void 
			_manager::unregister_observer(
				__in gaea::engine::command::observer &object,
				__in gaea::cmd_type_t type
				)
			{
				std::set<gaea::engine::command::observer *>::iterator iter;

				if(!m_initialized) {
					THROW_GAEA_COMMAND_EXCEPTION(GAEA_COMMAND_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_INVALID,
						"%x", type);
				}

				iter = m_observer.at(type).find(&object);
				if(iter != m_observer.at(type).end()) {
					THROW_GAEA_COMMAND_EXCEPTION_FORMAT(GAEA_COMMAND_EXCEPTION_OBSERVER_NOT_FOUND,
						"%s", STRING_CHECK(object.to_string(true)));
				}

				m_observer.at(type).erase(iter);
			}

			void 
			_manager::update(
				__in GLfloat delta
				)
			{
				// TODO: for each type:
					// for each registered observer:
						// flush registered observers queue (observer decrements any uid it contains)
						// for each entry in entry_queue:
							// pend entry into registered observer (w/ uid)

				// TODO: when pended entry arrives in observers queue, increment reference (using supplied uid)
			}
		}
	}
}
