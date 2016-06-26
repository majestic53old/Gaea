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

#ifndef GAEA_MODEL_H_
#define GAEA_MODEL_H_

namespace gaea {

	namespace engine {

		namespace model {

			typedef class _base :
					gaea::engine::entity::base_model {

				public:

					_base(
						__in_opt const glm::vec3 &position = ENTITY_POSITION_INIT,
						__in_opt const glm::vec3 &rotation = ENTITY_ROTATION_INIT,
						__in_opt const glm::vec3 &up = ENTITY_UP_INIT,
						__in_opt bool visible = ENTITY_VISIBLE_INIT
						);

					_base(
						__in const _base &other
						);

					virtual ~_base(void);

					_base &operator=(
						__in const _base &other
						);

					void add_attribute(
						__in const GLvoid *data,
						__in GLsizeiptr size,
						__in GLenum target,
						__in GLenum usage,
						__in GLuint index,
						__in GLint element_size,
						__in GLenum element_type,
						__in_opt GLboolean normalized = VAO_NORMALIZED_INIT,
						__in_opt GLsizei stride = VAO_STRIDE_INIT,
						__in_opt const GLvoid *pointer = VAO_POINTER_INIT
						);

					void add_data(
						__in const GLvoid *data,
						__in GLsizeiptr size,
						__in GLenum target,
						__in GLenum usage
						);

					static std::string as_string(
						__in const _base &object,
						__in_opt bool verbose = false
						);

					void link(
						__in const std::string &vertex,
						__in const std::string &fragment,
						__in_opt const std::vector<std::string> &attribute = std::vector<std::string>()
						);

					virtual void render(
						__in const glm::vec3 &position,
						__in const glm::vec3 &rotation,
						__in const glm::vec3 &up,
						__in const glm::mat4 &projection,
						__in const glm::mat4 &view
						);

					void set_indicies(
						__in GLuint indicies
						);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					virtual void update(
						__in GLfloat delta
						);

				protected:

					GLuint m_indicies;

					gaea::graphics::program::base m_program;

					gaea::graphics::vao::base m_vao;

			} base;
		}
	}
}

#endif // GAEA_MODEL_H_
