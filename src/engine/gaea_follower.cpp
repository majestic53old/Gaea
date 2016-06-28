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
#include "gaea_follower_type.h"

namespace gaea {

	namespace engine {

		namespace follower {

			#define FOLLOWER_STRING(_TYPE_) \
				((_TYPE_) > FOLLOWER_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(FOLLOWER_STR[_TYPE_]))

			static const std::string FOLLOWER_STR[] = {
				"FIRST", "THIRD",
				};

			_base::_base(
				__in gaea::follower_t type,
				__in glm::vec3 &follow_position,
				__in glm::vec3 &follow_rotation,
				__in glm::vec3 &follow_up,
				__in_opt GLfloat speed,
				__in_opt GLfloat sensitivity
				) :
					m_follow_position(follow_position),
					m_follow_rotation(follow_rotation),
					m_follow_up(follow_up),
					m_sensitivity(sensitivity),
					m_speed(speed),
					m_type(type)
			{

				setup(follow_position, follow_rotation, follow_up, speed, sensitivity);
				setup();
			}

			_base::_base(
				__in const _base &other
				) :
					m_follow_position(other.m_follow_position),
					m_follow_rotation(other.m_follow_rotation),
					m_follow_up(other.m_follow_up),
					m_sensitivity(other.m_sensitivity),
					m_speed(other.m_speed),
					m_type(other.m_type)
			{
				setup();
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
					m_follow_position = other.m_follow_position;
					m_follow_rotation = other.m_follow_rotation;
					m_follow_up = other.m_follow_up;
					m_sensitivity = other.m_sensitivity;
					m_speed = other.m_speed;
					m_type = other.m_type;
					setup();
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

				result << gaea::engine::observer::base::as_string(object, verbose)
					<< ", MODE=" << FOLLOWER_STRING(object.m_type)
					<< ", SPD=" << object.m_speed
					<< ", SEN=" << object.m_sensitivity;

				return result.str();
			}

			void 
			_base::event_handler(
				__in gaea::engine::event::base &event,
				__in void *context
				)
			{
				uint32_t specifier;
				SDL_KeyboardEvent *key = nullptr;
				SDL_MouseWheelEvent *wheel = nullptr;
				SDL_MouseButtonEvent *button = nullptr;
				SDL_MouseMotionEvent *motion = nullptr;
				gaea::engine::follower::base *instance = nullptr;

				instance = (gaea::engine::follower::base *) context;
				if(!instance) {
					THROW_GAEA_FOLLOWER_EXCEPTION(GAEA_FOLLOWER_EXCEPTION_INVALID_CONTEXT);
				}

				if(event.type() == EVENT_INPUT) {

					specifier = event.specifier();
					switch(specifier) {
						case EVENT_INPUT_BUTTON:

							button = (SDL_MouseButtonEvent *) event.context();
							if(!button || (event.length() != sizeof(SDL_MouseButtonEvent))) {
								THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							handle_button(*button, *instance);
							break;
						case EVENT_INPUT_KEY:

							key = (SDL_KeyboardEvent *) event.context();
							if(!key || (event.length() != sizeof(SDL_KeyboardEvent))) {
								THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							if(!key->repeat) {
								handle_key(*key, *instance);
							}
							break;
						case EVENT_INPUT_MOTION:

							motion = (SDL_MouseMotionEvent *) event.context();
							if(!motion || (event.length() != sizeof(SDL_MouseMotionEvent))) {
								THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							handle_motion(*motion, *instance);
							break;
						case EVENT_INPUT_WHEEL:

							wheel = (SDL_MouseWheelEvent *) event.context();
							if(!wheel || (event.length() != sizeof(SDL_MouseWheelEvent))) {
								THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							handle_wheel(*wheel, *instance);
							break;
						default:
							break;
					}
				}
			}

			void 
			_base::handle_button(
				__in const SDL_MouseButtonEvent &event,
				__in _base &context
				)
			{
				// TODO
			}

			void 
			_base::handle_key(
				__in const SDL_KeyboardEvent &event,
				__in _base &context
				)
			{
				// TODO
			}

			void 
			_base::handle_motion(
				__in const SDL_MouseMotionEvent &event,
				__in _base &context
				)
			{
				// TODO
			}

			void 
			_base::handle_wheel(
				__in const SDL_MouseWheelEvent &event,
				__in _base &context
				)
			{
				// TODO
			}

			GLfloat 
			_base::sensitivity(void)
			{
				return m_sensitivity;
			}

			void 
			_base::setup(void)
			{
				gaea::engine::observer::base::register_handler(&base::event_handler, EVENT_CAMERA, this);
			}

			void 
			_base::setup(
				__in glm::vec3 &follow_position,
				__in glm::vec3 &follow_rotation,
				__in glm::vec3 &follow_up,
				__in_opt GLfloat speed,
				__in_opt GLfloat sensitivity
				)
			{
				if(m_type > FOLLOWER_MAX) {
					THROW_GAEA_FOLLOWER_EXCEPTION_FORMAT(GAEA_FOLLOWER_EXCEPTION_INVALID_TYPE,
						"%x", m_type);
				}

				if(m_sensitivity < FOLLOWER_SENSITIVITY_MIN) {
					m_sensitivity = FOLLOWER_SENSITIVITY_MIN;
				}

				if(m_speed < FOLLOWER_SPEED_MIN) {
					m_speed = FOLLOWER_SPEED_MIN;
				}

				m_follow_position = follow_position;
				m_follow_rotation = follow_rotation;
				m_follow_up = follow_up;
			}

			GLfloat 
			_base::speed(void)
			{
				return m_speed;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::follower::base::as_string(*this, verbose);
			}

			gaea::follower_t 
			_base::type(void)
			{
				return m_type;
			}
		}
	}
}
