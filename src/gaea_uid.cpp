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

#include "../include/gaea.h"
#include "gaea_uid_type.h"

namespace gaea {

	namespace uid {

		#define UID_INIT 0

		_base::_base(void) :
			m_id(UID_INVALID)
		{
			generate();
		}

		_base::_base(
			__in const _base &other
			) :
				m_id(other.m_id)
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
				m_id = other.m_id;
				increment_reference();
			}

			return *this;
		}

		bool 
		_base::operator==(
			__in const _base &other
			)
		{
			return ((this == &other) || (m_id == other.m_id));
		}

		bool 
		_base::operator!=(
			__in const _base &other
			)
		{
			return ((this != &other) && (m_id != other.m_id));
		}

		std::string 
		_base::as_string(
			__in const _base &object,
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << SCALAR_AS_HEX(gaea::uid_t, object.m_id);

			return result.str();
		}

		void 
		_base::decrement_reference(void)
		{

			if(gaea::uid::manager::is_allocated()) {

				gaea::uid::manager &instance = gaea::uid::manager::acquire();
				if(instance.is_initialized() && instance.contains(m_id)) {
					instance.decrement_reference(m_id);
				}
			}
		}

		void 
		_base::generate(void)
		{

			if(gaea::uid::manager::is_allocated()) {

				gaea::uid::manager &instance = gaea::uid::manager::acquire();
				if(instance.is_initialized()) {
					instance.generate(m_id);
				}
			}
		}

		const gaea::uid_t &
		_base::id(void)
		{
			return m_id;
		}

		void 
		_base::increment_reference(void)
		{

			if(gaea::uid::manager::is_allocated()) {

				gaea::uid::manager &instance = gaea::uid::manager::acquire();
				if(instance.is_initialized() && instance.contains(m_id)) {
					instance.increment_reference(m_id);
				}
			}
		}

		std::string 
		_base::to_string(
			__in_opt bool verbose
			)
		{
			return gaea::uid::base::as_string(*this, verbose);
		}

		bool operator<(
			__in const gaea::uid::base &left,
			__in const gaea::uid::base &right
			)
		{
			return (left.m_id < right.m_id);
		}

		_manager *_manager::m_instance = nullptr;

		_manager::_manager(void) :
			m_initialized(false),
			m_next(UID_INIT)
		{
			std::atexit(gaea::uid::manager::_delete);
		}

		_manager::~_manager(void)
		{
			uninitialize();
		}

		void 
		_manager::_delete(void)
		{

			if(gaea::uid::manager::m_instance) {
				delete gaea::uid::manager::m_instance;
				gaea::uid::manager::m_instance = nullptr;
			}
		}

		_manager &
		_manager::acquire(void)
		{

			if(!gaea::uid::manager::m_instance) {

				gaea::uid::manager::m_instance = new gaea::uid::manager;
				if(!gaea::uid::manager::m_instance) {
					THROW_GAEA_UID_EXCEPTION(GAEA_UID_EXCEPTION_ALLOCATED);
				}
			}

			return *gaea::uid::manager::m_instance;
		}

		void 
		_manager::clear(void)
		{
			m_entry.clear();
			m_next = UID_INIT;
			m_surplus.clear();
		}

		bool 
		_manager::contains(
			__in const gaea::uid_t &id
			)
		{

			if(!m_initialized) {
				THROW_GAEA_UID_EXCEPTION(GAEA_UID_EXCEPTION_UNINITIALIZED);
			}

			return (m_entry.find(id) != m_entry.end());
		}

		size_t 
		_manager::decrement_reference(
			__in const gaea::uid_t &id
			)
		{
			size_t result = 0;
			std::map<gaea::uid_t, size_t>::iterator iter;

			iter = find(id);
			if(iter->second <= REFERENCE_INIT) {

				if(m_surplus.find(id) != m_surplus.end()) {
					THROW_GAEA_UID_EXCEPTION_FORMAT(GAEA_UID_EXCEPTION_DUPLICATE, "%x", id);
				}

				m_surplus.insert(id);
				m_entry.erase(iter);
			} else {
				result = --iter->second;
			}

			return result;
		}

		std::map<gaea::uid_t, size_t>::iterator 
		_manager::find(
			__in const gaea::uid_t &id
			)
		{
			std::map<gaea::uid_t, size_t>::iterator result;

			if(!m_initialized) {
				THROW_GAEA_UID_EXCEPTION(GAEA_UID_EXCEPTION_UNINITIALIZED);
			}

			result = m_entry.find(id);
			if(result == m_entry.end()) {
				THROW_GAEA_UID_EXCEPTION_FORMAT(GAEA_UID_EXCEPTION_NOT_FOUND, "%x", id);
			}

			return result;
		}

		void 
		_manager::generate(
			__out gaea::uid_t &id
			)
		{
			std::set<gaea::uid_t>::iterator iter;

			if(!m_surplus.empty()) {
				iter = m_surplus.begin();
				id = *iter;
				m_surplus.erase(iter);
			} else if(m_next < UID_INVALID) {
				id = m_next++;
			} else {
				THROW_GAEA_UID_EXCEPTION(GAEA_UID_EXCEPTION_FULL);
			}

			if(m_entry.find(id) != m_entry.end()) {
				THROW_GAEA_UID_EXCEPTION_FORMAT(GAEA_UID_EXCEPTION_DUPLICATE, "%x", id);
			}

			m_entry.insert(std::pair<gaea::uid_t, size_t>(id, REFERENCE_INIT));
		}

		size_t 
		_manager::increment_reference(
			__in const gaea::uid_t &id
			)
		{
			return ++find(id)->second;
		}

		void 
		_manager::initialize(void)
		{

			if(m_initialized) {
				THROW_GAEA_UID_EXCEPTION(GAEA_UID_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
		}

		bool 
		_manager::is_allocated(void)
		{
			return (gaea::uid::manager::m_instance != nullptr);
		}

		bool 
		_manager::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_manager::reference_count(
			__in const gaea::uid_t &id
			)
		{
			return find(id)->second;
		}

		size_t 
		_manager::size(void)
		{
			return m_entry.size();
		}

		std::string 
		_manager::to_string(
			__in_opt bool verbose
			)
		{
			size_t count = 0;
			std::stringstream result;
			std::map<gaea::uid_t, size_t>::iterator iter;

			result << GAEA_UID_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

			if(m_initialized) {
				result << " INST=" << SCALAR_AS_HEX(uintptr_t, this)
					<< ", ENTRIES=" << m_entry.size()
					<< ", SURPLUS=" << m_surplus.size()
					<< ", NEXT=" << SCALAR_AS_HEX(gaea::uid_t, m_next);

				if(verbose) {

					for(iter = m_entry.begin(); iter != m_entry.end(); ++count, ++iter) {
						result << std::endl << "[" << count << "]"
							<< " {" << SCALAR_AS_HEX(gaea::uid_t, iter->first) << "}"
							<< ", REF=" << iter->second;
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
				clear();
			}
		}
	}
}
