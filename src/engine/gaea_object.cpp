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
#include "gaea_object_type.h"

namespace gaea {

	namespace engine {

		namespace object {

			#define OBJECT_STRING(_TYPE_) \
				((_TYPE_) > OBJECT_MAX ? STRING_UNKNOWN : \
				STRING_CHECK(OBJECT_STR[_TYPE_]))

			static const std::string OBJECT_STR[] = {
				"ENTITY", "GL",
				};

			_base::_base(
				__in gaea::type_t type,
				__in_opt uint32_t subtype
				) :
					m_subtype(subtype),
					m_type(type)
			{

				if(m_type > OBJECT_MAX) {
					THROW_GAEA_OBJECT_EXCEPTION_FORMAT(GAEA_OBJECT_EXCEPTION_INVALID, 
						"%x", type);
				}
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::uid::base(other),
					m_subtype(other.m_subtype),
					m_type(other.m_type)
			{
				return;
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
					gaea::engine::uid::base::operator=(other);
					m_subtype = other.m_subtype;
					m_type = other.m_type;
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

				result << gaea::engine::uid::base::as_string(object, verbose)
					<< " [" << OBJECT_STRING(object.m_type);

				if(object.m_subtype != OBJECT_SUBTYPE_UNDEFINED) {
					result << ", " << SCALAR_AS_HEX(uint32_t, object.m_subtype);
				}

				result << "]";

				return result.str();
			}

			uint32_t 
			_base::subtype(void)
			{
				return m_subtype;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::object::base::as_string(*this, verbose);
			}

			gaea::type_t 
			_base::type(void)
			{
				return m_type;
			}
		}
	}
}
