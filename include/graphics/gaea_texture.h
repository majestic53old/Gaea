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

#ifndef GAEA_TEXTURE_H_
#define GAEA_TEXTURE_H_

namespace gaea {

	#define TEXTURE_FILTER_MAG_INIT GL_LINEAR
	#define TEXTURE_FILTER_MIN_INIT GL_LINEAR
	#define TEXTURE_WRAP_R_INIT GL_CLAMP_TO_EDGE
	#define TEXTURE_WRAP_S_INIT GL_CLAMP_TO_EDGE
	#define TEXTURE_WRAP_T_INIT GL_CLAMP_TO_EDGE

	namespace gl {

		namespace texture {

			typedef class _base :
					public gaea::gl::base {

				public:

					_base(void);

					_base(
						__in const std::string &texture,
						__in_opt GLint filter_mag = TEXTURE_FILTER_MAG_INIT,
						__in_opt GLint filter_min = TEXTURE_FILTER_MIN_INIT,
						__in_opt GLint wrap_s = TEXTURE_WRAP_S_INIT,
						__in_opt GLint wrap_t = TEXTURE_WRAP_T_INIT,
						__in_opt GLint wrap_r = TEXTURE_WRAP_R_INIT
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

					void setup(
						__in const std::string &texture,
						__in_opt GLint filter_mag = TEXTURE_FILTER_MAG_INIT,
						__in_opt GLint filter_min = TEXTURE_FILTER_MIN_INIT,
						__in_opt GLint wrap_s = TEXTURE_WRAP_S_INIT,
						__in_opt GLint wrap_t = TEXTURE_WRAP_T_INIT,
						__in_opt GLint wrap_r = TEXTURE_WRAP_R_INIT
						);

					void start(void);

					void stop(void);

					std::string to_string(
						__in_opt bool verbose = false
						);

			} base;
		}
	}
}

#endif // GAEA_TEXTURE_H_
