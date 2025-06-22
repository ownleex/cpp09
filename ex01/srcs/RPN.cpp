/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:43:58 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/22 12:59:20 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN& other) : _operands(other._operands) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other)
    {
        _operands = other._operands;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) const {
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isValidNumber(const std::string& token) const {
    if (token.length() == 1 && std::isdigit(token[0]))
        return true;
    
    return false;
}

double RPN::performOperation(double a, double b, const std::string& op) const {
    if (op == "+")
        return a + b;
    else if (op == "-")
        return a - b;
    else if (op == "*")
        return a * b;
    else if (op == "/") {
        if (b == 0)
            throw std::runtime_error("Error: division by zero");
        return a / b;
    }
    else
        throw std::runtime_error("Error: unknown operator");
}

void RPN::processToken(const std::string& token) {
    if (isValidNumber(token)) {
        double num = std::atoi(token.c_str());
        _operands.push(num);
    }
    else if (isOperator(token)) {
        if (_operands.size() < 2)
            throw std::runtime_error("Error: insufficient operands");
        
        double b = _operands.top();
        _operands.pop();
        double a = _operands.top();
        _operands.pop();
        
        double result = performOperation(a, b, token);
        _operands.push(result);
    }
    else {
        throw std::runtime_error("Error: invalid token");
    }
}

double RPN::evaluate(const std::string& expression) {
    if (expression.find('(') != std::string::npos || 
        expression.find(')') != std::string::npos) {
        throw std::runtime_error("Error");
    }
    
    std::stringstream ss(expression);
    std::string token;
    
    while (ss >> token) {
        processToken(token);
    }
    
    if (_operands.size() != 1)
        throw std::runtime_error("Error: invalid expression");
    
    return _operands.top();
}