/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:37:05 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/21 02:16:59 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _exchangeRates(other._exchangeRates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        _exchangeRates = other._exchangeRates;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::trimWhitespace(std::string& str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        str = "";
        return;
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    str = str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10)
        return false;
    
    if (date[4] != '-' || date[7] != '-')
        return false;
    
    // Vérifier que les caractères sont des chiffres
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
            return false;
    }
    
    // Extraire les valeurs
    int year, month, day;
    std::stringstream yearSS(date.substr(0, 4));
    std::stringstream monthSS(date.substr(5, 2));
    std::stringstream daySS(date.substr(8, 2));

    if ( !(yearSS >> year)   ||  !yearSS.eof()  ||  year < 1  ||  year > 9999 ||
         !(monthSS >> month) ||  !monthSS.eof() ||  month < 1 ||  month > 12  ||
         !(daySS >> day)     ||  !daySS.eof()   ||  day < 1   ||  day > 31 ) {
        return false;
    }
    
    // Vérifications plus spécifiques pour les mois
    if (month == 2) {
        // si divisible par 4 mais pas par 100 ou si divisble par 400 alors bisextile
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        if (day > 30)
            return false;
    
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const {
    if (valueStr.empty()) {
        std::cout << "Error: bad input => " << valueStr << std::endl;
        return false;
    }
    
    std::stringstream ss(valueStr);
    
    // Conversion avec vérification d'erreur
    if (!(ss >> value) || !ss.eof()) {
        std::cout << "Error: bad input => " << valueStr << std::endl;
        return false;
    }
    if (value < 0) {
        std::cout << "Error: not a positive number." << std::endl;
        return false; }
    if (value > 1000) {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const {
    // upper_bound renvoie un it sur valeur > date sinon sur .end()
    // contrairement à lower_bound qui renvoie >=
    std::map<std::string, double>::const_iterator it = _exchangeRates.upper_bound(date);
    
    // Si on peut reculer, on a notre date ≤
    // si sur .begin() c'est qu'on est sur une date anterieure
    if (it != _exchangeRates.begin()) {
        --it;
        return it->first;
    }
    
    // Aucune date antérieure ou égale disponible
    return "";
}

bool BitcoinExchange::loadDatabase(const std::string& filename) {
    // Utiliser .c_str() pour convertir std::string en const char* pour ifstream (C++98 compatible)
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return false;
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
            continue;
        
        std::string date = line.substr(0, commaPos);
        std::string rateStr = line.substr(commaPos + 1);
        
        trimWhitespace(date);
        trimWhitespace(rateStr);
        
        std::stringstream ss(rateStr);
        double rate;
        
        if (ss >> rate && ss.eof())
            _exchangeRates[date] = rate;
    }
    
    file.close();
    return true;
}

bool BitcoinExchange::processInputFile(const std::string& filename) const {
    // Utiliser .c_str() pour convertir std::string en const char* pour ifstream (C++98 compatible)
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return false;
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        // Ignorer les lignes vides
        trimWhitespace(line);
        if (line.empty())
            continue;
        
        if (firstLine) {
            // Vérifier si la première ligne est un header valide
            // Un header valide contient généralement "date" et "value" (insensible à la casse)
            
            // Convertir en minuscules pour la comparaison
            for (size_t i = 0; i < line.length(); ++i) {
                line[i] = std::tolower(line[i]);
            }
            
            // Vérifier la présence de "date" et "value" dans la ligne
            if (line.find("date") != std::string::npos && 
                line.find("value") != std::string::npos) {
                firstLine = false;
                continue; // Ignorer cette ligne d'en-tête
            }
            
            // Si ce n'est pas un header, traiter cette ligne comme une donnée normale
            firstLine = false;
        }
        
        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string date = line.substr(0, pipePos);
        std::string valueStr = line.substr(pipePos + 1);
        
        trimWhitespace(date);
        trimWhitespace(valueStr);
        
        // Vérifier la validité de la date
        if (!isValidDate(date)) {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }
        
        // Vérifier la validité de la valeur
        double value;
        if (!isValidValue(valueStr, value))
            continue;

        // Trouver la date la plus proche
        std::string closestDate = findClosestDate(date);
        if (closestDate.empty()) {
            std::cout << "Error: no exchange rate available for date " << date << std::endl;
            continue;
        }
        
        double exchangeRate = _exchangeRates.find(closestDate)->second;
        double result = value * exchangeRate;
        
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    
    file.close();
    return true;
}