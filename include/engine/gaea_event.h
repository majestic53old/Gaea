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

#ifndef GAEA_EVENT_H_
#define GAEA_EVENT_H_

namespace gaea {

	#define EVENT_CAMERA_MAX EVENT_CAMERA_UP_SET
	#define EVENT_INPUT_MAX EVENT_INPUT_WHEEL
	#define EVENT_INVALID SCALAR_INVALID(gaea::event_t)
	#define EVENT_MAX EVENT_INPUT
	#define EVENT_SPECIFIER_UNDEFINED SCALAR_INVALID(uint32_t)

	typedef enum {
		EVENT_UNDEFINED = 0,
		EVENT_CAMERA,
		EVENT_INPUT,
	} event_t;

	enum {
		EVENT_CAMERA_CLIP_SET = 0,
		EVENT_CAMERA_DIMENSIONS_SET,
		EVENT_CAMERA_FOV_SET,
		EVENT_CAMERA_POSITION_DELTA,
		EVENT_CAMERA_POSITION_SET,
		EVENT_CAMERA_ROTATION_DELTA,
		EVENT_CAMERA_ROTATION_SET,
		EVENT_CAMERA_UP_DELTA,
		EVENT_CAMERA_UP_SET,
	};

	enum {
		EVENT_INPUT_BUTTON = 0,
		EVENT_INPUT_KEY,
		EVENT_INPUT_MOTION,
		EVENT_INPUT_WHEEL,
	};

	namespace engine {

		namespace event {

			void notify(
				__in gaea::event_t type,
				__in_opt uint32_t specifier = EVENT_SPECIFIER_UNDEFINED,
				__in_opt void *context = nullptr,
				__in_opt size_t length = 0
				);

			typedef class _base :
					public gaea::engine::object::base {

				public:

					_base(
						__in gaea::event_t type,
						__in uint32_t specifier,
						__in std::vector<uint8_t> &context
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

					const void *context(void);

					size_t length(void);

					void set(
						__in std::vector<uint8_t> &context
						);

					uint32_t specifier(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

					gaea::event_t type(void);

				protected:

					void decrement_reference(void);

					void increment_reference(void);

					std::vector<uint8_t> m_context;

					uint32_t m_specifier;

			} base;

			typedef void (*handler_cb)(
				__in gaea::engine::event::base &event,
				__in void *context
				);

			typedef class _observer {

				public:

					_observer(void);

					_observer(
						__in const _observer &other
						);

					virtual ~_observer(void);

					_observer &operator=(
						__in const _observer &other
						);

					static std::string as_string(
						__in const _observer &object,
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

			} observer;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains_event(
						__in gaea::uid_t id,
						__in gaea::event_t type
						);

					bool contains_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

					size_t decrement_reference(
						__in gaea::uid_t id,
						__in gaea::event_t type
						);

					gaea::uid_t generate(
						__in gaea::event_t type,
						__in_opt uint32_t specifier = EVENT_SPECIFIER_UNDEFINED,
						__in_opt void *context = nullptr,
						__in_opt size_t length = 0
						);

					size_t increment_reference(
						__in gaea::uid_t id,
						__in gaea::event_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in gaea::uid_t id,
						__in gaea::event_t type
						);

					void register_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

					void unregister_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

				protected:

					_manager(void);

					_manager(
						__in const _manager &other
						);

					_manager &operator=(
						__in const _manager &other
						);

					static void _delete(void);

					static void _thread(void);

					void clear(void);

					std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator find_event(
						__in gaea::uid_t id,
						__in gaea::event_t type
						);

					std::set<std::pair<gaea::engine::event::handler_cb, void *>>::iterator find_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type,
						__in_opt void *context = nullptr
						);

					std::vector<std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>> m_event;

					std::vector<std::queue<gaea::uid_t>> m_event_queue;

					std::vector<std::set<std::pair<gaea::engine::event::handler_cb, void *>>> m_handler;

					bool m_initialized;

					static _manager *m_instance;

					std::recursive_mutex m_lock;

					gaea::engine::signal::base m_signal;

					std::thread m_thread;

			} manager;
		}
	}
}

#endif // GAEA_EVENT_H_
