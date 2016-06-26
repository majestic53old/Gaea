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
#include "gaea_camera_type.h"

namespace gaea {

	namespace engine {

		namespace camera {

			#define CAMERA_DIMENSION_MIN 1.f
			#define CAMERA_FOV_MAX 90.f
			#define CAMERA_FOV_MIN 10.f
			#define CAMERA_PITCH_MAX 89.f
			#define CAMERA_YAW_MAX 360.f
			#define CAMERA_YAW_MIN 0.f

			_base::_base(
				__in const glm::ivec2 &dimensions,
				__in_opt const glm::vec3 &position,
				__in_opt const glm::vec3 &rotation,
				__in_opt const glm::vec3 &up,
				__in_opt const glm::vec2 &clip,
				__in_opt GLfloat fov
				) :
					gaea::engine::entity::base(ENTITY_CAMERA, ENTITY_SPECIFIER_UNDEFINED, false),
					m_clip(CAMERA_CLIP_INIT),
					m_dimensions(dimensions),
					m_fov(CAMERA_FOV_INIT),
					m_position(CAMERA_POSITION_INIT),
					m_projection(glm::mat4()),
					m_rotation(CAMERA_ROTATION_INIT),
					m_up(CAMERA_UP_INIT),
					m_view(glm::mat4())
			{
				setup(dimensions, position, rotation, up, clip, fov);
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::entity::base(other),
					m_clip(other.m_clip),
					m_dimensions(other.m_dimensions),
					m_fov(other.m_fov),
					m_position(other.m_position),
					m_projection(other.m_projection),
					m_rotation(other.m_rotation),
					m_up(other.m_up),
					m_view(other.m_view)
			{
				setup();
			}

			_base::~_base(void)
			{
				return;
			}

			_base &
			_base::operator=(
				__in const _base &other
				)
			{

				if(this != &other) {
					gaea::engine::entity::base::operator=(other);
					m_clip = other.m_clip;
					m_dimensions = other.m_dimensions;
					m_fov = other.m_fov;
					m_position = other.m_position;
					m_projection = other.m_projection;
					m_rotation = other.m_rotation;
					m_up = other.m_up;
					m_view = other.m_view;
					setup();
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

				result << gaea::engine::entity::base::as_string(object, verbose)
					<< ", CLIP={" << object.m_clip.x << ", " << object.m_clip.y << "}"
					<< ", DIM={" << object.m_dimensions.x << ", " << object.m_dimensions.y << "}"
					<< ", FOV=" << object.m_fov
					<< ", POS={" << object.m_position.x << ", " << object.m_position.y << ", " << object.m_position.z << "}"
					<< ", ROT={" << object.m_rotation.x << ", " << object.m_rotation.y << ", " << object.m_rotation.z << "}"
					<< ", UP={" << object.m_up.x << ", " << object.m_up.y << ", " << object.m_up.z << "}";

				return result.str();
			}

			void 
			_base::event_handler(
				__in gaea::engine::event::base &event,
				__in void *context
				)
			{
				uint32_t specifier;
				GLfloat *float_data = nullptr;
				glm::vec2 *vec2_data = nullptr;
				glm::vec3 *vec3_data = nullptr;
				glm::ivec2 *ivec2_data = nullptr;
				gaea::engine::camera::base *instance = nullptr;

				instance = (gaea::engine::camera::base *) context;
				if(!instance) {
					THROW_GAEA_CAMERA_EXCEPTION(GAEA_CAMERA_EXCEPTION_INVALID_CONTEXT);
				}

				specifier = event.specifier();
				switch(specifier) {
					case EVENT_CAMERA_CLIP_SET:

						vec2_data = (glm::vec2 *) event.context();
						if(!vec2_data || (event.length() != sizeof(glm::vec2))) {
							THROW_GAEA_CAMERA_EXCEPTION_FORMAT(GAEA_CAMERA_EXCEPTION_INVALID_EVENT,
								"%s", STRING_CHECK(event.to_string(true)));
						}

						instance->set_clip(*vec2_data);
						instance->update_projection();
						break;
					case EVENT_CAMERA_DIMENSIONS_SET:

						ivec2_data = (glm::ivec2 *) event.context();
						if(!ivec2_data || (event.length() != sizeof(glm::ivec2))) {
							THROW_GAEA_CAMERA_EXCEPTION_FORMAT(GAEA_CAMERA_EXCEPTION_INVALID_EVENT,
								"%s", STRING_CHECK(event.to_string(true)));
						}

						instance->set_dimensions(*ivec2_data);
						instance->update_projection();
						break;
					case EVENT_CAMERA_FOV_SET:

						float_data = (GLfloat *) event.context();
						if(!float_data || (event.length() != sizeof(GLfloat))) {
							THROW_GAEA_CAMERA_EXCEPTION_FORMAT(GAEA_CAMERA_EXCEPTION_INVALID_EVENT,
								"%s", STRING_CHECK(event.to_string(true)));
						}

						instance->set_fov(*float_data);
						instance->update_projection();
						break;
					case EVENT_CAMERA_POSITION_DELTA:
					case EVENT_CAMERA_POSITION_SET:
					case EVENT_CAMERA_ROTATION_DELTA:
					case EVENT_CAMERA_ROTATION_SET:
					case EVENT_CAMERA_UP_DELTA:
					case EVENT_CAMERA_UP_SET:

						vec3_data = (glm::vec3 *) event.context();
						if(!vec3_data || (event.length() != sizeof(glm::vec3))) {
							THROW_GAEA_CAMERA_EXCEPTION_FORMAT(GAEA_CAMERA_EXCEPTION_INVALID_EVENT,
								"%s", STRING_CHECK(event.to_string(true)));
						}

						switch(specifier) {
							case EVENT_CAMERA_POSITION_DELTA:
								instance->set_position(*vec3_data, true);
								break;
							case EVENT_CAMERA_POSITION_SET:
								instance->set_position(*vec3_data);
								break;
							case EVENT_CAMERA_ROTATION_DELTA:
								instance->set_rotation(*vec3_data, true);
								break;
							case EVENT_CAMERA_ROTATION_SET:
								instance->set_rotation(*vec3_data);
								break;
							case EVENT_CAMERA_UP_DELTA:
								instance->set_up(*vec3_data, true);
								break;
							case EVENT_CAMERA_UP_SET:
								instance->set_up(*vec3_data);
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
			}

			const glm::mat4 &
			_base::projection(void)
			{
				return m_projection;
			}

			void 
			_base::render(void)
			{
				return;
			}

			void 
			_base::set_clip(
				__in const glm::vec2 &clip
				)
			{

				m_clip = clip;
				if(m_clip.x < CAMERA_CLIP_MIN) {
					m_clip.x = CAMERA_CLIP_MIN;
				} else if(m_clip.x > CAMERA_CLIP_MAX) {
					m_clip.x = CAMERA_CLIP_MAX;
				}

				if(m_clip.y < CAMERA_CLIP_MIN) {
					m_clip.y = CAMERA_CLIP_MIN;
				} else if(m_clip.y > CAMERA_CLIP_MAX) {
					m_clip.y = CAMERA_CLIP_MAX;
				}

				if(m_clip.x > m_clip.y) {
					m_clip.x = m_clip.y;
				}
			}

			void 
			_base::set_dimensions(
				__in const glm::ivec2 &dimensions
				)
			{

				m_dimensions = dimensions;
				if(m_dimensions.x < CAMERA_DIMENSION_MIN) {
					m_dimensions.x = CAMERA_DIMENSION_MIN;
				}

				if(m_dimensions.y < CAMERA_DIMENSION_MIN) {
					m_dimensions.y = CAMERA_DIMENSION_MIN;
				}
			}

			void 
			_base::set_fov(
				__in GLfloat fov
				)
			{

				m_fov = fov;
				if(m_fov < CAMERA_FOV_MIN) {
					m_fov = CAMERA_FOV_MIN;
				} else if(m_fov > CAMERA_FOV_MAX) {
					m_fov = CAMERA_FOV_MAX;
				}
			}

			void 
			_base::set_position(
				__in const glm::vec3 &position,
				__in_opt bool delta
				)
			{

				if(delta) {
					m_position += position;
				} else {
					m_position = position;
				}
			}

			void 
			_base::set_rotation(
				__in const glm::vec3 &rotation,
				__in_opt bool delta
				)
			{

				if(delta) {
					m_rotation += rotation;
				} else {
					m_rotation = rotation;
				}

				if(m_rotation.x < -CAMERA_PITCH_MAX) {
					m_rotation.x = -CAMERA_PITCH_MAX;
				} else if(m_rotation.x > CAMERA_PITCH_MAX) {
					m_rotation.x = CAMERA_PITCH_MAX;
				}

				if(m_rotation.y < CAMERA_YAW_MIN) {
					m_rotation.y = CAMERA_YAW_MAX;
				} else if(m_rotation.y > CAMERA_YAW_MAX) {
					m_rotation.y = CAMERA_YAW_MIN;
				}

				m_rotation = glm::normalize(glm::vec3(
					std::cos(glm::radians(m_rotation.x)) * std::sin(glm::radians(m_rotation.y)),
					std::sin(glm::radians(m_rotation.x)),
					std::cos(glm::radians(m_rotation.x)) * std::cos(glm::radians(m_rotation.y))));
			}

			void 
			_base::set_up(
				__in const glm::vec3 &up,
				__in_opt bool delta
				)
			{

				if(delta) {
					m_up += up;
				} else {
					m_up = up;
				}
			}

			void 
			_base::setup(void)
			{
				gaea::engine::event::observer::register_handler(&base::event_handler, EVENT_CAMERA);
				update_projection();
				update_view();
			}

			void 
			_base::setup(
				__in const glm::ivec2 &dimensions,
				__in const glm::vec3 &position,
				__in const glm::vec3 &rotation,
				__in const glm::vec3 &up,
				__in const glm::vec2 &clip,
				__in GLfloat fov
				)
			{
				set_clip(clip);
				set_dimensions(dimensions);
				set_fov(fov);
				m_position = position;
				m_rotation = rotation;
				m_up = up;
				setup();
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::camera::base::as_string(*this, verbose);
			}

			void 
			_base::update(
				__in GLfloat delta
				)
			{
				update_view();
			}

			void 
			_base::update_projection(void)
			{
				m_projection = glm::perspective(glm::radians(m_fov), (GLfloat) m_dimensions.x / (GLfloat) m_dimensions.y,
					m_clip.x, m_clip.y);
			}

			void 
			_base::update_view(void)
			{
				m_view = glm::lookAt(m_position, m_position + m_rotation, m_up);
			}

			const glm::mat4 &
			_base::view(void)
			{
				return m_view;
			}

			_manager *_manager::m_instance = nullptr;

			_manager::_manager(void) :
				m_entry(nullptr),
				m_initialized(false)
			{
				std::atexit(gaea::engine::camera::manager::_delete);
			}

			_manager::~_manager(void)
			{
				uninitialize();
			}

			void 
			_manager::_delete(void)
			{

				if(gaea::engine::camera::manager::m_instance) {
					delete gaea::engine::camera::manager::m_instance;
					gaea::engine::camera::manager::m_instance = nullptr;
				}
			}

			_manager &
			_manager::acquire(void)
			{

				if(!gaea::engine::camera::manager::m_instance) {

					gaea::engine::camera::manager::m_instance = new gaea::engine::camera::manager;
					if(!gaea::engine::camera::manager::m_instance) {
						THROW_GAEA_CAMERA_EXCEPTION(GAEA_CAMERA_EXCEPTION_ALLOCATED);
					}
				}

				return *gaea::engine::camera::manager::m_instance;
			}

			gaea::engine::camera::base &
			_manager::entry(void)
			{

				if(!m_initialized) {
					THROW_GAEA_CAMERA_EXCEPTION(GAEA_CAMERA_EXCEPTION_UNINITALIZED);
				}

				return *m_entry;
			}

			void 
			_manager::initialize(
				__in const glm::ivec2 &dimensions,
				__in_opt const glm::vec3 &position,
				__in_opt const glm::vec3 &rotation,
				__in_opt const glm::vec3 &up,
				__in_opt const glm::vec2 &clip,
				__in_opt GLfloat fov
				)
			{

				if(m_initialized) {
					THROW_GAEA_CAMERA_EXCEPTION(GAEA_CAMERA_EXCEPTION_INITIALIZED);
				}

				m_entry = new gaea::engine::camera::base(dimensions, position, rotation, up, clip);
				if(!m_entry) {
					THROW_GAEA_CAMERA_EXCEPTION(GAEA_CAMERA_EXCEPTION_ENTRY_ALLOCATED);
				}

				m_initialized = true;
			}

			bool 
			_manager::is_allocated(void)
			{
				return (gaea::engine::camera::manager::m_instance != nullptr);
			}

			bool 
			_manager::is_initialized(void)
			{
				return m_initialized;
			}

			std::string 
			_manager::to_string(
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				result << GAEA_CAMERA_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

				if(m_initialized) {
					result << " INST=" << SCALAR_AS_HEX(uintptr_t, this)
						<< ", ENTRY=" << SCALAR_AS_HEX(uintptr_t, m_entry);

					if(verbose) {
						result << ", " << m_entry->to_string(verbose);
					}
				}

				return result.str();
			}

			void 
			_manager::uninitialize(void)
			{

				if(m_initialized) {

					if(m_entry) {
						delete m_entry;
						m_entry = nullptr;
					}

					m_initialized = false;
				}
			}
		}
	}
}
