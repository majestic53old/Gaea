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
#include "gaea_entity_type.h"

namespace gaea {

	namespace engine {

		namespace entity {

			static const std::string ENTITY_STR[] = {
				"CAMERA", "SURFACE",
				};

			#define ENTITY_STRING(_TYPE_) \
				((_TYPE_) > ENTITY_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(ENTITY_STR[_TYPE_]))

			_base::_base(
				__in gaea::entity_t type,
				__in_opt uint32_t specifier,
				__in_opt bool visible
				) :
					gaea::engine::object::base(OBJECT_EVENT, type),
					m_specifier(specifier),
					m_visible(visible)
			{

				if(type > ENTITY_MAX) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}

				generate();
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::object::base(other),
					m_specifier(other.m_specifier),
					m_visible(other.m_visible)
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
					m_specifier = other.m_specifier;
					m_visible = other.m_visible;
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
					<< " [" << ENTITY_STRING((gaea::entity_t) object.m_subtype);

				if(object.m_specifier != ENTITY_SPECIFIER_UNDEFINED) {
					result << ", " << SCALAR_AS_HEX(uint32_t, object.m_specifier);
				}

				result << "] " << (object.m_visible ? "VISIBLE" : "INVISIBLE");

				return result.str();
			}

			void 
			_base::decrement_reference(void)
			{

				if(gaea::engine::entity::manager::is_allocated()) {

					gaea::engine::entity::manager &instance = gaea::engine::entity::manager::acquire();
					if(instance.is_initialized()
							&& instance.contains(m_id, (gaea::entity_t) m_subtype)) {
						instance.decrement_reference(m_id, (gaea::entity_t) m_subtype);
					}
				}
			}

			void 
			_base::generate(void)
			{

				if(gaea::engine::entity::manager::is_allocated()) {

					gaea::engine::entity::manager &instance = gaea::engine::entity::manager::acquire();
					if(instance.is_initialized()) {
						instance.generate(*this);
					}
				}
			}

			void 
			_base::increment_reference(void)
			{

				if(gaea::engine::entity::manager::is_allocated()) {

					gaea::engine::entity::manager &instance = gaea::engine::entity::manager::acquire();
					if(instance.is_initialized()
							&& instance.contains(m_id, (gaea::entity_t) m_subtype)) {
						instance.increment_reference(m_id, (gaea::entity_t) m_subtype);
					}
				}
			}

			bool 
			_base::is_visible(void)
			{
				return m_visible;
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
				return gaea::engine::entity::base::as_string(*this, verbose);
			}

			gaea::entity_t 
			_base::type(void)
			{
				return (gaea::entity_t) m_subtype;
			}

			void 
			_base::visible(
				__in bool value
				)
			{
				m_visible = value;
			}

			_base_position::_base_position(
				__in gaea::entity_t type,
				__in_opt uint32_t specifier,
				__in_opt const glm::vec3 &position,
				__in_opt const glm::vec3 &rotation,
				__in_opt const glm::vec3 &up,
				__in_opt bool visible
				) :
					gaea::engine::entity::base(type, specifier, visible),
					m_position(position),
					m_rotation(rotation),
					m_up(up)
			{
				return;
			}

			_base_position::_base_position(
				__in const _base_position &other
				) :
					gaea::engine::entity::base(other),
					m_position(other.m_position),
					m_rotation(other.m_rotation),
					m_up(other.m_up)
			{
				return;
			}

			_base_position::~_base_position(void)
			{
				return;
			}

			_base_position &
			_base_position::operator=(
				__in const _base_position &other
				)
			{

				if(this != &other) {
					gaea::engine::entity::base::operator=(other);
					m_position = other.m_position;
					m_rotation = other.m_rotation;
					m_up = other.m_up;
				}

				return *this;
			}

			std::string 
			_base_position::as_string(
				__in const _base_position &object,
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				result << gaea::engine::entity::base::as_string(object, verbose)
					<< ", POS={" << object.m_position.x << ", " << object.m_position.y << ", " << object.m_position.z << "}"
					<< ", ROT={" << object.m_rotation.x << ", " << object.m_rotation.y << ", " << object.m_rotation.z << "}"
					<< ", UP={" << object.m_up.x << ", " << object.m_up.y << ", " << object.m_up.z << "}";

				return result.str();
			}

			glm::vec3 &
			_base_position::position(void)
			{
				return m_position;
			}

			glm::vec3 &
			_base_position::rotation(void)
			{
				return m_rotation;
			}

			std::string 
			_base_position::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::entity::base_position::as_string(*this, verbose);
			}

			glm::vec3 &
			_base_position::up(void)
			{
				return m_up;
			}

			_base_model::_base_model(
				__in gaea::entity_t type,
				__in_opt uint32_t specifier,
				__in_opt const glm::vec3 &position,
				__in_opt const glm::vec3 &rotation,
				__in_opt const glm::vec3 &up,
				__in_opt GLuint model_id,
				__in_opt GLuint projection_id,
				__in_opt GLuint view_id,
				__in_opt bool visible
				) :
					gaea::engine::entity::base_position(type, specifier, position, rotation, up, visible),
					m_model_id(model_id),
					m_projection_id(projection_id),
					m_view_id(view_id)
			{
				return;
			}

			_base_model::_base_model(
				__in const _base_model &other
				) :
					gaea::engine::entity::base_position(other),
					m_model_id(other.m_model_id),
					m_projection_id(other.m_projection_id),
					m_view_id(other.m_view_id)
			{
				return;
			}

			_base_model::~_base_model(void)
			{
				return;
			}

			_base_model &
			_base_model::operator=(
				__in const _base_model &other
				)
			{

				if(this != &other) {
					gaea::engine::entity::base_position::operator=(other);
					m_model_id = other.m_model_id;
					m_projection_id = other.m_projection_id;
					m_view_id = other.m_view_id;
				}

				return *this;
			}

			std::string 
			_base_model::as_string(
				__in const _base_model &object,
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				result << gaea::engine::entity::base_position::as_string(object, verbose)
					<< ", MID=" << SCALAR_AS_HEX(GLint, object.m_model_id)
					<< ", PID=" << SCALAR_AS_HEX(GLint, object.m_projection_id)
					<< ", VID=" << SCALAR_AS_HEX(GLint, object.m_view_id);

				return result.str();
			}

			const glm::mat4 &
			_base_model::model(void)
			{
				return m_model;
			}

			GLint &
			_base_model::model_id(void)
			{
				return m_model_id;
			}

			GLint &
			_base_model::projection_id(void)
			{
				return m_projection_id;
			}

			std::string 
			_base_model::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::entity::base_model::as_string(*this, verbose);
			}

			void 
			_base_model::update_model(void)
			{
				m_model = glm::lookAt(m_position, m_position + m_rotation, m_up);
			}

			GLint &
			_base_model::view_id(void)
			{
				return m_view_id;
			}

			_manager *_manager::m_instance = nullptr;

			_manager::_manager(void) :
				m_initialized(false)
			{
				std::atexit(gaea::engine::entity::manager::_delete);
			}

			_manager::~_manager(void)
			{
				uninitialize();
			}

			void _manager::_delete(void)
			{

				if(gaea::engine::entity::manager::m_instance) {
					delete gaea::engine::entity::manager::m_instance;
					gaea::engine::entity::manager::m_instance = nullptr;
				}
			}

			_manager &
			_manager::acquire(void)
			{

				if(!gaea::engine::entity::manager::m_instance) {

					gaea::engine::entity::manager::m_instance = new gaea::engine::entity::manager;
					if(!gaea::engine::entity::manager::m_instance) {
						THROW_GAEA_ENTITY_EXCEPTION(GAEA_ENTITY_EXCEPTION_ALLOCATED);
					}
				}

				return *gaea::engine::entity::manager::m_instance;
			}

			void 
			_manager::clear(void)
			{
				m_entry.clear();
			}

			bool 
			_manager::contains(
				__in gaea::uid_t id,
				__in gaea::entity_t type
				)
			{

				if(!m_initialized) {
					THROW_GAEA_ENTITY_EXCEPTION(GAEA_ENTITY_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}

				return (m_entry.at(type).find(id) != m_entry.at(type).end());
			}

			size_t 
			_manager::decrement_reference(
				__in gaea::uid_t id,
				__in gaea::entity_t type
				)
			{
				size_t result = 0;
				std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator iter;

				iter = find(id, type);
				if(iter->second.second <= REFERENCE_INIT) {
					m_entry.at(type).erase(iter);
				} else {
					result = --iter->second.second;
				}

				return result;
			}

			std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator 
			_manager::find(
				__in gaea::uid_t id,
				__in gaea::entity_t type
				)
			{
				std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator result;

				if(!m_initialized) {
					THROW_GAEA_ENTITY_EXCEPTION(GAEA_ENTITY_EXCEPTION_UNINITIALIZED);
				}

				if(type >= m_entry.size()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}

				result = m_entry.at(type).find(id);
				if(result == m_entry.at(type).end()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_NOT_FOUND,
						"[%s (%x)] %x", ENTITY_STRING(type), type, id);
				}

				return result;
			}

			void 
			_manager::generate(
				__in gaea::engine::entity::base &object
				)
			{
				gaea::uid_t id;
				gaea::entity_t type;

				if(!m_initialized) {
					THROW_GAEA_ENTITY_EXCEPTION(GAEA_ENTITY_EXCEPTION_UNINITIALIZED);
				}

				type = object.type();
				if(type >= m_entry.size()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}

				id = object.id();
				if(m_entry.at(type).find(object.id()) != m_entry.at(type).end()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_DUPLICATE,
						"[%s (%x)] %x", ENTITY_STRING(type), type, id);
				}

				m_entry.at(type).insert(std::pair<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>(
					id, std::pair<gaea::engine::entity::base &, size_t>(object, REFERENCE_INIT)));
			}

			size_t 
			_manager::increment_reference(
				__in gaea::uid_t id,
				__in gaea::entity_t type
				)
			{
				return ++find(id, type)->second.second;
			}

			void 
			_manager::initialize(void)
			{

				if(m_initialized) {
					THROW_GAEA_ENTITY_EXCEPTION(GAEA_ENTITY_EXCEPTION_INITIALIZED);
				}

				m_entry.resize(ENTITY_MAX + 1,
					std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>());
				m_initialized = true;
			}

			bool 
			_manager::is_allocated(void)
			{
				return (gaea::engine::entity::manager::m_instance != nullptr);
			}

			bool 
			_manager::is_initialized(void)
			{
				return m_initialized;
			}

			size_t 
			_manager::reference_count(
				__in gaea::uid_t id,
				__in gaea::entity_t type
				)
			{
				return find(id, type)->second.second;
			}

			void 
			_manager::render(
				__in const glm::vec3 &position,
				__in const glm::vec3 &rotation,
				__in const glm::vec3 &up,
				__in const glm::mat4 &projection,
				__in const glm::mat4 &view
				)
			{
				size_t iter = 0;
				std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator entry_iter;

				for(; iter < m_entry.size(); ++iter) {

					if(iter == ENTITY_CAMERA) {
						continue;
					}

					for(entry_iter = m_entry.at(iter).begin(); entry_iter != m_entry.at(iter).end();
							++entry_iter) {

						gaea::engine::entity::base &object = entry_iter->second.first;
						if(object.is_visible()) {
							object.render(position, rotation, up, projection, view);
						}
					}
				}
			}

			size_t 
			_manager::size(void)
			{
				size_t iter = 0, result = 0;

				for(; iter < m_entry.size(); ++iter) {
					result += m_entry.at(iter).size();
				}

				return result;
			}

			size_t 
			_manager::size(
				__in gaea::entity_t type
				)
			{

				if(type >= m_entry.size()) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}

				return m_entry.at(type).size();
			}

			std::string 
			_manager::to_string(
				__in_opt bool verbose
				)
			{
				size_t count, iter = 0;
				std::stringstream result;
				std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator entry_iter;

				result << GAEA_ENTITY_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

				if(m_initialized) {
					result << " INST=" << SCALAR_AS_HEX(uintptr_t, this)
						<< ", ENTRIES=" << size();

					if(verbose) {

						for(; iter < m_entry.size(); ++iter) {
							result << std::endl << "[" << ENTITY_STRING(iter)
								<< " (" << SCALAR_AS_HEX(gaea::entity_t, iter) << ")]"
								<< "[" << m_entry.at(iter).size() << "]";

							for(count = 0, entry_iter = m_entry.at(iter).begin(); entry_iter != m_entry.at(iter).end();
									++count, ++entry_iter) {
								result << "[" << count << "]"
									<< " " << gaea::engine::entity::base::as_string(entry_iter->second.first,
										verbose)
									<< ", REF=" << entry_iter->second.second;
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
					clear();
				}
			}

			void 
			_manager::update(
				__in GLfloat delta
				)
			{
				size_t iter = 0;
				std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator entry_iter;

				for(; iter < m_entry.size(); ++iter) {

					for(entry_iter = m_entry.at(iter).begin(); entry_iter != m_entry.at(iter).end();
							++entry_iter) {
						entry_iter->second.first.update(delta);
					}
				}
			}
		}
	}
}
