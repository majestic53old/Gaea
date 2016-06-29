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

#ifndef GAEA_OBSERVER_H_
#define GAEA_OBSERVER_H_

namespace gaea {

	#define BUTTON_MAX BUTTON_X2
	#define INPUT_SENSITIVITY_INIT 15.f
	#define INPUT_SPEED_INIT 10.f
	#define KEY_MAX KEY_RIGHT

	typedef enum {
		BUTTON_LEFT = 0,
		BUTTON_MIDDLE,
		BUTTON_RIGHT,
		BUTTON_X1,
		BUTTON_X2,
	} button_t;

	typedef enum {
		KEY_BACKWARD = 0,
		KEY_FORWARD,
		KEY_LEFT,
		KEY_RIGHT,
	} key_t;

	static const std::map<gaea::key_t, SDL_Scancode> INPUT_KEY_MAP_INIT = {
		{KEY_BACKWARD, SDL_SCANCODE_S},
		{KEY_FORWARD, SDL_SCANCODE_W},
		{KEY_LEFT, SDL_SCANCODE_A},
		{KEY_RIGHT, SDL_SCANCODE_D},
		};

	namespace engine {

		namespace observer {

			typedef class _base {

				public:

					_base(void);

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

					bool is_handler_registered(
						__in gaea::event_t type
						);

					void register_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

					size_t size(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					void unregister_all_handlers(void);

					void unregister_handler(
						__in gaea::event_t type
						);

				protected:

					std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator find(
						__in gaea::event_t type
						);

					std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>> m_handler;

			} base;

			typedef class _base_input :
					public gaea::engine::observer::base {

				public:

					_base_input(
						
						__in glm::vec3 &position_reference,
						__in glm::vec3 &rotation_reference,
						__in glm::vec3 &up_reference,
						__in_opt const std::map<gaea::key_t, SDL_Scancode> &key_map = INPUT_KEY_MAP_INIT,
						__in_opt GLfloat speed = INPUT_SPEED_INIT,
						__in_opt GLfloat sensitivity = INPUT_SENSITIVITY_INIT
						);

					_base_input(
						__in const _base_input &other
						);

					virtual ~_base_input(void);

					_base_input &operator=(
						__in const _base_input &other
						);

					static std::string as_string(
						__in const _base_input &object,
						__in_opt bool verbose = false
						);

					void clear(void);

					bool is_button_pressed(
						__in gaea::button_t type,
						__out glm::ivec2 &coordinate,
						__out uint8_t &click
						);

					bool is_key_pressed(
						__in gaea::key_t type
						);

					void setup(
						__in const std::map<gaea::key_t, SDL_Scancode> &key_map,
						__in GLfloat speed,
						__in GLfloat sensitivity
						);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					virtual void update(
						__in GLfloat delta
						);

				protected:

					static void event_handler(
						__in gaea::engine::event::base &event,
						__in void *context
						);

					void on_button(
						__in const SDL_MouseButtonEvent &event
						);

					void on_key(
						__in const SDL_KeyboardEvent &event
						);

					void on_motion(
						__in const SDL_MouseMotionEvent &event
						);

					void on_wheel(
						__in const SDL_MouseWheelEvent &event
						);

					void setup(void);

					std::map<gaea::button_t, std::tuple<uint8_t, glm::ivec2, bool>> m_button;

					std::map<SDL_Scancode, bool> m_key;

					std::map<gaea::key_t, SDL_Scancode> m_key_map;

					glm::vec3 m_position_delta;

					glm::vec3 &m_position_reference;

					glm::vec3 m_rotation_current;

					glm::vec3 m_rotation_delta;

					glm::vec3 &m_rotation_reference;

					GLfloat m_sensitivity;

					GLfloat m_speed;

					glm::vec3 m_up_delta;

					glm::vec3 &m_up_reference;

					GLfloat m_wheel_delta;

			} base_input;
		}
	}
}

#endif // GAEA_OBSERVER_H_
