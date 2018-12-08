#include "../include/avm.hpp"

Parser::Parser( std::vector<std::tuple<int, std::pair<cmdName, std::string>>> lexemes ) : _exit(0)
{
	_funcs_ptr.insert(
		{
			{Push, &Parser::push},
			{Pop, &Parser::pop},
			{Dump, &Parser::dump},
			{Assert, &Parser::assertVM},
			{Add, &Parser::add},
			{Sub, &Parser::sub},
			{Mul, &Parser::mul},
			{Div, &Parser::div},
			{Mod, &Parser::mod},
			{Print, &Parser::print},
			{Min, &Parser::min},
			{Max, &Parser::max},
			{Rev, &Parser::rev},
			{Fulldump, &Parser::fulldump},
			{Avg, &Parser::avg}
		}
	);

	try
	{
		if (g_exit == 0)
			throw AvmException("no exit instruction", g_line);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
	try
	{
		if (g_end_of_read == 0)
			throw AvmException("stdin terminator not found", g_line);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}

	void ( Parser::*function )( std::string const & );

	for (auto a : lexemes)
	{
		function = _funcs_ptr.at(std::get<1>(a).first);
		g_line = std::get<0>(a);
		if (!g_error || std::get<1>(a).first == Push)
			(this->*function)(std::get<1>(a).second);
	}


}

Parser::~Parser( void )
{
}

Parser::Parser( void )
{
}

Parser::Parser( Parser const & )
{
}

eOperandType									Parser::convert_to_type( std::string str ) const
{
	std::string									s = str;

	if (s == "int8")
		return (Int8);
	else if (s == "int16")
		return (Int16);
	else if (s == "int32")
		return (Int32);
	else if (s == "float")
		return (Float);
	else
		return (Double);
}

std::string										Parser::type_to_string( eOperandType type ) const
{
	if (type == Int8)
		return ("int8");
	else if (type == Int16)
		return ("int16");
	else if (type == Int32)
		return ("int32");
	else if (type == Float)
		return ("float");
	else
		return ("double");
}

void											Parser::push( std::string const & str )
{
	std::smatch									type;
	std::smatch									value;
	std::regex									find_type("^(int(8|16|32)|float|double)");
	std::regex									find_value_int("\\([-+]?\\d+\\)$");
	std::regex									find_value_fract("\\([-+]?\\d+\\.\\d+\\)$");

	try
	{
		if (!std::regex_search(str, type, find_type))
			throw AvmException("invalid type", g_line);
		if (!std::regex_search(str, value, (convert_to_type(type.str(0)) < 3 ? find_value_int : find_value_fract)))
			throw AvmException("invalid value", g_line);

		if (convert_to_type(type.str(0)) < 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\)")))
			throw AvmException("extra symbols between type and value", g_line);
		if (convert_to_type(type.str(0)) >= 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\.\\d+\\)")))
			throw AvmException("extra symbols between type and value", g_line);

		if (!g_error)
			_stack.push(_factory.createOperand(convert_to_type(type.str(0)), value.str(0).substr(1, value.str(0).length() - 2)));
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::pop( std::string const & )
{
	try
	{
		if (_stack.empty())
			throw AvmException("pop on an empty stack", g_line);
		IOperand const *						temp = *(_stack.end() - 1);
		_stack.pop();
		delete temp;
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::dump( std::string const & )
{
	if (!_stack.empty())
	{
		for (MyStack<IOperand const *>::iterator it = _stack.end() - 1; it >= _stack.begin(); it--)
			std::cout << (*it)->toString() << std::endl;
	}
}

void											Parser::assertVM( std::string const & str )
{
	std::smatch									type;
	std::smatch									value;
	std::regex									find_type("^(int(8|16|32)|float|double)");
	std::regex									find_value_int("\\([-+]?\\d+\\)$");
	std::regex									find_value_fract("\\([-+]?\\d+\\.\\d+\\)$");

	try
	{
		if (_stack.empty() || !std::regex_search(str, type, find_type) ||
			!std::regex_search(str, value, (convert_to_type(type.str(0)) < 3 ? find_value_int : find_value_fract)))
			throw AvmException("an assert instruction is not true", g_line);
		if (convert_to_type(type.str(0)) < 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\)")))
			throw AvmException("an assert instruction is not true", g_line);
		if (convert_to_type(type.str(0)) >= 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\.\\d+\\)")))
			throw AvmException("an assert instruction is not true", g_line);

		MyStack<IOperand const *>::iterator		it = _stack.end() - 1;

		if ((*it)->getType() != convert_to_type(type.str(0)) || (*it)->toString() != value.str(0).substr(1, value.str(0).length() - 2))
			throw AvmException("an assert instruction is not true", g_line);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::add( std::string const & )
{
	try
	{
		if (_stack.size() < 2)
			throw AvmException("the stack is composed of less than two values", g_line);

		IOperand const *						first = *(_stack.end() - 2);
		IOperand const *						second = *(_stack.end() - 1);

		IOperand const *						temp = *first + *second;

		pop(std::string(""));
		pop(std::string(""));
		_stack.push(temp);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::sub( std::string const & )
{
	try
	{
		if (_stack.size() < 2)
			throw AvmException("the stack is composed of less than two values", g_line);

		IOperand const *						first = *(_stack.end() - 2);
		IOperand const *						second = *(_stack.end() - 1);

		IOperand const *						temp = *first - *second;

		pop(std::string(""));
		pop(std::string(""));
		_stack.push(temp);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::mul( std::string const & )
{
	try
	{
		if (_stack.size() < 2)
			throw AvmException("the stack is composed of less than two values", g_line);

		IOperand const *						first = *(_stack.end() - 2);
		IOperand const *						second = *(_stack.end() - 1);

		IOperand const *						temp = *first * *second;

		pop(std::string(""));
		pop(std::string(""));
		_stack.push(temp);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void										Parser::div( std::string const & )
{
	try
	{
		if (_stack.size() < 2)
			throw AvmException("the stack is composed of less than two values", g_line);

		IOperand const *						first = *(_stack.end() - 2);
		IOperand const *						second = *(_stack.end() - 1);

		IOperand const *						temp = *first / *second;

		pop(std::string(""));
		pop(std::string(""));
		_stack.push(temp);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::mod( std::string const & )
{
	try
	{
		if (_stack.size() < 2)
			throw AvmException("the stack is composed of less than two values", g_line);

		IOperand const *						first = *(_stack.end() - 2);
		IOperand const *						second = *(_stack.end() - 1);
		IOperand const *						temp = *first % *second;

		pop(std::string(""));
		pop(std::string(""));
		_stack.push(temp);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void										Parser::print( std::string const & )
{
	try
	{
		if (_stack.empty())
			throw AvmException("print on an empty stack", g_line);
		if ((*(_stack.end() - 1))->getType() != Int8)
			throw AvmException("print -> wrong type", g_line);
		std::cout << static_cast<char>(std::stoi((*(_stack.end() - 1))->toString())) << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::min( std::string const & )
{
	try
	{
		if (_stack.empty())
			throw AvmException("min on an empty stack", g_line);

		IOperand const *						temp = *(_stack.begin());

		for (auto a : _stack)
		{
			if (std::stold(a->toString()) < std::stold(temp->toString()))
				temp = a;
		}
		std::cout << temp->toString() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::max( std::string const & )
{
	try
	{
		if (_stack.empty())
			throw AvmException("max on an empty stack", g_line);

		IOperand const *						temp = *(_stack.begin());

		for (auto a : _stack)
		{
			if (std::stold(a->toString()) > std::stold(temp->toString()))
				temp = a;
		}
		std::cout << temp->toString() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void											Parser::rev( std::string const & )
{
	MyStack<IOperand const *>					temp;
	MyStack<IOperand const *>::iterator			orig_stack = _stack.end() - 1;

	while (orig_stack >= _stack.begin())
	{
		temp.push(_factory.createOperand((*orig_stack)->getType(), (*orig_stack)->toString()));
		orig_stack--;
	}

	swap(_stack, temp);

	for (auto a : temp)
		delete a;
}

void											Parser::fulldump( std::string const & )
{
	std::list<std::string>						temp;

	for (auto a : _stack)
		temp.push_front((type_to_string(a->getType()).append(std::string("(").append(a->toString()))).append(")"));
	for (auto a : temp)
		std::cout << a << std::endl;
}

void											Parser::avg( std::string const & )
{
	try
	{
		if (_stack.empty())
			throw AvmException("avg on an empty stack", g_line);

		long double								sum = 0;
		int										quantity = 0;
		eOperandType							type = Int8;

		for (auto a : _stack)
		{
			if (a->getType() > type)
				type = a->getType();
			sum += std::stold(a->toString());
			quantity++;
		}

		IOperand const *						temp(_factory.createOperand(type, std::to_string(sum /= quantity)));

		std::cout << temp->toString() << std::endl;

		delete temp;
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}
