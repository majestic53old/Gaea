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

	#define EVENT_INPUT_INVALID EVENT_SPECIFIER_UNDEFINED
	#define EVENT_INPUT_MAX EVENT_INPUT_WHEEL
	#define EVENT_INVALID SCALAR_INVALID(gaea::event_t)
	#define EVENT_MAX EVENT_INPUT
	#define EVENT_SPECIFIER_UNDEFINED SCALAR_INVALID(uint32_t)

	typedef enum {
		EVENT_UNDEFINED = 0,
		EVENT_INPUT,
	} event_t;

	enum {
		EVENT_INPUT_BUTTON = 0,
		EVENT_INPUT_KEY,
		EVENT_INPUT_MOTION,
		EVENT_INPUT_WHEEL,
	};

	namespace engine {

		namespace event {

			void signal(
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
						__in_opt uint32_t specifier = EVENT_SPECIFIER_UNDEFINED,
						__in_opt void *context = nullptr,
						__in_opt size_t length = 0
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

					uint32_t specifier(void);

					std::string to_string(
						__in_opt bool verbose = false
						);

					gaea::event_t type(void);

				protected:

					void copy(
						__in void *context,
						__in size_t length
						);

					void decrement_reference(void);

					void generate(void);

					void increment_reference(void);

					std::vector<uint8_t> m_context;

					uint32_t m_specifier;

			} base;

			typedef void (*handler_cb)(
				__in gaea::engine::event::base &event
				);

			typedef class _observer {

				public:

					// TODO

				protected:

					// TODO

			} observer;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains(
						__in gaea::uid_t &id,
						__in gaea::event_t type
						);

					bool contains(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type
						);

					size_t decrement_reference(
						__in gaea::uid_t &id,
						__in gaea::event_t type
						);

					void generate(
						__in gaea::engine::event::base &object
						);

					size_t increment_reference(
						__in gaea::uid_t &id,
						__in gaea::event_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in gaea::uid_t &id,
						__in gaea::event_t type
						);

					void register_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type
						);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

					void unregister_handler(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type
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

					std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>::iterator find(
						__in gaea::uid_t &id,
						__in gaea::event_t type
						);

					std::set<gaea::engine::event::handler_cb>::iterator find(
						__in gaea::engine::event::handler_cb handler,
						__in gaea::event_t type
						);

					std::vector<std::map<gaea::uid_t, std::pair<gaea::engine::event::base, size_t>>> m_entry;

					std::vector<std::queue<gaea::uid_t>> m_entry_queue;

					std::vector<std::set<gaea::engine::event::handler_cb>> m_handler;

					bool m_initialized;

					static _manager *m_instance;

					gaea::engine::signal::base m_signal;

					std::thread m_thread;

			} manager;
		}
	}
}

#endif // GAEA_EVENT_H_
