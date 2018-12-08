#ifndef FACTORY_HPP
# define FACTORY_HPP

# include "avm.hpp"

class Factory
{
public:
	Factory( void );
	~Factory( void );

	Factory &																			operator=( Factory const & rhs );
	IOperand const *																	createOperand( eOperandType type, std::string const & value ) const;

private:
	Factory( Factory const & src );
	IOperand const *																	createInt8( std::string const & value ) const;
	IOperand const *																	createInt16( std::string const & value ) const;
	IOperand const *																	createInt32( std::string const & value ) const;
	IOperand const *																	createFloat( std::string const & value ) const;
	IOperand const *																	createDouble( std::string const & value ) const;

	std::vector< IOperand const * ( Factory::* )( std::string const & value ) const >	_funcs_ptr;
};

#endif
