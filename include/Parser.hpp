#ifndef PARSER_HPP
# define PARSER_HPP

# include "avm.hpp"
# include "MyStack.hpp"

class Parser
{
public:
	Parser( std::vector<std::tuple<int, std::pair<cmdName, std::string>>> lexemest );
	~Parser( void );

private:
	Parser( void );
	Parser( Parser const & );

	eOperandType																convert_to_type( std::string str ) const;
	std::string																	type_to_string( eOperandType type ) const;

	void																		push( std::string const & str );
	void																		pop( std::string const & );
	void																		dump( std::string const & );
	void																		assertVM( std::string const & str );
	void																		add( std::string const & );
	void																		sub( std::string const & );
	void																		mul( std::string const & );
	void																		div( std::string const & );
	void																		mod( std::string const & );
	void																		print( std::string const & );
	void																		min( std::string const & );
	void																		max( std::string const & );
	void																		rev( std::string const & );
	void																		fulldump( std::string const & );
	void																		avg( std::string const & );

	Parser &																	operator=( Parser const & );
	MyStack<IOperand const *>													_stack;
	Factory																		_factory;
	int																			_exit;
	std::map< cmdName, void ( Parser::* )( std::string const & ) >				_funcs_ptr;
};

#endif
