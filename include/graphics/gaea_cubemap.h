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

#ifndef GAEA_CUBEMAP_H_
#define GAEA_CUBEMAP_H_

namespace gaea {

	#define CUBEMAP_FILTER_MAG_INIT GL_LINEAR
	#define CUBEMAP_FILTER_MIN_INIT GL_LINEAR
	#define CUBEMAP_WRAP_R_INIT GL_CLAMP_TO_EDGE
	#define CUBEMAP_WRAP_S_INIT GL_CLAMP_TO_EDGE
	#define CUBEMAP_WRAP_T_INIT GL_CLAMP_TO_EDGE

	namespace gl {

		namespace cubemap {

			typedef class _base :
					public gaea::gl::base {

				public:

					_base(
						__in const std::vector<std::string> &face,
						__in_opt GLint filter_mag = CUBEMAP_FILTER_MAG_INIT,
						__in_opt GLint filter_min = CUBEMAP_FILTER_MIN_INIT,
						__in_opt GLint wrap_s = CUBEMAP_WRAP_S_INIT,
						__in_opt GLint wrap_t = CUBEMAP_WRAP_T_INIT,
						__in_opt GLint wrap_r = CUBEMAP_WRAP_R_INIT
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

			} base;
		}
	}
}

#endif // GAEA_CUBEMAP_H_
