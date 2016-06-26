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
#include "gaea_surface_type.h"

namespace gaea {

	namespace engine {

		namespace surface {

			_base::_base(
				__in_opt const glm::vec3 &position,
				__in_opt const glm::vec3 &rotation,
				__in_opt const glm::vec3 &up,
				__in_opt bool visible
				) :
					gaea::engine::entity::base_model(ENTITY_SURFACE, ENTITY_SPECIFIER_UNDEFINED,
						position, rotation, up, GL_HANDLE_INVALID, GL_HANDLE_INVALID, GL_HANDLE_INVALID, 
						visible),
					m_indicies(0)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::entity::base_model(other),
					m_indicies(other.m_indicies),
					m_program(other.m_program),
					m_vao(other.m_vao)
			{
				return;
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
					gaea::engine::entity::base_model::operator=(other);
					m_indicies = other.m_indicies;
					m_program = other.m_program;
					m_vao = other.m_vao;
				}
	
				return *this;
			}

			void 
			_base::add_attribute(
				__in const GLvoid *data,
				__in GLsizeiptr size,
				__in GLenum target,
				__in GLenum usage,
				__in GLuint index,
				__in GLint element_size,
				__in GLenum element_type,
				__in_opt GLboolean normalized,
				__in_opt GLsizei stride,
				__in_opt const GLvoid *pointer
				)
			{
				m_vao.add_attribute(data, size, target, usage, index, element_size, element_type, 
					normalized, stride, pointer);
			}

			void 
			_base::add_data(
				__in const GLvoid *data,
				__in GLsizeiptr size,
				__in GLenum target,
				__in GLenum usage
				)
			{
				m_vao.add_data(data, size, target, usage);
			}

			std::string 
			_base::as_string(
				__in const _base &object,
				__in_opt bool verbose
				)
			{
				std::stringstream result;

				result << gaea::engine::entity::base_model::as_string(object, verbose)
					<< ", " << gaea::graphics::program::base::as_string(object.m_program, verbose)
					<< ", " << gaea::graphics::vao::base::as_string(object.m_vao, verbose)
					<< ", IDC=" << object.m_indicies;

				return result.str();
			}

			void 
			_base::link(
				__in const std::string &vertex,
				__in const std::string &fragment,
				__in_opt const std::vector<std::string> &attribute
				)
			{
				m_program.link(vertex, fragment, attribute);
				m_model_id = m_program.uniform_location(ENTITY_UNIFORM_MODEL);
				m_projection_id = m_program.uniform_location(ENTITY_UNIFORM_PROJECTION);
				m_view_id = m_program.uniform_location(ENTITY_UNIFORM_VIEW);
			}

			void 
			_base::render(
				__in const glm::vec3 &position,
				__in const glm::vec3 &rotation,
				__in const glm::vec3 &up,
				__in const glm::mat4 &projection,
				__in const glm::mat4 &view
				)
			{
				update_model();
				m_program.start();
				m_program.set_uniform(m_model_id, m_model);
				m_program.set_uniform(m_projection_id, projection);
				m_program.set_uniform(m_view_id, view);
				m_vao.start();
				GL_CHECK(glDrawArrays, GL_TRIANGLES, 0, m_indicies);
				m_vao.stop();
				m_program.stop();
			}

			void 
			_base::set_indicies(
				__in GLuint indicies
				)
			{
				m_indicies = indicies;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::surface::base::as_string(*this, verbose);
			}

			void 
			_base::update(
				__in GLfloat delta
				)
			{
				return;
			}
		}
	}
}
