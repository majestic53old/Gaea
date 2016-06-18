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
#include "gaea_vbo_type.h"

namespace gaea {

	namespace gl {

		namespace vbo {

			_base::_base(
				__in const GLvoid *data,
				__in GLsizeiptr size,
				__in GLenum target,
				__in GLenum usage
				) :
					gaea::gl::base(GL_OBJECT_VBO, target)
			{
				setup(data, size, target, usage);
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::gl::base(other)
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
				}

				return *this;
			}

			std::string 
			_base::as_string(
				__in const _base &object,
				__in_opt bool verbose
				)
			{
				return gaea::gl::base::as_string(object, verbose);
			}

			void 
			_base::setup(
				__in const GLvoid *data,
				__in GLsizeiptr size,
				__in GLenum target,
				__in GLenum usage
				)
			{
				GL_CHECK(glBindBuffer, m_target, m_handle);
				GL_CHECK(glBufferData, target, size, data, usage);
			}

			void 
			_base::start(void)
			{
				GL_CHECK(glBindBuffer, m_target, m_handle);
			}

			void 
			_base::stop(void)
			{
				GL_CHECK(glBindBuffer, m_target, GL_HANDLE_INVALID);
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::gl::vbo::base::as_string(*this, verbose);
			}
		}
	}
}
