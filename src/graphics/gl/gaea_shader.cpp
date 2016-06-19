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

#include <fstream>
#include "../../../include/gaea.h"
#include "gaea_shader_type.h"

namespace gaea {

	namespace graphics {

		namespace gl {

			namespace shader {

				#define SHADER_ERROR_MALFORMED "Malformed shader error"
				#define SHADER_ERROR_UNKNOWN "Unknown shader error"

				_base::_base(
					__in const std::string &shader,
					__in GLenum type
					) :
						gaea::graphics::gl::base(GL_OBJECT_SHADER, type)
				{
					compile(shader);
				}

				_base::_base(
					__in const _base &other
					) :
						gaea::graphics::gl::base(other)
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
						gaea::graphics::gl::base::operator=(other);
					}

					return *this;
				}

				std::string 
				_base::as_string(
					__in const _base &object,
					__in_opt bool verbose
					)
				{
					return gaea::graphics::gl::base::as_string(object, verbose);
				}

				void 
				_base::compile(
					__in const std::string &shader
					)
				{				
					std::string buffer;
					std::ifstream file;
					GLint complete, length;
					const GLchar *source = nullptr;

					file = std::ifstream(shader.c_str(), std::ios::in);
					if(!file) {
						THROW_GAEA_SHADER_EXCEPTION_FORMAT(GAEA_SHADER_EXCEPTION_NOT_FOUND,
							"%s", STRING_CHECK(shader));
					}

					file.seekg(0, std::ios::end);
					length = file.tellg();
					file.seekg(0, std::ios::beg);

					if(length > 0) {
						buffer.resize(++length, 0);
						file.read(&buffer[0], length);
					}

					file.close();

					if(length < 0) {
						THROW_GAEA_SHADER_EXCEPTION_FORMAT(GAEA_SHADER_EXCEPTION_MALFORMED,
							"%s", STRING_CHECK(shader));
					}

					source = (const GLchar *) &buffer[0];
					GL_CHECK(glShaderSource, m_handle, OBJECT_COUNT, &source, &length);
					GL_CHECK(glCompileShader, m_handle);
					GL_CHECK(glGetShaderiv, m_handle, GL_COMPILE_STATUS, &complete);

					if(!complete) {
						GL_CHECK(glGetShaderiv, m_handle, GL_INFO_LOG_LENGTH, &length);

						if(length > 0) {
							buffer.resize(++length, 0);
							GL_CHECK(glGetShaderInfoLog, m_handle, length, &length, &buffer[0]);
						}

						if(!length) {
							buffer = SHADER_ERROR_UNKNOWN;
						} else if(length < 0) {
							buffer = SHADER_ERROR_MALFORMED;
						}

						THROW_GAEA_SHADER_EXCEPTION_FORMAT(GAEA_SHADER_EXCEPTION_EXTERNAL,
							"%s", STRING_CHECK(buffer));
					}
				}

				void 
				_base::start(void)
				{
					return;
				}

				void 
				_base::stop(void)
				{
					return;
				}

				std::string 
				_base::to_string(
					__in_opt bool verbose
					)
				{
					return gaea::graphics::gl::shader::base::as_string(*this, verbose);
				}
			}
		}
	}
}
