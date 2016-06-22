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
#include "gaea_texture_type.h"

namespace gaea {

	namespace graphics {

		namespace texture {

			#define IMAGE_COLOR_INVALID SCALAR_INVALID(png_byte)
			#define IMAGE_DEPTH_8 8
			#define IMAGE_DEPTH_16 16
			#define IMAGE_DEPTH_INVALID SCALAR_INVALID(png_byte)

			void 
			import_cleanup_png(
				__inout FILE **file,
				__inout png_structpp reader,
				__inout png_infopp reader_info
				)
			{

				if(reader) {

					if(*reader) {
						png_destroy_read_struct(reader, *reader_info ? reader_info : nullptr, 
							nullptr);
					}

					reader = nullptr;
				}

				if(reader_info) {
					reader_info = nullptr;
				}

				if(file) {

					if(*file) {
						std::fclose(*file);
					}

					file = nullptr;
				}
			}

			void 
			import_setup_png(
				__in const std::string &texture,
				__out png_byte &color,
				__out png_byte &depth,
				__out glm::ivec2 &dimensions,
				__out FILE **file,
				__out png_structpp reader,
				__out png_infopp reader_info
				)
			{

				if(!file || !reader || !reader_info) {
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_INVALID,
						"file=%p, reader=%p, reader_info=%p", file, reader, reader_info);
				}

				*reader = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				if(!*reader) {
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
						"png_create_read_struct failed: %p", *reader);
				}

				*reader_info = png_create_info_struct(*reader);
				if(!*reader_info) {
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
						"png_create_read_struct failed: %p", *reader_info);
				}

				*file = std::fopen(texture.c_str(), "rb");
				if(!*file) {
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_NOT_FOUND,
						"%s", STRING_CHECK(texture));
				}

				if(setjmp(png_jmpbuf(*reader))) {
					import_cleanup_png(file, reader, reader_info);
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
						"png_jmpbuf failed: %p", *reader);
				}

				png_init_io(*reader, *file);
				png_read_info(*reader, *reader_info);

				dimensions.x = png_get_image_width(*reader, *reader_info);
				if(!dimensions.x) {
					import_cleanup_png(file, reader, reader_info);
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
						"png_get_image_width failed: %u (must be >0)", dimensions.x);
				}

				dimensions.y = png_get_image_height(*reader, *reader_info);
				if(!dimensions.y) {
					import_cleanup_png(file, reader, reader_info);
					THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
						"png_get_image_height failed: %u (must be >0)", dimensions.y);
				}

				depth = png_get_bit_depth(*reader, *reader_info);
				if(depth == IMAGE_DEPTH_16) {
					png_set_strip_16(*reader);
				}

				color = png_get_color_type(*reader, *reader_info);
				switch(color) {
					case PNG_COLOR_TYPE_GRAY:

						if(depth < IMAGE_DEPTH_8) {
							png_set_expand_gray_1_2_4_to_8(*reader);
						}
						break;
					case PNG_COLOR_TYPE_PALETTE:
						png_set_palette_to_rgb(*reader);
						break;
					default:
						break;
				}

				if(png_get_valid(*reader, *reader_info, PNG_INFO_tRNS)) {
					png_set_tRNS_to_alpha(*reader);
				}

				switch(color) {
					case PNG_COLOR_TYPE_GRAY:
						png_set_filler(*reader, UINT8_MAX, PNG_FILLER_AFTER);
					case PNG_COLOR_TYPE_GRAY_ALPHA:
						png_set_gray_to_rgb(*reader);
						break;
					case PNG_COLOR_TYPE_PALETTE:
					case PNG_COLOR_TYPE_RGB:
						png_set_filler(*reader, UINT8_MAX, PNG_FILLER_AFTER);
						break;
					default:
						break;
				}

				png_read_update_info(*reader, *reader_info);
				color = PNG_COLOR_TYPE_RGB_ALPHA;
				depth = IMAGE_DEPTH_8;
			}

			void 
			import(
				__in const std::string &texture,
				__in gaea::image_t type,
				__out std::vector<uint8_t> &data,
				__out glm::ivec2 &dimensions,
				__out uint8_t &color,
				__out uint8_t &depth
				)
			{
				size_t iter = 0;
				png_uint_32 width;
				FILE *file = nullptr;
				png_structp reader = nullptr;
				png_infop reader_info = nullptr;
				std::vector<png_bytep> data_ptr;

				data.clear();
				dimensions = glm::ivec2();
				color = IMAGE_COLOR_INVALID;
				depth = IMAGE_DEPTH_INVALID;

				switch(type) {
					case IMAGE_PNG:
						import_setup_png(texture, color, depth, dimensions, &file, &reader, &reader_info);

						width = png_get_rowbytes(reader, reader_info);
						if(!width) {
							THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_EXTERNAL,
								"png_get_rowbytes failed: %u (must be >0)", width);
						}

						data.resize(width * dimensions.y, 0);
						data_ptr.resize(dimensions.y, nullptr);

						for(; iter < data_ptr.size(); ++iter) {
							data_ptr.at(iter) = ((&data[0]) + (width * iter));
						}

						png_read_image(reader, &data_ptr[0]);
						import_cleanup_png(&file, &reader, &reader_info);
						break;
					default:
						THROW_GAEA_TEXTURE_EXCEPTION_FORMAT(GAEA_TEXTURE_EXCEPTION_UNSUPPORTED,
							"%s (%x)", STRING_CHECK(texture), type);
				}
			}

			_base::_base(
				__in const std::string &texture,
				__in gaea::image_t type,
				__in_opt GLint filter_mag,
				__in_opt GLint filter_min,
				__in_opt GLint wrap_s,
				__in_opt GLint wrap_t,
				__in_opt GLint level,
				__in_opt GLint border,
				__in_opt GLuint index
				) :
					gaea::graphics::base(GL_OBJECT_TEXTURE),
					m_index(index)
			{
				load(texture, type, filter_mag, filter_min, wrap_s, wrap_t, level, border);
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
				__in const std::string &texture,
				__in gaea::image_t type,
				__in_opt GLint filter_mag,
				__in_opt GLint filter_min,
				__in_opt GLint wrap_s,
				__in_opt GLint wrap_t,
				__in_opt GLint level,
				__in_opt GLint border
				)
			{
				uint8_t color, depth;
				glm::ivec2 dimensions;
				std::vector<uint8_t> data;

				GL_CHECK(glActiveTexture, m_index);
				GL_CHECK(glBindTexture, GL_TEXTURE_2D, m_handle);
				gaea::graphics::texture::import(texture, type, data, dimensions, color, depth);
				GL_CHECK(glTexImage2D, GL_TEXTURE_2D, level, GL_RGBA, dimensions.x, dimensions.y, 
					border, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
				GL_CHECK(glGenerateMipmap, GL_TEXTURE_2D);
				GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
				GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
				GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
				GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			}

			void 
			_base::start(void)
			{
				GL_CHECK(glActiveTexture, m_index);
				GL_CHECK(glBindTexture, GL_TEXTURE_2D, m_handle);
			}

			void 
			_base::stop(void)
			{
				GL_CHECK(glBindTexture, GL_TEXTURE_2D, GL_HANDLE_INVALID);
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::graphics::texture::base::as_string(*this, verbose);
			}
		}
	}
}
