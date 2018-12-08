#ifndef OPERAND_HPP
# define OPERAND_HPP

# include "avm.hpp"
# include "Factory.hpp"
# include "AvmException.hpp"



extern int					g_line;
extern int					g_end_of_read;
extern int					g_exit;

template< typename T >
class Operand : public IOperand
{
public:
	Operand( std::string const & value, eOperandType type, Factory const * factory ) :
		_type(type), _factory(factory)
	{
		isInFlow(value, type);

		_value = static_cast<T>(std::stod(value));

		_string = value;

		if (_type >= Float)
		{
			std::string::iterator	i = std::find(_string.begin(), _string.end(), '.') + 1;
			std::string::iterator	j = _string.end() - 1;
			while (*j == '0' && j != i)
			{
				_string = _string.substr(0, _string.length() - 1);
				i = std::find(_string.begin(), _string.end(), '.') + 1;
				j = _string.end() - 1;
			}

			/*
			**In case: getPrecision should return current precision of an operand
			**
			**_precision = std::distance(_string.end(), std::find(_string.begin(), _string.end(), '.'));
			**_precision = (_precision == 0 ? 0 : abs(_precision + 1));
			*/

			_precision = (_type == Float ? 6 : 15);
		}
		else
		{
			_string = _string.substr(0, std::distance(_string.begin(), std::find(_string.begin(), _string.end(), '.')));
			_precision = 0;
		}
	}

	~Operand( void ) {};

	int							getPrecision( void ) const
	{
		return (_precision);
	}

	eOperandType				getType( void ) const
	{
		return (_type);
	}

	IOperand const *			operator+( IOperand const & rhs ) const
	{
		if (_precision < rhs.getPrecision() ||
			(_precision == rhs.getPrecision() && _type < rhs.getType()))
			return ( rhs + *this );

		std::string				v = my_to_string(std::stold(_string) + std::stold(rhs.toString()), _type);

		isInFlow(v, _type);

		return (_factory->createOperand(_type, v));
	}

	IOperand const *			operator-( IOperand const & rhs ) const
	{
		eOperandType			new_type;

		if (_precision < rhs.getPrecision() ||
			(_precision == rhs.getPrecision() && _type < rhs.getType()))
			new_type = rhs.getType();
		else
			new_type = _type;

		std::string				v = my_to_string(std::stold(_string) - std::stold(rhs.toString()), new_type);

		isInFlow(v, new_type);

		return (_factory->createOperand(new_type, v));
	}

	IOperand const *			operator*( IOperand const & rhs ) const
	{
		if (_precision < rhs.getPrecision() ||
			(_precision == rhs.getPrecision() && _type < rhs.getType()))
			return ( rhs * *this );

		if (_type >= Float)
			underflow_test(rhs, _type, '*');
		std::string				v = my_to_string(std::stold(_string) * std::stold(rhs.toString()), _type);

		isInFlow(v, _type);

		return (_factory->createOperand(_type, v));
	}

	IOperand const *			operator/( IOperand const & rhs ) const
	{
		eOperandType			new_type;

		if (_precision < rhs.getPrecision() ||
			(_precision == rhs.getPrecision() && _type < rhs.getType()))
			new_type = rhs.getType();
		else
			new_type = _type;

		if (rhs.toString() == "0" || rhs.toString() == "0.0")
			throw AvmException("division by zero", g_line);

		if (new_type >= Float)
			underflow_test(rhs, new_type, '/');
		std::string				v = my_to_string(std::stold(_string) / std::stold(rhs.toString()), new_type);

		isInFlow(v, new_type);

		return (_factory->createOperand(new_type, v));
	}

	IOperand const *			operator%( IOperand const & rhs ) const
	{
		eOperandType			new_type;

		if (_precision < rhs.getPrecision() ||
			(_precision == rhs.getPrecision() && _type < rhs.getType()))
			new_type = rhs.getType();
		else
			new_type = _type;

		if (rhs.toString() == "0" || rhs.toString() == "0.0")
			throw AvmException("modulo by zero", g_line);

		std::string				v = my_to_string(std::fmod(std::stold(_string), std::stold(rhs.toString())), new_type);

		isInFlow(v, new_type);

		return (_factory->createOperand(new_type, v));
	}

	void						underflow_test( IOperand const & rhs, eOperandType type, char c ) const
	{
		double d;
		float f;

		feclearexcept(FE_ALL_EXCEPT);
		if (type == Float)
		{
			if (c == '*')
				f = this->_value * std::stof(rhs.toString());
			else
				f = this->_value / std::stof(rhs.toString());
		}
		else
		{
			if (c == '*')
				d = this->_value * std::stod(rhs.toString());
			else
				d = this->_value / std::stod(rhs.toString());
		}
		if (fetestexcept(FE_UNDERFLOW))
			throw AvmException("floting point underflow", g_line);
	}

	std::string const &			toString( void ) const
	{
		return (_string);
	}

private:
	Operand( void ) {};
	void						isInFlow( std::string const & value, eOperandType type ) const
	{
		long double				v = std::stold(value);

		switch (type)
		{
			case Int8:
			{
				if (v < SCHAR_MIN || v > SCHAR_MAX)
					throw AvmException("int8 - overflow on a value", g_line);
				break ;
			}
			case Int16:
			{
				if (v < SHRT_MIN || v > SHRT_MAX)
					throw AvmException("int16 - overflow on a value", g_line);
				break ;
			}
			case Int32:
			{
				if (v < INT_MIN || v > INT_MAX)
					throw AvmException("int32 - overflow on a value", g_line);
				break ;
			}
			case Float:
			{
				if (v < -FLT_MAX || v > FLT_MAX)
					throw AvmException("float - overflow on a value", g_line);
				break ;
			}
			case Double:
			{
				if (v < -DBL_MAX || v > DBL_MAX)
					throw AvmException("double - overflow on a value", g_line);
				break ;
			}
		}
	}

	std::string					my_to_string( long double num, eOperandType new_type ) const
	{
		if (new_type < Float)
			return (std::to_string(num));

		std::ostringstream		stream;

		/*
		** To get rid of scientific notation
		*/
		stream << std::fixed;
		stream << std::setprecision(15);

		stream << num;
		std::string strObj = stream.str();

		return (strObj);
	}

	Operand( Operand const & ) {};
	Operand						operator=( Operand const & ) const;

	std::string					_string;
	eOperandType				_type;
	int							_precision;
	T							_value;
	Factory const *				_factory;
};

#endif
