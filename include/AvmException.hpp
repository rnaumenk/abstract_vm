#ifndef AVMEXCEPTION_HPP
# define AVMEXCEPTION_HPP

extern int					g_error;

class AvmException : public std::exception
	{
	public:
		AvmException( std::string const & message );
		AvmException( std::string const & message, int line );
		AvmException( AvmException const & src );
		~AvmException( void ) throw();

		std::string						getMessage( void ) const;
		AvmException &					operator=( AvmException const & rhs );
		virtual const char * 			what() const throw();

	private:
		AvmException( void );

		std::string						_message;
		int								_line;
	};

#endif