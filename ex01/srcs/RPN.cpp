/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:43:58 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/05/30 22:45:54 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN()
{
}

RPN::RPN(const RPN& other)
{
    _operands = other._operands;
}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
    {
        _operands = other._operands;
    }
    return *this;
}

RPN::~RPN()
{
}

bool RPN::isOperator(const std::string& token) const
{
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isValidNumber(const std::string& token) const
{
    if (token.empty())
        return false;
    
    // Vérifier si c'est un chiffre unique (< 10)
    if (token.length() == 1 && std::isdigit(token[0]))
        return true;
    
    // Gérer les nombres négatifs (bien que pas explicitement demandé)
    size_t start = 0;
    if (token[0] == '-' && token.length() > 1)
        start = 1;
    
    for (size_t i = start; i < token.length(); i++)
    {
        if (!std::isdigit(token[i]))
            return false;
    }
    
    // Vérifier que le nombre est < 10 (selon les consignes)
    int num = std::atoi(token.c_str());
    return (num >= 0 && num < 10);
}

double RPN::performOperation(double a, double b, const std::string& op) const
{
    if (op == "+")
        return a + b;
    else if (op == "-")
        return a - b;
    else if (op == "*")
        return a * b;
    else if (op == "/")
    {
        if (b == 0)
            throw std::runtime_error("Error: division by zero");
        return a / b;
    }
    else
        throw std::runtime_error("Error: unknown operator");
}

void RPN::processToken(const std::string& token)
{
    if (isValidNumber(token))
    {
        double num = std::atoi(token.c_str());
        _operands.push(num);
    }
    else if (isOperator(token))
    {
        if (_operands.size() < 2)
            throw std::runtime_error("Error: insufficient operands");
        
        double b = _operands.top();
        _operands.pop();
        double a = _operands.top();
        _operands.pop();
        
        double result = performOperation(a, b, token);
        _operands.push(result);
    }
    else
    {
        throw std::runtime_error("Error: invalid token");
    }
}

double RPN::evaluate(const std::string& expression)
{
    // Vider la pile au cas où elle contiendrait des données précédentes
    while (!_operands.empty())
        _operands.pop();
    
    std::stringstream ss(expression);
    std::string token;
    
    // Vérifier s'il y a des parenthèses (interdites)
    if (expression.find('(') != std::string::npos || 
        expression.find(')') != std::string::npos)
    {
        throw std::runtime_error("Error");
    }
    
    while (ss >> token)
    {
        processToken(token);
    }
    
    // À la fin, il doit y avoir exactement un élément dans la pile
    if (_operands.size() != 1)
        throw std::runtime_error("Error: invalid expression");
    
    return _operands.top();
}