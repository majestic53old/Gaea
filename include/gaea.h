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

#ifndef GAEA_H_
#define GAEA_H_

//#define NDEBUG

#include "gaea_define.h"
#include "gaea_exception.h"
#include "engine/gaea_uid.h"
#include "engine/gaea_object.h"
#include "engine/gaea_command.h"
#include "graphics/gl/gaea_gl.h"
#include "graphics/gl/gaea_vbo.h"
#include "graphics/gl/gaea_vao.h"
#include "graphics/gl/gaea_shader.h"
#include "graphics/gl/gaea_program.h"
#include "graphics/gl/gaea_texture.h"
#include "graphics/gl/gaea_cubemap.h"

namespace gaea {

	#define TICK_INIT 0
	#define TICK_INVALID SCALAR_INVALID(gaea::tick_t)

	typedef uint32_t tick_t;

	std::string version(
		__in_opt bool verbose = false
		);

	typedef class _manager {

		public:

			~_manager(void);

			static _manager &acquire(void);

			void initialize(void);

			static bool is_allocated(void);

			bool is_initialized(void);

			bool is_started(void);

			void start(
				__in const std::string &title,
				__in const glm::uvec2 &dimensions,
				__in_opt bool fullscreen = false,
				__in_opt gaea::tick_t tick = TICK_INIT
				);

			void stop(void);

			gaea::tick_t tick(void);

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

			void render(void);

			void setup(
				__in const std::string &title,
				__in const glm::uvec2 &dimensions,
				__in_opt bool fullscreen = false,
				__in_opt gaea::tick_t tick = TICK_INIT
				);

			void teardown(void);

			void update(
				__in GLfloat delta
				);

			gaea::engine::command::manager &m_command_manager;

			gaea::graphics::gl::manager &m_gl_manager;

			bool m_initialized;

			static _manager *m_instance;

			bool m_started;

			gaea::tick_t m_tick; 

			gaea::engine::uid::manager &m_uid_manager;

			SDL_Window *m_window;

			SDL_GLContext m_window_context;

	} manager;
}

#endif // GAEA_H_
