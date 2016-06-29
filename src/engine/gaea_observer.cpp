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
#include "gaea_observer_type.h"

namespace gaea {

	namespace engine {

		namespace observer {

			#define INPUT_PITCH_MAX 89.f
			#define INPUT_SENSITIVITY_MIN 0.1f
			#define INPUT_SPEED_MIN 1.f
			#define INPUT_STRAFE_SCALE 1.5f
			#define INPUT_YAW_MAX 360.f
			#define INPUT_YAW_MIN 0.f

			#define BUTTON_STRING(_TYPE_) \
				((_TYPE_) > BUTTON_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(BUTTON_STR[_TYPE_]))

			#define KEY_STRING(_TYPE_) \
				((_TYPE_) > KEY_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(KEY_STR[_TYPE_]))

			enum {
				INPUT_BUTTON_CLICK = 0,
				INPUT_BUTTON_COORDINATE,
				INPUT_BUTTON_STATE,
			};

			static const std::string BUTTON_STR[] = {
				"LEFT", "MIDDLE", "RIGHT", "X1", "X2",
				};

			static const std::string KEY_STR[] = {
				"BACKWARD", "FORWARD", "LEFT", "RIGHT",
				};

			_base::_base(void)
			{
				return;
			}

			_base::_base(
				__in const _base &other
				)
			{
				return;
			}

			_base::~_base(void)
			{
				unregister_all_handlers();
			}

			_base &
			_base::operator=(
				__in const _base &other
				)
			{

				if(this != &other) {
					unregister_all_handlers();
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
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::const_iterator iter;

				result << "OBS[" << object.m_handler.size() << "]";

				if(!object.m_handler.empty()) {
					result << "={";

					for(iter = object.m_handler.begin(); iter != object.m_handler.end(); ++iter) {

						if(iter != object.m_handler.begin()) {
							result << "; ";
						}

						result << EVENT_STRING(iter->first)
							<< " (" << SCALAR_AS_HEX(uintptr_t, iter->second.first)
							<< ", CONT=" << SCALAR_AS_HEX(uintptr_t, iter->second.second) << ")";
					}

					result << "}";
				}

				return result.str();
			}

			std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator 
			_base::find(
				__in gaea::event_t type
				)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator result;

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				result = m_handler.find(type);
				if(result == m_handler.end()) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_HANDLER_NOT_FOUND,
						"%x", type);
				}

				return result;
			}

			bool 
			_base::is_handler_registered(
				__in gaea::event_t type
				)
			{

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				return (m_handler.find(type) != m_handler.end());
			}

			void 
			_base::register_handler(
				__in gaea::engine::event::handler_cb handler,
				__in gaea::event_t type,
				__in_opt void *context
				)
			{

				if(type > EVENT_MAX) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_TYPE,
						"%x", type);	
				}

				if(!handler) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_HANDLER,
						"%p", handler);
				}

				if(m_handler.find(type) != m_handler.end()) {
					THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_REGISTERED,
						"[%s (%x)] %p(%p)", EVENT_STRING(type), type, handler, context);
				}

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& !instance.contains_handler(handler, type, context)) {
						instance.register_handler(handler, type, context);
					}
				}

				m_handler.insert(std::pair<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>(
					type, std::pair<gaea::engine::event::handler_cb, void *>(handler, context)));
			}

			size_t 
			_base::size(void)
			{
				return m_handler.size();
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::observer::base::as_string(*this, verbose);
			}

			void 
			_base::unregister_all_handlers(void)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized()) {

						for(iter = m_handler.begin(); iter != m_handler.end(); ++iter) {

							if(!instance.contains_handler(iter->second.first, iter->first, iter->second.second)) {
								continue;
							}

							instance.unregister_handler(iter->second.first, iter->first, iter->second.second);
						}
					}
				}

				m_handler.clear();
			}

			void 
			_base::unregister_handler(
				__in gaea::event_t type
				)
			{
				std::map<gaea::event_t, std::pair<gaea::engine::event::handler_cb, void *>>::iterator iter;

				iter = find(type);

				if(gaea::engine::event::manager::is_allocated()) {

					gaea::engine::event::manager &instance = gaea::engine::event::manager::acquire();
					if(instance.is_initialized() 
							&& instance.contains_handler(iter->second.first, type, iter->second.second)) {
						instance.unregister_handler(iter->second.first, type, iter->second.second);
					}
				}

				m_handler.erase(iter);
			}

			_base_input::_base_input(
				__in glm::vec3 &position_reference,
				__in glm::vec3 &rotation_reference,
				__in glm::vec3 &up_reference,
				__in_opt const std::map<gaea::key_t, SDL_Scancode> &key_map,
				__in_opt GLfloat speed,
				__in_opt GLfloat sensitivity
				) :
					m_position_delta(glm::vec3()),
					m_position_reference(position_reference),
					m_rotation_current(glm::vec3()),
					m_rotation_delta(glm::vec3()),
					m_rotation_reference(rotation_reference),
					m_sensitivity(INPUT_SENSITIVITY_MIN),
					m_speed(INPUT_SPEED_MIN),
					m_up_delta(glm::vec3()),
					m_up_reference(up_reference),
					m_wheel_delta(0)
			{
				setup(key_map, speed, sensitivity);
				setup();
			}

			_base_input::_base_input(
				__in const _base_input &other
				) :
					gaea::engine::observer::base(other),
					m_button(other.m_button),
					m_key(other.m_key),
					m_key_map(other.m_key_map),
					m_position_delta(other.m_position_delta),
					m_position_reference(other.m_position_reference),
					m_rotation_current(other.m_rotation_current),
					m_rotation_delta(other.m_rotation_delta),
					m_rotation_reference(other.m_rotation_reference),
					m_sensitivity(other.m_sensitivity),
					m_speed(other.m_speed),
					m_up_delta(other.m_up_delta),
					m_up_reference(other.m_up_reference),
					m_wheel_delta(other.m_wheel_delta)
			{
				clear();
				setup();
			}

			_base_input::~_base_input(void)
			{
				return;
			}

			_base_input &
			_base_input::operator=(
				__in const _base_input &other
				)
			{

				if(this != &other) {
					gaea::engine::observer::base::operator=(other);
					m_button = other.m_button;
					m_key = other.m_key;
					m_key_map = other.m_key_map;
					m_position_delta = other.m_position_delta;
					m_position_reference = other.m_position_reference;
					m_rotation_current = other.m_rotation_current;
					m_rotation_delta = other.m_rotation_delta;
					m_rotation_reference = other.m_rotation_reference;
					m_sensitivity = other.m_sensitivity;
					m_speed = other.m_speed;
					m_up_delta = other.m_up_delta;
					m_up_reference = other.m_up_reference;
					m_wheel_delta = other.m_wheel_delta;
					clear();
					setup();
				}

				return *this;
			}

			std::string 
			_base_input::as_string(
				__in const _base_input &object,
				__in_opt bool verbose
				)
			{
				std::stringstream result;				
				std::map<SDL_Scancode, bool>::const_iterator key_iter;				
				std::map<gaea::key_t, SDL_Scancode>::const_iterator key_map_iter;
				std::map<gaea::button_t, std::tuple<uint8_t, glm::ivec2, bool>>::const_iterator button_iter;

				result << gaea::engine::observer::base::as_string(object, verbose)
					<< ", BUT[" << object.m_button.size() << "]";

				if(!object.m_button.empty()) {
					result << "={";

					for(button_iter = object.m_button.begin(); button_iter != object.m_button.end();
							++button_iter) {

						if(button_iter != object.m_button.begin()) {
							result << "; ";
						}

						result << BUTTON_STRING(button_iter->first) 
							<< "=(" << TUPLE_ENTRY(button_iter->second, INPUT_BUTTON_CLICK) 
							<< ", {" << TUPLE_ENTRY(button_iter->second, INPUT_BUTTON_COORDINATE).x
								<< ", " << TUPLE_ENTRY(button_iter->second, INPUT_BUTTON_COORDINATE).y << "}"
							<< ", " << TUPLE_ENTRY(button_iter->second, INPUT_BUTTON_STATE) << ")";
					}

					result << "}";
				}

				result << ", KEY[" << object.m_key_map.size() << "]";

				if(!object.m_key_map.empty()) {
					result << "={";

					for(key_map_iter = object.m_key_map.begin(); key_map_iter != object.m_key_map.end();
							++key_map_iter) {

						if(key_map_iter != object.m_key_map.begin()) {
							result << ", ";
						}

						result << KEY_STRING(key_map_iter->first) 
							<< "(" << SCALAR_AS_HEX(SDL_Scancode, key_map_iter->second) 
							<< ")=";

						key_iter = object.m_key.find(key_map_iter->second);
						if(key_iter != object.m_key.end()) {
							result << key_iter->second;
						}
					}

					result << "}";
				}

				result << ", WHL=" << object.m_wheel_delta;

				return result.str();
			}

			void 
			_base_input::clear(void)
			{
				m_button.clear();
				m_key.clear();
				m_position_delta = glm::vec3();
				m_rotation_current = glm::vec3();
				m_rotation_delta = glm::vec3();
				m_up_delta = glm::vec3();
				m_wheel_delta = 0;
			}

			void 
			_base_input::event_handler(
				__in gaea::engine::event::base &event,
				__in void *context
				)
			{
				SDL_KeyboardEvent *key = nullptr;
				SDL_MouseWheelEvent *wheel = nullptr;
				SDL_MouseButtonEvent *button = nullptr;
				SDL_MouseMotionEvent *motion = nullptr;
				gaea::engine::observer::base_input *instance = nullptr;

				instance = (gaea::engine::observer::base_input *) context;
				if(!instance) {
					THROW_GAEA_OBSERVER_EXCEPTION(GAEA_OBSERVER_EXCEPTION_INVALID_CONTEXT);
				}

				if(event.type() == EVENT_INPUT) {

					switch(event.specifier()) {
						case EVENT_INPUT_BUTTON:

							button = (SDL_MouseButtonEvent *) event.context();
							if(!button || (event.length() != sizeof(SDL_MouseButtonEvent))) {
								THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							instance->on_button(*button);
							break;
						case EVENT_INPUT_KEY:

							key = (SDL_KeyboardEvent *) event.context();
							if(!key || (event.length() != sizeof(SDL_KeyboardEvent))) {
								THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							instance->on_key(*key);
							break;
						case EVENT_INPUT_MOTION:

							motion = (SDL_MouseMotionEvent *) event.context();
							if(!motion || (event.length() != sizeof(SDL_MouseMotionEvent))) {
								THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							instance->on_motion(*motion);
							break;
						case EVENT_INPUT_WHEEL:

							wheel = (SDL_MouseWheelEvent *) event.context();
							if(!wheel || (event.length() != sizeof(SDL_MouseWheelEvent))) {
								THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_INVALID_EVENT,
									"%s", STRING_CHECK(event.to_string(true)));
							}

							instance->on_wheel(*wheel);
							break;
						default:
							break;
					}
				}
			}

			bool 
			_base_input::is_button_pressed(
				__in gaea::button_t type,
				__out glm::ivec2 &coordinate,
				__out uint8_t &click
				)
			{
				bool result = false;
				std::map<gaea::button_t, std::tuple<uint8_t, glm::ivec2, bool>>::iterator iter;

				coordinate = glm::ivec2();
				click = 0;

				iter = m_button.find(type);
				if(iter != m_button.end()) {
					result = TUPLE_ENTRY(iter->second, INPUT_BUTTON_STATE);
					coordinate = TUPLE_ENTRY(iter->second, INPUT_BUTTON_COORDINATE);
					click = TUPLE_ENTRY(iter->second, INPUT_BUTTON_CLICK);
				}

				return result;
			}

			bool 
			_base_input::is_key_pressed(
				__in gaea::key_t type
				)
			{
				bool result = false;
				std::map<SDL_Scancode, bool>::iterator key_iter;
				std::map<gaea::key_t, SDL_Scancode>::iterator key_map_iter;

				key_map_iter = m_key_map.find(type);
				if(key_map_iter != m_key_map.end()) {

					key_iter = m_key.find(key_map_iter->second);
					if(key_iter != m_key.end()) {
						result = key_iter->second;
					}
				}

				return result;
			}

			void 
			_base_input::on_button(
				__in const SDL_MouseButtonEvent &event
				)
			{
				std::map<gaea::button_t, std::tuple<uint8_t, glm::ivec2, bool>>::iterator iter;

				iter = m_button.find((gaea::button_t) event.button);
				if(iter == m_button.end()) {
					m_button.insert(std::pair<gaea::button_t, std::tuple<uint8_t, glm::ivec2, bool>>(
						(gaea::button_t) event.button, std::make_tuple(event.clicks, glm::ivec2(event.x, event.y),
						event.state == SDL_PRESSED)));
				} else {
					TUPLE_ENTRY(iter->second, INPUT_BUTTON_STATE) = (event.state == SDL_PRESSED);
				}
			}

			void 
			_base_input::on_key(
				__in const SDL_KeyboardEvent &event
				)
			{
				std::map<SDL_Scancode, bool>::iterator iter;

				if(!event.repeat) {

					iter = m_key.find(event.keysym.scancode);
					if(iter == m_key.end()) {
						m_key.insert(std::pair<SDL_Scancode, bool>(event.keysym.scancode, 
							event.state == SDL_PRESSED));
					} else {
						iter->second = (event.state == SDL_PRESSED);
					}
				}
			}

			void 
			_base_input::on_motion(
				__in const SDL_MouseMotionEvent &event
				)
			{
				m_rotation_delta += glm::vec3(event.xrel, event.yrel, 0.f);
			}

			void 
			_base_input::on_wheel(
				__in const SDL_MouseWheelEvent &event
				)
			{
				m_wheel_delta += event.y;
			}

			void 
			_base_input::setup(void)
			{
				gaea::engine::observer::base::register_handler(&gaea::engine::observer::base_input::event_handler,
					EVENT_INPUT, this);
			}

			void 
			_base_input::setup(
				__in const std::map<gaea::key_t, SDL_Scancode> &key_map,
				__in GLfloat speed,
				__in GLfloat sensitivity
				)
			{
				size_t iter = 0;
				std::map<gaea::key_t, SDL_Scancode>::const_iterator key_iter;

				m_key.clear();

				for(; iter <= KEY_MAX; ++iter) {

					key_iter = key_map.find((gaea::key_t) iter);
					if(key_iter == key_map.end()) {
						THROW_GAEA_OBSERVER_EXCEPTION_FORMAT(GAEA_OBSERVER_EXCEPTION_MISSING_KEY,
							"%s(%x)", KEY_STRING(iter), iter);
					}
				}

				m_key_map = key_map;

				m_sensitivity = sensitivity;
				if(m_sensitivity < INPUT_SENSITIVITY_MIN) {
					m_sensitivity = INPUT_SENSITIVITY_MIN;
				}

				m_speed = speed;
				if(m_speed < INPUT_SPEED_MIN) {
					m_speed = INPUT_SPEED_MIN;
				}
			}

			std::string 
			_base_input::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::observer::base_input::as_string(*this, verbose);
			}

			void 
			_base_input::update(
				__in GLfloat delta
				)
			{
				std::map<SDL_Scancode, bool>::iterator key_iter;
				std::map<gaea::key_t, SDL_Scancode>::iterator key_map_iter;
				GLfloat forward_speed = (m_speed * delta), rotational_speed, 
						strafe_speed = (forward_speed * INPUT_STRAFE_SCALE);

				for(key_map_iter = m_key_map.begin(); key_map_iter != m_key_map.end(); ++key_map_iter) {

					key_iter = m_key.find(key_map_iter->second);
					if((key_iter != m_key.end()) && key_iter->second) {

						switch(key_map_iter->first) {
							case KEY_BACKWARD:
								m_position_delta -= (m_rotation_reference * forward_speed);
								break;
							case KEY_FORWARD:
								m_position_delta += (m_rotation_reference * forward_speed);
								break;
							case KEY_LEFT:
								m_position_delta -= (glm::normalize(
									glm::cross(m_rotation_reference, m_up_reference)) 
									* strafe_speed);
								break;
							case KEY_RIGHT:
								m_position_delta += (glm::normalize(
									glm::cross(m_rotation_reference, m_up_reference)) 
									* strafe_speed);
								break;
							default:
								break;
						}
					}
				}

				if(m_position_delta != glm::vec3()) {
					m_position_reference += m_position_delta;
					m_position_delta = glm::vec3();
				}

				if(m_rotation_delta != glm::vec3()) {
					rotational_speed = (m_sensitivity * delta);

					m_rotation_current.x -= (m_rotation_delta.y * rotational_speed);
					if(m_rotation_current.x < -INPUT_PITCH_MAX) {
						m_rotation_current.x = -INPUT_PITCH_MAX;
					} else if(m_rotation_current.x > INPUT_PITCH_MAX) {
						m_rotation_current.x = INPUT_PITCH_MAX;
					}

					m_rotation_current.y -= (m_rotation_delta.x * rotational_speed);
					if(m_rotation_current.y < INPUT_YAW_MIN) {
						m_rotation_current.y = INPUT_YAW_MAX;
					} else if(m_rotation_current.y > INPUT_YAW_MAX) {
						m_rotation_current.y = INPUT_YAW_MIN;
					}

					m_rotation_reference = glm::normalize(glm::vec3(
						std::cos(glm::radians(m_rotation_current.x)) * std::sin(glm::radians(m_rotation_current.y)),
						std::sin(glm::radians(m_rotation_current.x)),
						std::cos(glm::radians(m_rotation_current.x)) * std::cos(glm::radians(m_rotation_current.y))));
					m_rotation_delta = glm::vec3();
				}

				if(m_up_delta != glm::vec3()) {
					m_up_delta = glm::vec3();
				}

				if(m_wheel_delta) {
					m_wheel_delta = 0;
				}
			}
		}
	}
}
