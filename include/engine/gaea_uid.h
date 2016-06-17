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

#ifndef GAEA_UID_H_
#define GAEA_UID_H_

namespace gaea {

	#define UID_INVALID SCALAR_INVALID(gaea::uid_t)

	typedef uint32_t uid_t;

	namespace uid {

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

				bool operator==(
					__in const _base &other
					);

				bool operator!=(
					__in const _base &other
					);

				static std::string as_string(
					__in const _base &object,
					__in_opt bool verbose = false
					);

				const gaea::uid_t &id(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				friend bool operator<(
					__in const _base &left,
					__in const _base &right
					);

				void decrement_reference(void);

				void generate(void);

				void increment_reference(void);

				gaea::uid_t m_id;

		} base;

		bool operator<(
			__in const gaea::uid::base &left,
			__in const gaea::uid::base &right
			);

		typedef class _manager {

			public:

				~_manager(void);

				static _manager &acquire(void);

				bool contains(
					__in const gaea::uid_t &id
					);

				size_t decrement_reference(
					__in const gaea::uid_t &id
					);

				void generate(
					__out gaea::uid_t &id
					);

				size_t increment_reference(
					__in const gaea::uid_t &id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in const gaea::uid_t &id
					);

				size_t size(void);

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

				void clear(void);

				std::map<gaea::uid_t, size_t>::iterator find(
					__in const gaea::uid_t &id
					);

				std::map<gaea::uid_t, size_t> m_entry;

				bool m_initialized;

				static _manager *m_instance;

				gaea::uid_t m_next;

				std::set<gaea::uid_t> m_surplus;

		} manager;
	}
}

#endif // GAEA_UID_H_
