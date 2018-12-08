#include "../include/avm.hpp"

Factory::Factory( void )
{
	_funcs_ptr.reserve(5);
	_funcs_ptr.push_back(&Factory::createInt8);
	_funcs_ptr.push_back(&Factory::createInt16);
	_funcs_ptr.push_back(&Factory::createInt32);
	_funcs_ptr.push_back(&Factory::createFloat);
	_funcs_ptr.push_back(&Factory::createDouble);
}

Factory::~Factory( void )
{
}

IOperand const *						Factory::createOperand( eOperandType type, std::string const & value ) const
{
	IOperand const * ( Factory::*_function )( std::string const & value ) const = _funcs_ptr.at(type);
	return ((this->*_function)(value));
}

IOperand const *						Factory::createInt8( std::string const & value ) const
{
	return ( new Operand< char >( value, Int8, this ) );
}

IOperand const *						Factory::createInt16( std::string const & value ) const
{
	return ( new Operand< short >( value, Int16, this ) );
}

IOperand const *						Factory::createInt32( std::string const & value ) const
{
	return ( new Operand< int >( value, Int32, this ) );
}

IOperand const *						Factory::createFloat( std::string const & value ) const
{
	return ( new Operand< float >( value, Float, this ) );
}

IOperand const *						Factory::createDouble( std::string const & value ) const
{
	return ( new Operand< double >( value, Double, this ) );
}
