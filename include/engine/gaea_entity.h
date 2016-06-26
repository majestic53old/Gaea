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

#ifndef GAEA_ENTITY_H_
#define GAEA_ENTITY_H_

namespace gaea {

	#define ENTITY_ALL SCALAR_INVALID(gaea::entity_t)
	#define ENTITY_MAX ENTITY_MODEL
	#define ENTITY_POSITION_INIT glm::vec3()
	#define ENTITY_ROTATION_INIT glm::vec3(0.f, 0.f, glm::pi<GLfloat>())
	#define ENTITY_SPECIFIER_UNDEFINED SCALAR_INVALID(uint32_t)
	#define ENTITY_UP_INIT glm::vec3(0.f, DIRECTION_UP, 0.f)
	#define ENTITY_UNIFORM_MODEL "model"
	#define ENTITY_UNIFORM_PROJECTION "projection"
	#define ENTITY_UNIFORM_VIEW "view"
	#define ENTITY_VISIBLE_INIT true

	typedef enum {
		ENTITY_CAMERA = 0,
		ENTITY_MODEL,
	} entity_t;

	namespace engine {

		namespace entity {

			typedef class _base :
					public gaea::engine::object::base {

				public:

					_base(
						__in gaea::entity_t type,
						__in_opt uint32_t specifier = ENTITY_SPECIFIER_UNDEFINED,
						__in_opt bool visible = ENTITY_VISIBLE_INIT
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

					bool is_visible(void);

					virtual void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						) = 0;

					uint32_t specifier(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					gaea::entity_t type(void);

					virtual void update(
						__in GLfloat delta
						) = 0;

					void visible(
						__in bool value
						);

				protected:

					void decrement_reference(void);

					void generate(void);

					void increment_reference(void);

					uint32_t m_specifier;

					bool m_visible;

			} base;

			typedef class _base_position :
					public gaea::engine::entity::base {

				public:

					_base_position(
						__in gaea::entity_t type,
						__in_opt uint32_t specifier = ENTITY_SPECIFIER_UNDEFINED,
						__in_opt const glm::vec3 &position = ENTITY_POSITION_INIT,
						__in_opt const glm::vec3 &rotation = ENTITY_ROTATION_INIT,
						__in_opt const glm::vec3 &up = ENTITY_UP_INIT,
						__in_opt bool visible = ENTITY_VISIBLE_INIT
						);

					_base_position(
						__in const _base_position &other
						);

					virtual ~_base_position(void);

					_base_position &operator=(
						__in const _base_position &other
						);

					static std::string as_string(
						__in const _base_position &object,
						__in_opt bool verbose = false
						);

					glm::vec3 &position(void);

					virtual void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						) = 0;

					glm::vec3 &rotation(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					glm::vec3 &up(void);

					virtual void update(
						__in GLfloat delta
						) = 0;

				protected:

					glm::vec3 m_position;

					glm::vec3 m_rotation;

					glm::vec3 m_up;

			} base_position;

			typedef class _base_model :
					public gaea::engine::entity::base_position {

				public:

					_base_model(
						__in gaea::entity_t type,
						__in_opt uint32_t specifier = ENTITY_SPECIFIER_UNDEFINED,
						__in_opt const glm::vec3 &position = ENTITY_POSITION_INIT,
						__in_opt const glm::vec3 &rotation = ENTITY_ROTATION_INIT,
						__in_opt const glm::vec3 &up = ENTITY_UP_INIT,
						__in_opt GLuint model_id = GL_HANDLE_INVALID,
						__in_opt GLuint projection_id = GL_HANDLE_INVALID,
						__in_opt GLuint view_id = GL_HANDLE_INVALID,
						__in_opt bool visible = ENTITY_VISIBLE_INIT
						);

					_base_model(
						__in const _base_model &other
						);

					virtual ~_base_model(void);

					_base_model &operator=(
						__in const _base_model &other
						);

					static std::string as_string(
						__in const _base_model &object,
						__in_opt bool verbose = false
						);

					const glm::mat4 &model(void);

					GLint &model_id(void);

					GLint &projection_id(void);

					virtual void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						) = 0;

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					virtual void update(
						__in GLfloat delta
						) = 0;

					void update_model(void);

					GLint &view_id(void);

				protected:

					glm::mat4 m_model;

					GLint m_model_id;

					GLint m_projection_id;

					GLint m_view_id;

			} base_model;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					size_t decrement_reference(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void generate(
						__in gaea::engine::entity::base &object
						);

					size_t increment_reference(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						);

					size_t size(void);

					size_t size(
						__in gaea::entity_t type
						);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

					void update(
						__in GLfloat delta
						);

				protected:

					_manager(void);

					_manager(
						__in const _manager &other
						);

					_manager &operator=(
						__in const _manager &other
						);

					static void _delete(void);

					void clear(void);

					std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator find(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					std::vector<std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>> m_entry;

					bool m_initialized;

					static _manager *m_instance;

			} manager;
		}
	}
}

#endif // GAEA_ENTITY_H_
