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

#ifndef GAEA_ENTITY_H_
#define GAEA_ENTITY_H_

namespace gaea {

	#define ENTITY_ALL SCALAR_INVALID(gaea::entity_t)
	#define ENTITY_MAX ENTITY_CAMERA
	#define ENTITY_SPECIFIER_UNDEFINED SCALAR_INVALID(uint32_t)
	#define ENTITY_VISIBLE_INIT true

	typedef enum {
		ENTITY_CAMERA = 0,
	} entity_t;

	namespace engine {

		namespace entity {

			typedef class _base :
					public gaea::engine::object::base,
					public gaea::engine::event::observer {

				public:

					_base(
						__in gaea::entity_t type,
						__in_opt uint32_t specifier = ENTITY_SPECIFIER_UNDEFINED,
						__in_opt bool visible = ENTITY_VISIBLE_INIT
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

					bool is_visible(void);

					virtual void render(void) = 0;

					uint32_t specifier(void);

					std::string to_string(
						__in_opt bool verbose = false
						);

					gaea::entity_t type(void);

					virtual void update(
						__in GLfloat delta
						) = 0;

					void visible(
						__in bool value
						);

				protected:

					uint32_t m_specifier;

					bool m_visible;

			} base;

			typedef class _manager {

				public:

					~_manager(void);

					static _manager &acquire(void);

					bool contains(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					size_t decrement_reference(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void generate(
						__in gaea::engine::entity::base &object
						);

					size_t increment_reference(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void initialize(void);

					static bool is_allocated(void);

					bool is_initialized(void);

					size_t reference_count(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					void render(void);

					size_t size(void);

					size_t size(
						__in gaea::entity_t type
						);

					std::string to_string(
						__in_opt bool verbose = false
						);

					void uninitialize(void);

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

					std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>::iterator find(
						__in gaea::uid_t id,
						__in gaea::entity_t type
						);

					std::vector<std::map<gaea::uid_t, std::pair<gaea::engine::entity::base &, size_t>>> m_entry;

					bool m_initialized;

					static _manager *m_instance;

			} manager;
		}
	}
}

#endif // GAEA_ENTITY_H_
