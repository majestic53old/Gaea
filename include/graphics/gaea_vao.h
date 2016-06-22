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

#ifndef GAEA_VAO_H_
#define GAEA_VAO_H_

namespace gaea {

	#define VAO_NORMALIZED_INIT GL_FALSE
	#define VAO_POINTER_INIT nullptr
	#define VAO_STRIDE_INIT 0

	namespace graphics {

		namespace vao {

			typedef class _base :
					public gaea::graphics::base {

				public:

					_base(void);

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

					void start(void);

					void stop(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

				protected:

					std::vector<GLuint> m_index;

					std::vector<gaea::graphics::vbo::base> m_vbo;

			} base;
		}
	}
}

#endif // GAEA_VAO_H_
