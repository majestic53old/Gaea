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

#include <cstdarg>
#include "../include/gaea.h"

namespace gaea {

	#define EXCEPTION_MALFORMED "Malformed exception"

	_exception::_exception(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line
		) :
			std::runtime_error(message),
			m_file(file),
			m_function(function),
			m_line(line)
	{
		return;
	}

	_exception::_exception(
		__in const _exception &other
		) :
			std::runtime_error(other),
			m_file(other.m_file),
			m_function(other.m_function),
			m_line(other.m_line)
	{
		return;
	}

	_exception::~_exception(void)
	{
		return;
	}

	_exception &
	_exception::operator=(
		__in const _exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(other);
			m_file = other.m_file;
			m_function = other.m_function;
			m_line = other.m_line;
		}

		return *this;
	}

	std::string 
	_exception::as_string(
		__in const _exception &object,
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << object.what();

		if(!result.str().empty()) {
			result << " ";
		}

		if(verbose) {
			result << "(";

			if(!object.m_function.empty()) {
				result << object.m_function << ":";
			}

			if(!object.m_file.empty()) {
				result << object.m_file << ":";
			}

			result << object.m_line << ")";
		}

		return result.str();
	}

	const std::string &
	_exception::file(void)
	{
		return m_file;
	}

	const std::string &
	_exception::function(void)
	{
		return m_function;
	}

	void 
	_exception::generate(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line,
		__in const char *format,
		...
		)
	{
		int length;
		va_list arguments;
		std::string buffer;
		std::stringstream result;

		result << message;

		if(format) {
			va_start(arguments, format);
			length = std::vsnprintf(nullptr, 0, format, arguments);
			va_end(arguments);

			if(length > 0) {
				buffer.resize(++length, 0);
				va_start(arguments, format);
				length = std::vsnprintf(&buffer[0], length, format, arguments);
				va_end(arguments);
			}

			if(length < 0) {
				buffer = EXCEPTION_MALFORMED;
			}

			if(!buffer.empty()) {

				if(!result.str().empty()) {
					result << ": ";
				}

				result << buffer;
			}
		}

		throw gaea::exception(result.str(), file, function, line);
	}

	size_t 
	_exception::line(void)
	{
		return m_line;
	}

	std::string 
	_exception::to_string(
		__in_opt bool verbose
		)
	{
		return gaea::exception::as_string(*this, verbose);
	}
}
