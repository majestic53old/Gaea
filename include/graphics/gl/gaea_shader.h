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

#ifndef GAEA_SHADER_H_
#define GAEA_SHADER_H_

namespace gaea {

	namespace graphics {

		namespace gl {

			namespace shader {

				typedef class _base :
						public gaea::graphics::gl::base {

					public:

						_base(
							__in const std::string &shader,
							__in GLenum type
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

						void start(void);

						void stop(void);

						std::string to_string(
							__in_opt bool verbose = false
							);

					protected:

						void compile(
							__in const std::string &shader
							);	

				} base;
			}
		}
	}
}

#endif // GAEA_SHADER_H_