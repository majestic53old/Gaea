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
#include "gaea_cubemap_type.h"

namespace gaea {

	namespace graphics {

		namespace cubemap {

			_base::_base(void) :
				gaea::graphics::base(GL_OBJECT_CUBEMAP),
				m_index(CUBEMAP_INDEX_INIT)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::graphics::base(other),
					m_index(other.m_index)
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
					gaea::graphics::base::operator=(other);
					m_index = other.m_index;
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

				result << gaea::graphics::base::as_string(object, verbose)
					<< ", IDX=" << SCALAR_AS_HEX(GLuint, object.m_index);

				return result.str();
			}

			void 
			_base::load(
				__in const std::vector<std::string> &face,
				__in gaea::image_t type,
				__in_opt GLint filter_mag,
				__in_opt GLint filter_min,
				__in_opt GLint wrap_s,
				__in_opt GLint wrap_t,
				__in_opt GLint wrap_r,
				__in_opt GLint level,
				__in_opt GLint border,
				__in_opt GLuint index
				)
			{
				size_t iter = 0;
				uint8_t color, depth;
				glm::ivec2 dimensions;
				std::vector<uint8_t> data;

				if(face.size() < CUBEMAP_FACE_COUNT) {
					THROW_GAEA_CUBEMAP_EXCEPTION_FORMAT(GAEA_CUBEMAP_EXCEPTION_MISSING,
						"%u faces (expecting %u)", face.size(), CUBEMAP_FACE_COUNT);
				}

				m_index = index;
				GL_CHECK(glActiveTexture, m_index);
				GL_CHECK(glBindTexture, GL_TEXTURE_CUBE_MAP, m_handle);

				for(; iter < CUBEMAP_FACE_COUNT; ++iter) {
					gaea::graphics::texture::import(face.at(iter), type, data, dimensions, color, depth);
					GL_CHECK(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum) iter, 
						level, GL_RGBA, dimensions.x, dimensions.y, border, GL_RGBA, 
						GL_UNSIGNED_BYTE, &data[0]);
				}

				GL_CHECK(glGenerateMipmap, GL_TEXTURE_CUBE_MAP);
				GL_CHECK(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_mag);
				GL_CHECK(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_min);
				GL_CHECK(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
				GL_CHECK(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
				GL_CHECK(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
			}

			void 
			_base::start(void)
			{
				GL_CHECK(glActiveTexture, m_index);
				GL_CHECK(glBindTexture, GL_TEXTURE_CUBE_MAP, m_handle);
			}

			void 
			_base::stop(void)
			{
				GL_CHECK(glBindTexture, GL_TEXTURE_CUBE_MAP, GL_HANDLE_INVALID);
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::graphics::cubemap::base::as_string(*this, verbose);
			}
		}
	}
}
