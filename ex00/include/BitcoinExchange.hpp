/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:36:45 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/10 17:03:31 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

class BitcoinExchange
{
private:
    // map car trie automatiquement par clé (dates) et offre la methode upper_bound()
    // qui renvoie la valeur superieur ou sur .end() ce qui nous permet de reculer d'une case 
    std::map<std::string, double> _exchangeRates;
    
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& valueStr, double& value) const;
    std::string findClosestDate(const std::string& date) const;
    void trimWhitespace(std::string& str) const;
    
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();
    
    bool loadDatabase(const std::string& filename);
    bool processInputFile(const std::string& filename) const;
};

#endif