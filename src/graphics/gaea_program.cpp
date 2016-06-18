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
#include "gaea_program_type.h"

namespace gaea {

	namespace gl {

		namespace program {

			#define PROGRAM_ERROR_MALFORMED "Malformed program error"
			#define PROGRAM_ERROR_UNKNOWN "Unknown program error"

			_base::_base(
				__in const std::string &vertex,
				__in const std::string &fragment,
				__in_opt const std::vector<std::string> &attribute
				) :
					gaea::gl::base(GL_OBJECT_PROGRAM)
			{
				link(vertex, fragment, attribute);
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
			_base::link(
				__in const std::string &vertex,
				__in const std::string &fragment,
				__in_opt const std::vector<std::string> &attribute
				)
			{
				GLuint index = 0;
				std::string buffer;
				GLint complete, length;
				std::vector<gaea::gl::shader::base> shader;
				std::vector<std::string>::const_iterator attrib_iter;
				std::vector<gaea::gl::shader::base>::iterator shader_iter;

				shader.push_back(gaea::gl::shader::base(vertex, GL_VERTEX_SHADER));
				GL_CHECK(glAttachShader, m_handle, shader.back().handle());
				shader.push_back(gaea::gl::shader::base(fragment, GL_FRAGMENT_SHADER));
				GL_CHECK(glAttachShader, m_handle, shader.back().handle());

				for(attrib_iter = attribute.begin(); attrib_iter != attribute.end(); 
						++attrib_iter) {
					GL_CHECK(glBindAttribLocation, m_handle, index++, attrib_iter->c_str());
				}

				GL_CHECK(glLinkProgram, m_handle);
				GL_CHECK(glGetProgramiv, m_handle, GL_LINK_STATUS, &complete);

				if(!complete) {
					GL_CHECK(glGetProgramiv, m_handle, GL_INFO_LOG_LENGTH, &length);

					if(length > 0) {
						buffer.resize(++length, 0);
						GL_CHECK(glGetProgramInfoLog, m_handle, length, &length, &buffer[0]);
					}

					if(!length) {
						buffer = PROGRAM_ERROR_UNKNOWN;
					} else if(length < 0) {
						buffer = PROGRAM_ERROR_MALFORMED;
					}

					THROW_GAEA_PROGRAM_EXCEPTION_FORMAT(GAEA_PROGRAM_EXCEPTION_EXTERNAL,
						"%s", STRING_CHECK(buffer));
				}

				for(shader_iter = shader.begin(); shader_iter != shader.end(); 
						++shader_iter) {
					GL_CHECK(glDetachShader, m_handle, shader_iter->handle());
				}
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in GLboolean value
				)
			{
				GL_CHECK(glUniform1ui, location, (GLuint) value);
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in GLfloat value
				)
			{
				GL_CHECK(glUniform1f, location, value);
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in GLint value
				)
			{
				GL_CHECK(glUniform1i, location, value);
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in GLuint value
				)
			{
				GL_CHECK(glUniform1ui, location, value);
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::vec2 &value
				)
			{
				GL_CHECK(glUniform2fv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::vec3 &value
				)
			{
				GL_CHECK(glUniform3fv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::vec4 &value
				)
			{
				GL_CHECK(glUniform4fv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::ivec2 &value
				)
			{
				GL_CHECK(glUniform2iv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::ivec3 &value
				)
			{
				GL_CHECK(glUniform3iv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::ivec4 &value
				)
			{
				GL_CHECK(glUniform4iv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::uvec2 &value
				)
			{
				GL_CHECK(glUniform2uiv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::uvec3 &value
				)
			{
				GL_CHECK(glUniform3uiv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::uvec4 &value
				)
			{
				GL_CHECK(glUniform4uiv, location, OBJECT_COUNT, glm::value_ptr(value));
			}

			void 
			_base::set_uniform(
				__in GLint location,
				__in const glm::mat4 &value
				)
			{
				GL_CHECK(glUniformMatrix4fv, location, OBJECT_COUNT, GL_FALSE, glm::value_ptr(value));
			}

			void 
			_base::start(void)
			{
				GL_CHECK(glUseProgram, m_handle);
			}

			void 
			_base::stop(void)
			{
				GL_CHECK(glUseProgram, GL_HANDLE_INVALID);
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::gl::program::base::as_string(*this, verbose);
			}

			GLint 
			_base::uniform_location(
				__in const std::string &name
				)
			{
				GLint result = GL_HANDLE_INVALID;

				GL_CHECK_RESULT(result, glGetUniformLocation, m_handle, name.c_str());

				return result;
			}
		}
	}
}
