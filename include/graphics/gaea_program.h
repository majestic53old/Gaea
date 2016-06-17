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

#ifndef GAEA_PROGRAM_H_
#define GAEA_PROGRAM_H_

namespace gaea {

	namespace gl {

		namespace program {

			typedef class _base :
					public gaea::gl::base {

				public:

					_base(void);

					_base(
						__in const std::string &vertex,
						__in const std::string &fragment
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

					void set_bool(
						__in GLint location,
						__in GLboolean value
						);

					void set_float(
						__in GLint location,
						__in GLfloat value
						);

					void set_int(
						__in GLint location,
						__in GLint value
						);

					void set_ivec2(
						__in GLint location,
						__in const glm::ivec2 &value
						);

					void set_ivec3(
						__in GLint location,
						__in const glm::ivec3 &value
						);

					void set_ivec4(
						__in GLint location,
						__in const glm::ivec4 &value
						);

					void set_mat4(
						__in GLint location,
						__in const glm::mat4 &value
						);

					void set_uint(
						__in GLint location,
						__in GLuint value
						);

					void set_uvec2(
						__in GLint location,
						__in const glm::uvec2 &value
						);

					void set_uvec3(
						__in GLint location,
						__in const glm::uvec3 &value
						);

					void set_uvec4(
						__in GLint location,
						__in const glm::uvec4 &value
						);

					void set_vec2(
						__in GLint location,
						__in const glm::vec2 &value
						);

					void set_vec3(
						__in GLint location,
						__in const glm::vec3 &value
						);

					void set_vec4(
						__in GLint location,
						__in const glm::vec4 &value
						);

					void setup(
						__in const std::string &vertex,
						__in const std::string &fragment
						);

					void start(void);

					void stop(void);

					std::string to_string(
						__in_opt bool verbose = false
						);

					GLint uniform_location(
						__in const std::string &name
						);

			} base;
		}
	}
}

#endif // GAEA_PROGRAM_H_
