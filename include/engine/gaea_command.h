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

#ifndef GAEA_COMMAND_H_
#define GAEA_COMMAND_H_

namespace gaea {

	#define COMMAND_INVALID SCALAR_INVALID(gaea::cmd_type_t)
	#define COMMAND_MAX COMMAND_INPUT_MOUSE_WHEEL
	#define COMMAND_SPECIFIER_UNDEFINED SCALAR_INVALID(uint32_t)

	typedef enum {
		COMMAND_USER = 0,
		COMMAND_INPUT_KEYBOARD,
		COMMAND_INPUT_MOUSE_BUTTON,
		COMMAND_INPUT_MOUSE_MOTION,
		COMMAND_INPUT_MOUSE_WHEEL,
	} cmd_type_t;

	namespace engine {

		namespace command {

			void notify(
				__in gaea::cmd_type_t type,
				__in_opt uint32_t specifier = COMMAND_SPECIFIER_UNDEFINED,
				__in_opt void *context = nullptr,
				__in_opt uint32_t length = 0
				);

			typedef class _base :
					public gaea::engine::object::base {

				public:

					_base(
						__in gaea::cmd_type_t type,
						__in_opt uint32_t specifier = COMMAND_SPECIFIER_UNDEFINED,
						__in_opt void *context = nullptr,
						__in_opt uint32_t length = 0
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

					void *context(void);

					uint32_t length(void);

					uint32_t specifier(void);

					gaea::cmd_type_t type(void);

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

				protected:

					void decrement_reference(void);

					void generate(void);

					void increment_reference(void);

					void *m_context;

					uint32_t m_length;

					uint32_t m_specifier;

			} base;

			typedef class _observer {

				public:

					// TODO

					virtual std::string to_string(
						__in_opt bool verbose = false
						);

				protected:

					// TODO

					std::queue<std::pair<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>> m_entry;

			} observer;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type
						);

					size_t decrement_reference(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type
						);

					void *generate(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type,
						__in_opt uint32_t specifier = COMMAND_SPECIFIER_UNDEFINED,
						__in_opt void *context = nullptr,
						__in_opt uint32_t length = 0
						);

					size_t increment_reference(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type
						);

					void register_observer(
						__in gaea::engine::command::observer &object,
						__in gaea::cmd_type_t type
						);

					std::string to_string(
						__in_opt bool verbose
						);

					void uninitialize(void);

					void unregister_observer(
						__in gaea::engine::command::observer &object,
						__in gaea::cmd_type_t type
						);

					void update(
						__in GLfloat delta
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

					void clear(void);

					void destroy(
						__in const std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator &entry
						);

					std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>::iterator find(
						__in const gaea::uid_t &id,
						__in gaea::cmd_type_t type
						);

					std::vector<std::map<gaea::uid_t, std::tuple<uint32_t, void *, uint32_t, size_t>>> m_entry;

					std::vector<std::queue<gaea::uid_t>> m_entry_queue;

					bool m_initialized;

					static _manager *m_instance;

					std::vector<std::set<gaea::engine::command::observer *>> m_observer;

			} manager;
		}
	}
}

#endif // GAEA_COMMAND_H_
