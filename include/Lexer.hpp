#ifndef LEXER_HPP
# define LEXER_HPP

# include "avm.hpp"

enum cmdName
{
	Push,
	Pop,
	Dump,
	Assert,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Print,
	Min,
	Max,
	Rev,
	Fulldump,
	Avg,
	Exit
};

class Lexer
{
public:
	Lexer( int argc, char **argv );
	~Lexer( void );

	std::vector<std::tuple<int, std::pair<cmdName, std::string>>>		getLexemes( void ) const;

private:
	Lexer( void );
	Lexer( Lexer const & );
	Lexer &																operator=( Lexer const & );

	void																clean_from_comment( std::string & str );
	static inline void													rtrim(std::string &s);
	void																token_identification( std::string str );
	void																check_value_token(std::string str);
	cmdName																convert_to_cmd( std::string token ) const;
	eOperandType														convert_to_type( std::string str ) const;

	void																read_from_stdin( void );
	void																read_from_file( char *filename );

	std::list<std::string>												_content;
	std::vector<std::tuple<int, std::pair<cmdName, std::string>>>		_lexemes;
};

#endif
