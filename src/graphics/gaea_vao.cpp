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
#include "gaea_vao_type.h"

namespace gaea {

	namespace gl {

		namespace vao {

			_base::_base(void) :
				gaea::gl::base(GL_OBJECT_VAO)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::gl::base(other),
					m_index(other.m_index),
					m_vbo(other.m_vbo)
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
					gaea::gl::base::operator=(other);
					m_index = other.m_index;
					m_vbo = other.m_vbo;
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
				std::vector<GLuint>::const_iterator index_iter;
				std::vector<gaea::gl::vbo::base>::const_iterator vbo_iter;

				result << gaea::gl::base::as_string(object, verbose)
					<< ", IDX[" << object.m_index.size() << "]";

				if(!object.m_index.empty()) {
					result << "={";

					for(index_iter = object.m_index.begin(); index_iter != object.m_index.end(); 
							++index_iter) {

						if(index_iter != object.m_index.begin()) {
							result << ", ";
						}

						result << *index_iter;
					}

					result << "}";
				}

				result << ", VBO[" << object.m_vbo.size() << "]";

				if(!object.m_vbo.empty()) {
					result << "={";

					for(vbo_iter = object.m_vbo.begin(); vbo_iter != object.m_vbo.end();
							++vbo_iter) {

						if(vbo_iter != object.m_vbo.begin()) {
							result << ", ";
						}

						result << gaea::gl::vbo::base::as_string(*vbo_iter, verbose);
					}

					result << "}";
				}

				return result.str();
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
				GL_CHECK(glBindVertexArray, m_handle);
				m_vbo.push_back(gaea::gl::vbo::base(data, size, target, usage));
				GL_CHECK(glVertexAttribPointer, index, element_size, element_type, 
					normalized, stride, pointer);
				m_index.push_back(index);
			}

			void 
			_base::add_data(
				__in const GLvoid *data,
				__in GLsizeiptr size,
				__in GLenum target,
				__in GLenum usage
				)
			{
				GL_CHECK(glBindVertexArray, m_handle);
				m_vbo.push_back(gaea::gl::vbo::base(data, size, target, usage));
			}

			void 
			_base::start(void)
			{
				std::vector<GLuint>::const_iterator iter;

				GL_CHECK(glBindVertexArray, m_handle);

				for(iter = m_index.begin(); iter != m_index.end(); ++iter) {
					GL_CHECK(glEnableVertexAttribArray, *iter);
				}
			}

			void 
			_base::stop(void)
			{
				std::vector<GLuint>::const_iterator iter;

				for(iter = m_index.begin(); iter != m_index.end(); ++iter) {
					GL_CHECK(glDisableVertexAttribArray, *iter);
				}

				GL_CHECK(glBindVertexArray, GL_HANDLE_INVALID);
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::gl::vao::base::as_string(*this, verbose);
			}
		}
	}
}
