#include "../include/avm.hpp"

AvmException::AvmException( void )
{
}

AvmException::AvmException( std::string const & message ) : _message(message), _line(-1)
{
}

AvmException::AvmException( std::string const & message, int line ) : _message(message), _line(line)
{
	/*
	** If you want the program to perform despite lexical error(s) found, set g_error to 0
	** else - g_error = 1
	*/
	g_error = 0;
	_message.append(std::string(" (line").append(std::to_string(_line)).append(")"));
}

AvmException::AvmException( AvmException const & src )
{
	*this = src;
}

AvmException::~AvmException( void ) throw()
{
}

const char *					AvmException::what() const throw()
{
	return (_message.c_str());
}

std::string						AvmException::getMessage( void ) const
{
	return (_message);
}

AvmException &					AvmException::operator=( AvmException const & rhs )
{
	this->_message = rhs.getMessage();
	return (*this);
}
