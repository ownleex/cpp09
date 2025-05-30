/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:43:19 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/05/30 22:44:07 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cctype>

class RPN
{
private:
    std::stack<double> _operands;
    
    bool isOperator(const std::string& token) const;
    bool isValidNumber(const std::string& token) const;
    double performOperation(double a, double b, const std::string& op) const;
    void processToken(const std::string& token);
    
public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();
    
    double evaluate(const std::string& expression);
};

#endif