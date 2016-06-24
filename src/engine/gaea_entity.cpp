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
#include "gaea_entity_type.h"

namespace gaea {

	namespace engine {

		namespace entity {

			static const std::string ENTITY_STR[] = {
				"CAMERA",
				};

			#define ENTITY_STRING(_TYPE_) \
				((_TYPE_) > ENTITY_MAX ? STRING_UNKNOWN : \
				ENTITY_STR[_TYPE_])

			_base::_base(
				__in gaea::entity_t type,
				__in_opt uint32_t specifier
				) :
					gaea::engine::object::base(OBJECT_EVENT, type),
					m_specifier(specifier)
			{

				if(type > ENTITY_MAX) {
					THROW_GAEA_ENTITY_EXCEPTION_FORMAT(GAEA_ENTITY_EXCEPTION_INVALID,
						"%x", type);
				}
			}

			_base::_base(
				__in const _base &other
				) :
					gaea::engine::object::base(other),
					m_specifier(other.m_specifier)
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
					gaea::engine::object::base::operator=(other);
					m_specifier = other.m_specifier;
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
					<< " [" << ENTITY_STRING((gaea::entity_t) object.m_subtype);

				if(object.m_specifier != ENTITY_SPECIFIER_UNDEFINED) {
					result << ", " << SCALAR_AS_HEX(uint32_t, object.m_specifier);
				}

				result << "]";

				return result.str();
			}

			uint32_t 
			_base::specifier(void)
			{
				return m_specifier;
			}

			std::string 
			_base::to_string(
				__in_opt bool verbose
				)
			{
				return gaea::engine::entity::base::as_string(*this, verbose);
			}

			gaea::entity_t 
			_base::type(void)
			{
				return (gaea::entity_t) m_subtype;
			}

			// TODO
		}
	}
}
