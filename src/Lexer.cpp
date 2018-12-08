#include "../include/avm.hpp"

Lexer::Lexer( int argc, char **argv )
{
	try
	{
		if (argc == 1)
			Lexer::read_from_stdin();
		else
		{
			if (argc > 2)
				throw AvmException("Too many arguments");
			Lexer::read_from_file(argv[1]);
		}
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
		exit(1);
	}

	_lexemes.reserve(_content.size());
	for_each( _content.begin(), _content.end(), [this](std::string & str) mutable -> void
	{
		if (g_exit == 1)
			return ;
		clean_from_comment(str);
		if (str.size())
			token_identification(str);
		g_line++;
	});
}

Lexer::~Lexer( void )
{
}

std::vector<std::tuple<int, std::pair<cmdName, std::string>>>				Lexer::getLexemes( void ) const
{
	return (_lexemes);
}

Lexer::Lexer( void )
{
}

void																		Lexer::clean_from_comment( std::string & str )
{
	std::string																temp;

	if (str.size())
		temp = str.substr(0, std::distance(str.begin(), std::find(str.begin(), str.end(), COMMENT)));
	if (temp.size() != str.size())
	{
		rtrim(temp);
		str = temp;
	}
}

inline void																	Lexer::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void																		Lexer::token_identification( std::string str )
{
	std::regex																valid_cmd("push|pop|dump|assert|add|sub|mul|div|mod|print|min|max|rev|fulldump|avg|exit");
	char *																	temp = const_cast<char *>(str.c_str());

	try
	{
		if (std::regex_match(str, std::regex(".*\\s$")) || std::regex_match(str, std::regex("\\s.*")))
			throw AvmException("separators at the beginning/end of the line", g_line);

		temp = std::strtok(temp, " \n\t\v\f\r\n");
		std::string															token(temp);
		
		if (std::regex_match(token, valid_cmd))
		{
			temp = std::strtok(NULL, " \n\t\v\f\r\n");
			std::string														token2((!temp ? "none" : temp));

			if (std::strtok(NULL, " \n\t\v\f\r\n") || ((token == "push" || token == "assert") && token2 == "none"))
				throw AvmException("invalid value token", g_line);
			else if (token != "push" && token != "assert" && token2 != "none")
				throw AvmException("superfluous value token is given", g_line);
			else if (token == "exit")
				g_exit = 1;
			else
			{
				if (token == "push")
					check_value_token(std::string(token2));
				_lexemes.push_back(std::make_tuple(g_line, std::make_pair(convert_to_cmd(token), token2)));
			}
		}
		else
			throw AvmException("unknown instruction", g_line);
	}
	catch (std::exception & e)
	{
		std::cout << KRED << "ERROR: " << KNRM << e.what() << std::endl;
	}
}

void																		Lexer::check_value_token(std::string str)
{
	std::smatch									type;
	std::smatch									value;
	std::regex									find_type("^(int(8|16|32)|float|double)");
	std::regex									find_value_int("\\([-+]?\\d+\\)$");
	std::regex									find_value_fract("\\([-+]?\\d+\\.\\d+\\)$");

	if (!std::regex_search(str, type, find_type))
		throw AvmException("invalid type", g_line);
	if (!std::regex_search(str, value, (convert_to_type(type.str(0)) < 3 ? find_value_int : find_value_fract)))
		throw AvmException("invalid value", g_line);

	if (convert_to_type(type.str(0)) < 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\)")))
		throw AvmException("extra symbols between type and value", g_line);
	if (convert_to_type(type.str(0)) >= 3 && !std::regex_match(str, std::regex("(int(8|16|32)|float|double)\\([-+]?\\d+\\.\\d+\\)")))
		throw AvmException("extra symbols between type and value", g_line);
}

cmdName																		Lexer::convert_to_cmd( std::string token ) const
{
	if (token == "push")
		return (Push);
	else if (token == "pop")
		return (Pop);
	else if (token == "dump")
		return (Dump);
	else if (token == "assert")
		return (Assert);
	else if (token == "add")
		return (Add);
	else if (token == "sub")
		return (Sub);
	else if (token == "mul")
		return (Mul);
	else if (token == "div")
		return (Div);
	else if (token == "mod")
		return (Mod);
	else if (token == "print")
		return (Print);
	else if (token == "min")
		return (Min);
	else if (token == "max")
		return (Max);
	else if (token == "rev")
		return (Rev);
	else if (token == "fulldump")
		return (Fulldump);
	else if (token == "avg")
		return (Avg);
	else
		return (Exit);
}

eOperandType									Lexer::convert_to_type( std::string str ) const
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

void																		Lexer::read_from_stdin( void )
{
	std::string																line;

	while (getline(std::cin, line))
	{
		if (line == END_OF_READ)
			return ;
		_content.push_back(line);
	}
	g_end_of_read = 0;
}

void																		Lexer::read_from_file( char *filename )
{
	std::ifstream															file;
	std::string																line;

	file.open(filename);
	if (!file.is_open())
		throw AvmException("file error");

	while (getline(file, line))
		_content.push_back(line);

	file.close();
}
