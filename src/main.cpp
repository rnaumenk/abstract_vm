/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnaumenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 16:11:54 by rnaumenk          #+#    #+#             */
/*   Updated: 2018/08/08 16:11:56 by rnaumenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/avm.hpp"

int		main(int argc, char **argv)
{
	Lexer				lexer(argc, argv);
	Parser				parser(lexer.getLexemes());

	return (0);
}
