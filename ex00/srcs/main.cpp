/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:37:21 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/08 17:56:20 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./BitcoinExchange file" << std::endl;
        return 1;
    }
    
    BitcoinExchange exchange;
    
    // Charger la base de données des taux de change
    if (!exchange.loadDatabase("data.csv"))
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    
    // Traiter le fichier d'entrée
    exchange.processInputFile(argv[1]);
    
    return 0;
}