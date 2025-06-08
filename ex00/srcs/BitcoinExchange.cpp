/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:37:05 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/08 20:06:47 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _exchangeRates(other._exchangeRates)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        _exchangeRates = other._exchangeRates;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::trimWhitespace(std::string& str) const
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
    {
        str = "";
        return;
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    str = str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
    if (date.length() != 10)
        return false;
    
    if (date[4] != '-' || date[7] != '-')
        return false;
    
    // Vérifier que les caractères sont des chiffres
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
            return false;
    }
    
    // Extraire année, mois, jour
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    
    // Vérifications basiques
    if (year < 1000 || year > 9999)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    
    // Vérifications plus spécifiques pour les mois
    if (month == 2) // Février
    {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) // Mois à 30 jours
    {
        if (day > 30)
            return false;
    }
    
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const
{
    std::stringstream ss(valueStr);
    ss >> value;
    
    if (ss.fail() || !ss.eof())
        return false;
    
    if (value < 0)
        return false;
    
    if (value > 1000)
        return false;
    
    return true;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const
{
    std::map<std::string, double>::const_iterator it = _exchangeRates.lower_bound(date);
    
    if (it == _exchangeRates.begin())
    {
        // Si la date est antérieure à toutes les dates de la base
        if (it->first == date)
            return date;
        else
            return ""; // Pas de date précédente disponible
    }
    
    if (it != _exchangeRates.end() && it->first == date)
    {
        return date; // Date exacte trouvée
    }
    
    // Retourner la date précédente la plus proche
    --it;
    return it->first;
}

bool BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue; // Ignorer la première ligne (header)
        }
        
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
            continue;
        
        std::string date = line.substr(0, commaPos);
        std::string rateStr = line.substr(commaPos + 1);
        
        trimWhitespace(date);
        trimWhitespace(rateStr);
        
        double rate;
        std::stringstream ss(rateStr);
        ss >> rate;
        
        if (!ss.fail() && ss.eof())
        {
            _exchangeRates[date] = rate;
        }
    }
    
    file.close();
    return true;
}

void BitcoinExchange::processInputFile(const std::string& filename) const
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue; // Ignorer la première ligne si c'est un header
        }
        
        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos)
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string date = line.substr(0, pipePos);
        std::string valueStr = line.substr(pipePos + 1);
        
        trimWhitespace(date);
        trimWhitespace(valueStr);
        
        // Vérifier la validité de la date
        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }
        
        // Vérifier la validité de la valeur
        double value;
        if (!isValidValue(valueStr, value))
        {
            if (value < 0)
                std::cout << "Error: not a positive number." << std::endl;
            else if (value > 1000)
                std::cout << "Error: too large a number." << std::endl;
            else
                std::cout << "Error: bad input => " << valueStr << std::endl;
            continue;
        }
        
        // Trouver la date la plus proche
        std::string closestDate = findClosestDate(date);
        if (closestDate.empty())
        {
            std::cout << "Error: no exchange rate available for date " << date << std::endl;
            continue;
        }
        
        double exchangeRate = _exchangeRates.find(closestDate)->second;
        double result = value * exchangeRate;
        
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    
    file.close();
}