#ifndef MYSTACK_HPP
# define MYSTACK_HPP

# include "avm.hpp"

template<typename T>
class MyStack : public std::stack<T>
{
public:
	typedef typename std::stack<T>::container_type::iterator iterator;
	
	MyStack( void ) {};
	~MyStack( void ) {};
	MyStack( MyStack const & src ) : std::stack<T>(src) {}

	using		std::stack<T>::operator=;
	
	iterator	begin() { return std::stack<T>::c.begin(); }
	iterator	end() { return std::stack<T>::c.end(); }
	void		swap() { std::stack<T>::c.swap(); }
};

#endif
