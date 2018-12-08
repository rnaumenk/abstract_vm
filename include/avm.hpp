/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnaumenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 16:13:04 by rnaumenk          #+#    #+#             */
/*   Updated: 2018/08/08 16:13:05 by rnaumenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_HPP
# define AVM_HPP

# include <iostream>
# include <map>
# include <cmath>
# include <cfloat>
# include <fstream>
# include <cstring>
# include <regex>
# include <vector>
# include <list>
# include <stack>
# include <sstream>
# include <iomanip>
# include <fenv.h>
# include "IOperand.hpp"
# include "Operand.hpp"
# include "Factory.hpp"
# include "AvmException.hpp"
# include "Lexer.hpp"
# include "Parser.hpp"
# include "MyStack.hpp"

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"

# define END_OF_READ ";;"
# define COMMENT ';'

#endif
