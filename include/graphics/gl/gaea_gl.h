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

#ifndef GAEA_GL_H_
#define GAEA_GL_H_

namespace gaea {

	#define GL_HANDLE_INVALID 0
	#define GL_INVALID SCALAR_INVALID(gaea::gl_type_t)
	#define GL_OBJECT_MAX GL_OBJECT_VBO
	#define GL_TARGET_UNDEFINED 0

	typedef enum {
		GL_OBJECT_CUBEMAP = 0,
		GL_OBJECT_PROGRAM,
		GL_OBJECT_SHADER,
		GL_OBJECT_TEXTURE,
		GL_OBJECT_VAO,
		GL_OBJECT_VBO,
	} gl_type_t;

	namespace graphics {

		namespace gl {

			typedef class _base :
					public gaea::engine::object::base {

				public:

					_base(
						__in gaea::gl_type_t type,
						__in_opt GLenum target = GL_TARGET_UNDEFINED
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

					GLuint handle(void);

					GLenum target(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					virtual void start(void) = 0;

					virtual void stop(void) = 0;

					gaea::gl_type_t type(void);

				protected:

					void decrement_reference(void);

					void generate(void);

					void increment_reference(void);

					GLuint m_handle;

					GLenum m_target;

			} base;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type
						);

					size_t decrement_reference(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type
						);

					GLuint generate(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type,
						__in GLenum target
						);

					size_t increment_reference(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type
						);

					size_t size(void);

					size_t size(
						__in gaea::gl_type_t type
						);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

				protected:

					_manager(void);

					_manager(
						__in const _manager &other
						);

					_manager &operator=(
						__in const _manager &other
						);

					static void _delete(void);

					void clear(void);

					GLuint create(
						__in gaea::gl_type_t type,
						__in GLenum target
						);

					void destroy(
						__in const std::map<gaea::uid_t, std::tuple<gaea::gl_type_t, GLenum, GLuint, size_t>>::iterator &entry
						);

					std::map<gaea::uid_t, std::tuple<gaea::gl_type_t, GLenum, GLuint, size_t>>::iterator find(
						__in const gaea::uid_t &id,
						__in gaea::gl_type_t type
						);

					std::vector<std::map<gaea::uid_t, std::tuple<gaea::gl_type_t, GLenum, GLuint, size_t>>> m_entry;

					bool m_initialized;

					static _manager *m_instance;

			} manager;
		}
	}
}

#endif // GAEA_GL_H_
