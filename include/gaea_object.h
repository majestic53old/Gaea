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

#ifndef GAEA_OBJECT_H_
#define GAEA_OBJECT_H_

namespace gaea {

	#define OBJECT_INVALID SCALAR_INVALID(gaea::type_t)
	#define OBJECT_MAX OBJECT_GL
	#define OBJECT_SUBTYPE_UNDEFINED SCALAR_INVALID(uint32_t)

	typedef enum {
		OBJECT_GL = 0,
	} type_t;

	namespace object {

		typedef class _base :
				public gaea::uid::base {

			public:

				_base(
					__in gaea::type_t type,
					__in_opt uint32_t subtype = OBJECT_SUBTYPE_UNDEFINED
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

				uint32_t subtype(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				gaea::type_t type(void);

			protected:

				uint32_t m_subtype;

				gaea::type_t m_type;

		} base;
	}
}

#endif // GAEA_OBJECT_H_
