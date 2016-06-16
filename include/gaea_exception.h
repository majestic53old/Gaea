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

#ifndef GAEA_EXCEPTION_H_
#define GAEA_EXCEPTION_H_

#include <stdexcept>

namespace gaea {

	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define THROW_EXCEPTION(_EXCEPT_) \
		gaea::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, nullptr)
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		gaea::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, \
		__VA_ARGS__)

	typedef class _exception :
			public std::runtime_error {

		public:

			_exception(
				__in const std::string &message,
				__in const std::string &file,
				__in const std::string &function,
				__in size_t line
				);

			_exception(
				__in const _exception &other
				);

			virtual ~_exception(void);

			_exception &operator=(
				__in const _exception &other
				);

			static std::string as_string(
				__in const _exception &object,
				__in_opt bool verbose = false
				);

			const std::string &file(void);

			const std::string &function(void);

			static void generate(
				__in const std::string &message,
				__in const std::string &file,
				__in const std::string &function,
				__in size_t line,
				__in const char *format,
				...
				);

			size_t line(void);

			virtual std::string to_string(
				__in_opt bool verbose = false
				);

		protected:

			std::string m_file;

			std::string m_function;

			size_t m_line;

	} exception;
}

#endif // GAEA_EXCEPTION_H_
