/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:50:05 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/05/30 22:50:35 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char* argv[])
{
    PmergeMe sorter;
    
    if (!sorter.parseInput(argc, argv))
    {
        return 1;
    }
    
    sorter.sortAndMeasure();
    
    return 0;
}