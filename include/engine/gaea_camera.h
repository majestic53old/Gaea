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

#ifndef GAEA_CAMERA_H_
#define GAEA_CAMERA_H_

namespace gaea {

	namespace engine {

		namespace camera {

			#define CAMERA_CLIP_INIT glm::vec2(CAMERA_CLIP_MIN, CAMERA_CLIP_MAX)
			#define CAMERA_CLIP_MAX 1000.f
			#define CAMERA_CLIP_MIN 0.1f
			#define CAMERA_FOV_INIT 45.f
			#define CAMERA_FOV_MAX 90.f
			#define CAMERA_FOV_MIN 10.f
			#define CAMERA_PITCH_MAX 89.f
			#define CAMERA_POSITION_INIT ENTITY_POSITION_INIT
			#define CAMERA_ROTATION_INIT ENTITY_ROTATION_INIT
			#define CAMERA_UP_INIT ENTITY_UP_INIT
			#define CAMERA_YAW_MAX 360.f
			#define CAMERA_YAW_MIN 0.f

			typedef class _base :
					public gaea::engine::entity::base_position,
					public gaea::engine::event::observer {

				public:

					_base(
						__in const glm::ivec2 &dimensions,
						__in_opt const glm::vec3 &position = CAMERA_POSITION_INIT,
						__in_opt const glm::vec3 &rotation = CAMERA_ROTATION_INIT,
						__in_opt const glm::vec3 &up = CAMERA_UP_INIT,
						__in_opt const glm::vec2 &clip = CAMERA_CLIP_INIT,
						__in_opt GLfloat fov = CAMERA_FOV_INIT
						);

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

					const glm::mat4 &projection(void);

					virtual void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						);

					void set_clip(
						__in const glm::vec2 &clip
						);

					void set_dimensions(
						__in const glm::ivec2 &dimensions
						);

					void set_fov(
						__in GLfloat fov
						);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					virtual void update(
						__in GLfloat delta
						);

					const glm::mat4 &view(void);

				protected:

					static void event_handler(
						__in gaea::engine::event::base &event,
						__in void *context
						);

					void setup(void);

					void setup(
						__in const glm::ivec2 &dimensions,
						__in const glm::vec2 &clip,
						__in GLfloat fov
						);

					void update_projection(void);

					void update_view(void);

					glm::vec2 m_clip;

					glm::ivec2 m_dimensions;

					GLfloat m_fov;
				
					GLfloat m_fov_delta;

					glm::vec3 m_position_delta;

					glm::mat4 m_projection;

					glm::vec3 m_rotation_delta;

					glm::vec3 m_up_delta;

					glm::mat4 m_view;
			} base;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					gaea::engine::camera::base &entry(void);

					void initialize(
						__in const glm::ivec2 &dimensions,
						__in_opt const glm::vec3 &position = CAMERA_POSITION_INIT,
						__in_opt const glm::vec3 &rotation = CAMERA_ROTATION_INIT,
						__in_opt const glm::vec3 &up = CAMERA_UP_INIT,
						__in_opt const glm::vec2 &clip = CAMERA_CLIP_INIT,
						__in_opt GLfloat fov = CAMERA_FOV_INIT
						);

					static bool is_allocated(void);

					bool is_initialized(void);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

				protected:

					_manager(void);

					_manager(
						__in const _manager &other
						);

					_manager &operator=(
						__in const _manager &other
						);

					static void _delete(void);

					gaea::engine::camera::base *m_entry;

					bool m_initialized;

					static _manager *m_instance;

			} manager;
		}
	}
}

#endif // GAEA_CAMERA_H_
