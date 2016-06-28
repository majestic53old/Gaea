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

#ifndef GAEA_FOLLOWER_H_
#define GAEA_FOLLOWER_H_

namespace gaea {

	#define FOLLOWER_MAX FOLLOWER_THIRD_PERSON
	#define FOLLOWER_SENSITIVITY_INIT 1.f
	#define FOLLOWER_SENSITIVITY_MIN 0.1f
	#define FOLLOWER_SPEED_INIT 5.f
	#define FOLLOWER_SPEED_MIN 1.f

	typedef enum {
		FOLLOWER_FIRST_PERSON = 0,
		FOLLOWER_THIRD_PERSON,
	} follower_t;

	namespace engine {

		namespace follower {

			typedef class _base :
					public gaea::engine::observer::base {

				public:

					_base(
						__in gaea::follower_t type,
						__in glm::vec3 &follow_position,
						__in glm::vec3 &follow_rotation,
						__in glm::vec3 &follow_up,
						__in_opt GLfloat speed = FOLLOWER_SPEED_INIT,
						__in_opt GLfloat sensitivity = FOLLOWER_SENSITIVITY_INIT
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

					GLfloat sensitivity(void);

					void setup(
						__in glm::vec3 &follow_position,
						__in glm::vec3 &follow_rotation,
						__in glm::vec3 &follow_up,
						__in_opt GLfloat speed = FOLLOWER_SPEED_INIT,
						__in_opt GLfloat sensitivity = FOLLOWER_SENSITIVITY_INIT
						);

					GLfloat speed(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					gaea::follower_t type(void);

				protected:

					static void event_handler(
						__in gaea::engine::event::base &event,
						__in void *context
						);

					static void handle_button(
						__in const SDL_MouseButtonEvent &event,
						__in _base &context
						);

					static void handle_key(
						__in const SDL_KeyboardEvent &event,
						__in _base &context
						);

					static void handle_motion(
						__in const SDL_MouseMotionEvent &event,
						__in _base &context
						);

					static void handle_wheel(
						__in const SDL_MouseWheelEvent &event,
						__in _base &context
						);

					void setup(void);

					glm::vec3 &m_follow_position;

					glm::vec3 &m_follow_rotation;

					glm::vec3 &m_follow_up;

					GLfloat m_sensitivity;

					GLfloat m_speed;

					gaea::follower_t m_type;

			} base;
		}
	}
}

#endif // GAEA_FOLLOWER_H_
