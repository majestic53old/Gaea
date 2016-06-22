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

#include "../include/gaea.h"
#include "gaea_type.h"

namespace gaea {

	#define DELTA_DIV 1000
	#define DELTA_MIN 1

	#define GL_ATTRIB_ACCELERATED 1
	#define GL_ATTRIB_COLOR 8
	#define GL_ATTRIB_DEPTH 24
	#define GL_ATTRIB_DOUBLE_BUFFER 1
	#define GL_ATTRIB_MAJOR 3
	#define GL_ATTRIB_MINOR 3

	#define GL_CLEAR_FLAGS (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

	#define GL_CHAN_ALPHA GL_CHAN_MAX
	#define GL_CHAN_BLUE (164 / (GLfloat) GL_CHAN_MAX)
	#define GL_CHAN_GREEN (101 / (GLfloat) GL_CHAN_MAX)
	#define GL_CHAN_MAX 255
	#define GL_CHAN_RED (52 / (GLfloat) GL_CHAN_MAX)

	#define SDL_ATTRIB_VSYNC 1
	#define SDL_INIT_FLAGS (SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO)
	#define SDL_WINDOW_FLAGS (SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_OPENGL)

	std::string 
	version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		if(verbose) {
			result << VERSION_NAME << " ";
		}

		result << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_WEEK
			<< "." << VERSION_REVISION;

		if(verbose) {
			result << "-" << VERSION_MILESTONE;
		}

		return result.str();
	}

	_manager *_manager::m_instance = nullptr;

	_manager::_manager(void) :
		m_event_manager(gaea::engine::event::manager::acquire()),
		m_gfx_manager(gaea::graphics::manager::acquire()),
		m_initialized(false),
		m_started(false),
		m_tick(TICK_INVALID),
		m_uid_manager(gaea::engine::uid::manager::acquire()),
		m_window(nullptr),
		m_window_context(nullptr)
	{
		std::atexit(gaea::manager::_delete);
	}

	_manager::~_manager(void)
	{
		uninitialize();
	}

	void 
	_manager::_delete(void)
	{

		if(gaea::manager::m_instance) {
			delete gaea::manager::m_instance;
			gaea::manager::m_instance = nullptr;
		}
	}

	_manager &
	_manager::acquire(void)
	{

		if(!gaea::manager::m_instance) {

			gaea::manager::m_instance = new gaea::manager;
			if(!gaea::manager::m_instance) {
				THROW_GAEA_EXCEPTION(GAEA_EXCEPTION_ALLOCATED);
			}
		}

		return *gaea::manager::m_instance;
	}

	void 
	_manager::initialize(void)
	{

		if(m_initialized) {
			THROW_GAEA_EXCEPTION(GAEA_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
	}

	bool 
	_manager::is_allocated(void)
	{
		return (gaea::manager::m_instance != nullptr);
	}

	bool 
	_manager::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	_manager::is_started(void)
	{
		return m_started;
	}

	void 
	_manager::render(void)
	{
		GL_CHECK(glClearColor, GL_CHAN_RED, GL_CHAN_GREEN, GL_CHAN_BLUE, GL_CHAN_ALPHA);
		GL_CHECK(glClear, GL_CLEAR_FLAGS);

		// TODO: render

		SDL_GL_SwapWindow(m_window);
	}

	void 
	_manager::setup(
		__in const std::string &title,
		__in const glm::uvec2 &dimensions,
		__in_opt bool fullscreen,
		__in_opt gaea::tick_t tick
		)
	{
		GLenum result;
		uint32_t flags = SDL_WINDOW_FLAGS;

		m_tick = tick;

		if(SDL_Init(SDL_INIT_FLAGS)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_Init failed: %s",
				SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, GL_ATTRIB_COLOR)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_RED_SIZE) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, GL_ATTRIB_COLOR)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, GL_ATTRIB_COLOR)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, GL_ATTRIB_DEPTH)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_ATTRIB_DOUBLE_BUFFER)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, GL_ATTRIB_ACCELERATED)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_ATTRIB_MAJOR)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION) failed: %s", SDL_GetError());
		}

		if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_ATTRIB_MINOR)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, 
				"SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION) failed: %s", SDL_GetError());
		}

		if(fullscreen) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		m_window = SDL_CreateWindow(STRING_CHECK(title), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				dimensions.x, dimensions.y, flags);

		if(!m_window) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_CreateWindow failed: %s", 
				SDL_GetError());
		}

		m_window_context = SDL_GL_CreateContext(m_window);
		if(!m_window_context) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_GL_CreateContext failed: %s", 
				SDL_GetError());
		}

		if(SDL_GL_MakeCurrent(m_window, m_window_context)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_GL_MakeCurrent failed: %s", 
				SDL_GetError());
		}

		if(SDL_SetRelativeMouseMode(SDL_TRUE)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_SetRelativeMouseMode(SDL_TRUE) failed: %s", 
				SDL_GetError());
		}

		glewExperimental = GL_TRUE;

		result = glewInit();
		if(result != GLEW_OK) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "glewInit failed: %s",
				GL_ERROR_STRING(result));
		}

		GL_ERROR_CLEAR;

		if(SDL_GL_SetSwapInterval(SDL_ATTRIB_VSYNC)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_GL_SetSwapInterval failed: %s", 
				SDL_GetError());
		}

		m_uid_manager.initialize();
		m_event_manager.initialize();
		m_gfx_manager.initialize();

		// TODO: initialize singletons
	}

	void 
	_manager::start(
		__in const std::string &title,
		__in const glm::uvec2 &dimensions,
		__in_opt bool fullscreen,
		__in_opt gaea::tick_t tick
		)
	{
		SDL_Event event;
		gaea::tick_t begin = 0, delta = 0, end = 0;

		if(!m_initialized) {
			THROW_GAEA_EXCEPTION(GAEA_EXCEPTION_UNINITALIZED);
		}

		if(m_started) {
			THROW_GAEA_EXCEPTION(GAEA_EXCEPTION_STARTED);
		}

		setup(title, dimensions, fullscreen, tick);

		m_started = true;
		while(m_started) {

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						// TODO
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						// TODO
						break;
					case SDL_MOUSEMOTION:
						// TODO
						break;
					case SDL_MOUSEWHEEL:
						// TODO
						break;
					case SDL_QUIT:
						stop();
						break;
					default:
						break;
				}
			}

			if(delta < DELTA_MIN) {
				begin = SDL_GetTicks();
				SDL_Delay(DELTA_MIN);
				end = SDL_GetTicks();
				delta = (end - begin);
			}

			begin = SDL_GetTicks();
			update(delta / (GLfloat) DELTA_DIV);
			render();
			end = SDL_GetTicks();
			delta = (end - begin);
			++m_tick;
		}

		teardown();
	}

	void 
	_manager::stop(void)
	{

		if(m_initialized && m_started) {
			m_started = false;
		}
	}

	void 
	_manager::teardown(void)
	{
		// TODO: uninitialize singletons

		m_gfx_manager.uninitialize();
		m_event_manager.uninitialize();
		m_uid_manager.uninitialize();

		if(SDL_SetRelativeMouseMode(SDL_FALSE)) {
			THROW_GAEA_EXCEPTION_FORMAT(GAEA_EXCEPTION_EXTERNAL, "SDL_SetRelativeMouseMode(SDL_FALSE) failed: %s", 
				SDL_GetError());
		}

		if(m_window_context) {
			SDL_GL_DeleteContext(m_window_context);
			m_window_context = nullptr;
		}

		if(m_window) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}

		SDL_Quit();
		m_tick = TICK_INVALID;
	}

	gaea::tick_t 
	_manager::tick(void)
	{
		return m_tick;
	}

	std::string 
	_manager::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << GAEA_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT")
			<< ", " << (m_started ? "START" : "STOP") << ")";

		if(m_initialized) {
			result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

			if(m_started) {
				result << ", WIN=" << SCALAR_AS_HEX(uintptr_t, m_window) 
					<< ", CONT=" << SCALAR_AS_HEX(uintptr_t, m_window_context)
					<< ", TICK=" << m_tick;
			}
		}

		return result.str();
	}

	void 
	_manager::uninitialize(void)
	{

		if(m_initialized) {

			if(m_started) {
				stop();
			}

			m_initialized = false;
		}
	}

	void 
	_manager::update(
		__in GLfloat delta
		)
	{
		// TODO: update
	}
}
