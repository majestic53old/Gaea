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
#include "gaea_gl_type.h"

namespace gaea {

	namespace graphics {

		#define GL_OBJECT_STRING(_TYPE_) \
			((_TYPE_) > GL_OBJECT_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(GL_OBJECT_STR[_TYPE_]))

		static const std::string GL_OBJECT_STR[] = {
			"CUBEMAP", "PROGRAM", "SHADER", "TEXTURE", "VAO", "VBO",
			};

		enum {
			GL_TUPLE_TYPE = 0,
			GL_TUPLE_TARGET,
			GL_TUPLE_HANDLE,
			GL_TUPLE_REFERENCE,
		};

		_base::_base(
			__in gaea::gl_t type,
			__in_opt GLenum target
			) :
				gaea::engine::object::base(OBJECT_GRAPHIC, type),
				m_handle(GL_HANDLE_INVALID),
				m_target(target)
		{
			generate();
		}

		_base::_base(
			__in const _base &other
			) :
				gaea::engine::object::base(other),
				m_handle(other.m_handle),
				m_target(other.m_target)
		{
			increment_reference();
		}

		_base::~_base(void)
		{
			decrement_reference();
		}

		_base &
		_base::operator=(
			__in const _base &other
			)
		{

			if(this != &other) {
				decrement_reference();
				gaea::engine::object::base::operator=(other);
				m_handle = other.m_handle;
				m_target = other.m_target;
				increment_reference();
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

			result << gaea::engine::uid::base::as_string(object, verbose)
				<< " [" << GL_OBJECT_STRING(object.m_subtype);

			if(object.m_target != GL_TARGET_UNDEFINED) {
				result << ", " << SCALAR_AS_HEX(GLenum, object.m_target);
			}

			result << "]";

			if(object.m_handle) {
				result << " HDL=" << SCALAR_AS_HEX(GLuint, object.m_handle);
			}

			return result.str();
		}

		void 
		_base::decrement_reference(void)
		{

			if(gaea::graphics::manager::is_allocated()) {

				gaea::graphics::manager &instance = gaea::graphics::manager::acquire();
				if(instance.is_initialized() 
						&& instance.contains(m_id, (gaea::gl_t) m_subtype)) {
					instance.decrement_reference(m_id, (gaea::gl_t) m_subtype);
				}
			}
		}

		void 
		_base::generate(void)
		{

			if(gaea::graphics::manager::is_allocated()) {

				gaea::graphics::manager &instance = gaea::graphics::manager::acquire();
				if(instance.is_initialized()) {
					m_handle = instance.generate(m_id, (gaea::gl_t) m_subtype, m_target);
				}
			}
		}

		GLuint 
		_base::handle(void)
		{
			return m_handle;
		}

		void 
		_base::increment_reference(void)
		{

			if(gaea::graphics::manager::is_allocated()) {

				gaea::graphics::manager &instance = gaea::graphics::manager::acquire();
				if(instance.is_initialized() 
						&& instance.contains(m_id, (gaea::gl_t) m_subtype)) {
					instance.increment_reference(m_id, (gaea::gl_t) m_subtype);
				}
			}
		}

		GLenum 
		_base::target(void)
		{
			return m_target;
		}

		std::string 
		_base::to_string(
			__in_opt bool verbose
			)
		{
			return gaea::graphics::base::as_string(*this, verbose);
		}

		gaea::gl_t 
		_base::type(void)
		{
			return (gaea::gl_t) gaea::engine::object::base::subtype();
		}

		_manager *_manager::m_instance = nullptr;

		_manager::_manager(void) :
			m_initialized(false)
		{
			std::atexit(gaea::graphics::manager::_delete);
		}

		_manager::~_manager(void)
		{
			uninitialize();
		}

		void 
		_manager::_delete(void)
		{

			if(gaea::graphics::manager::m_instance) {
				delete gaea::graphics::manager::m_instance;
				gaea::graphics::manager::m_instance = nullptr;
			}
		}

		_manager &
		_manager::acquire(void)
		{

			if(!gaea::graphics::manager::m_instance) {

				gaea::graphics::manager::m_instance = new gaea::graphics::manager;
				if(!gaea::graphics::manager::m_instance) {
					THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_ALLOCATED);
				}
			}

			return *gaea::graphics::manager::m_instance;
		}

		void 
		_manager::clear(void)
		{
			std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator entry_iter;
			std::vector<std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>>::iterator type_iter;

			for(type_iter = m_entry.begin(); type_iter != m_entry.end(); ++type_iter) {

				for(entry_iter = type_iter->begin(); entry_iter != type_iter->end(); ++entry_iter) {
					destroy(entry_iter);
				}

				type_iter->clear();
			}

			m_entry.clear();
		}

		bool 
		_manager::contains(
			__in gaea::uid_t &id,
			__in gaea::gl_t type
			)
		{

			if(!m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_UNINITIALIZED);
			}

			if(type >= m_entry.size()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID,
					"%x", type);
			}

			return (m_entry.at(type).find(id) != m_entry.at(type).end());
		}

		GLuint 
		_manager::create(
			__in gaea::gl_t type,
			__in GLenum target
			)
		{
			std::string name;
			GLuint result = GL_HANDLE_INVALID;

			switch(type) {
				case GL_OBJECT_CUBEMAP:
				case GL_OBJECT_TEXTURE:
					GL_CHECK(glGenTextures, OBJECT_COUNT, &result);
					name = STRING_CONCAT(glGenTextures);
					break;
				case GL_OBJECT_PROGRAM:
					GL_CHECK_RESULT(result, glCreateProgram);
					name = STRING_CONCAT(glCreateProgram);
					break;
				case GL_OBJECT_SHADER:
					GL_CHECK_RESULT(result, glCreateShader, target);
					name = STRING_CONCAT(glCreateShader);
					break;
				case GL_OBJECT_VAO:
					GL_CHECK(glGenVertexArrays, OBJECT_COUNT, &result);
					name = STRING_CONCAT(glGenVertexArrays);
					break;
				case GL_OBJECT_VBO:
					GL_CHECK(glGenBuffers, OBJECT_COUNT, &result);
					name = STRING_CONCAT(glGenBuffers);
					break;
				default:
					THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID, "%x", type);
			}

			if(!result) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_EXTERNAL, "%s failed",
					STRING_CHECK(name));
			}

			return result;
		}

		size_t 
		_manager::decrement_reference(
			__in gaea::uid_t &id,
			__in gaea::gl_t type
			)
		{
			size_t result = 0;
			std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator iter;

			iter = find(id, type);
			if(TUPLE_ENTRY(iter->second, GL_TUPLE_REFERENCE) <= REFERENCE_INIT) {
				destroy(iter);
				m_entry.at(type).erase(iter);
			} else {
				result = --TUPLE_ENTRY(iter->second, GL_TUPLE_REFERENCE);
			}

			return result;
		}

		void 
		_manager::destroy(
			__in const std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator &entry
			)
		{
			GLuint handle;

			handle = TUPLE_ENTRY(entry->second, GL_TUPLE_HANDLE);

			switch(TUPLE_ENTRY(entry->second, GL_TUPLE_TYPE)) {
				case GL_OBJECT_CUBEMAP:
				case GL_OBJECT_TEXTURE:
					GL_CHECK(glDeleteTextures, OBJECT_COUNT, &handle);
					break;
				case GL_OBJECT_PROGRAM:
					GL_CHECK(glDeleteProgram, handle);
					break;
				case GL_OBJECT_SHADER:
					GL_CHECK(glDeleteShader, handle);
					break;
				case GL_OBJECT_VAO:
					GL_CHECK(glDeleteVertexArrays, OBJECT_COUNT, &handle);
					break;
				case GL_OBJECT_VBO:
					GL_CHECK(glDeleteBuffers, OBJECT_COUNT, &handle);
					break;
				default:
					THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID,
						"%x", TUPLE_ENTRY(entry->second, GL_TUPLE_TYPE));
			}
		}

		std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator 
		_manager::find(
			__in gaea::uid_t &id,
			__in gaea::gl_t type
			)
		{
			std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_UNINITIALIZED);
			}

			if(type >= m_entry.size()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID,
					"%x", type);
			}

			result = m_entry.at(type).find(id);
			if(result == m_entry.at(type).end()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_NOT_FOUND,
					"%x", id);
			}

			return result;
		}

		GLuint 
		_manager::generate(
			__in gaea::uid_t &id,
			__in gaea::gl_t type,
			__in GLenum target
			)
		{
			GLuint result = GL_HANDLE_INVALID;

			if(!m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_UNINITIALIZED);
			}

			if(type >= m_entry.size()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID,
					"%x", type);
			}

			if(m_entry.at(type).find(id) != m_entry.at(type).end()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_DUPLICATE,
					"%x", id);
			}

			result = create(type, target);
			m_entry.at(type).insert(std::pair<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>(
				id, std::make_tuple(type, target, result, REFERENCE_INIT)));

			return result;
		}

		size_t 
		_manager::increment_reference(
			__in gaea::uid_t &id,
			__in gaea::gl_t type
			)
		{
			return ++TUPLE_ENTRY(find(id, type)->second, GL_TUPLE_REFERENCE);
		}

		void 
		_manager::initialize(void)
		{

			if(m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_INITIALIZED);
			}

			m_entry.resize(GL_OBJECT_MAX + 1, 
				std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>());
			m_initialized = true;
		}

		bool 
		_manager::is_allocated(void)
		{
			return (gaea::graphics::manager::m_instance != nullptr);
		}

		bool 
		_manager::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_manager::reference_count(
			__in gaea::uid_t &id,
			__in gaea::gl_t type
			)
		{
			return TUPLE_ENTRY(find(id, type)->second, GL_TUPLE_REFERENCE);
		}

		size_t 
		_manager::size(void)
		{
			size_t result = 0;
			std::vector<std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>>::iterator iter;

			if(!m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_entry.begin(); iter != m_entry.end(); ++iter) {
				result += iter->size();
			}

			return result;
		}

		size_t 
		_manager::size(
			__in gaea::gl_t type
			)
		{

			if(!m_initialized) {
				THROW_GAEA_GL_EXCEPTION(GAEA_GL_EXCEPTION_UNINITIALIZED);
			}

			if(type >= m_entry.size()) {
				THROW_GAEA_GL_EXCEPTION_FORMAT(GAEA_GL_EXCEPTION_INVALID,
					"%x", type);
			}

			return m_entry.at(type).size();
		}

		std::string 
		_manager::to_string(
			__in_opt bool verbose
			)
		{
			size_t count;
			std::stringstream result;
			std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>::iterator entry_iter;
			std::vector<std::map<gaea::uid_t, std::tuple<gaea::gl_t, GLenum, GLuint, size_t>>>::iterator type_iter;

			result << GAEA_GL_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT") << ")";

			if(m_initialized) {
				result << " INST=" << SCALAR_AS_HEX(uintptr_t, this)
					<< ", ENTRIES=" << size();

				if(verbose) {

					for(type_iter = m_entry.begin(); type_iter != m_entry.end(); ++type_iter) {

						for(count = 0, entry_iter = type_iter->begin(); entry_iter != type_iter->end(); 
								++count, ++entry_iter) {
							result << std::endl << "[" << count << "]" 
								<< GL_OBJECT_STRING(TUPLE_ENTRY(entry_iter->second, GL_TUPLE_TYPE))
								<< ", TRG=" << SCALAR_AS_HEX(GLenum, TUPLE_ENTRY(entry_iter->second, 
									GL_TUPLE_TARGET))
								<< ", HDL=" << SCALAR_AS_HEX(GLuint, TUPLE_ENTRY(entry_iter->second, 
									GL_TUPLE_HANDLE))
								<< ", REF=" << TUPLE_ENTRY(entry_iter->second, GL_TUPLE_REFERENCE);
						}
					}
				}
			}

			return result.str();
		}

		void 
		_manager::uninitialize(void)
		{

			if(m_initialized) {
				m_initialized = false;
				clear();
			}
		}
	}
}
