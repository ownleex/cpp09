/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:48 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/16 17:41:59 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <ctime>
#include <climits>
#include <sstream>
#include <cstdlib>

class PmergeMe {
private:
    // Vector et Deque pour les algorithmes nécessitant accès aléatoire et insertions
    // methode .insert et acces par container[]
    // Vector est plus rapide car meme bloc mémoire pour les données
    // Deque plus lent données stockées sous forme de pointeur dans des espaces mémoire disctints qui doivent être déréférencer
    std::vector<int> _vec;
    std::deque<int> _deq;
    
    // Ford-Johnson pour vector
    void fordJohnsonSortVector(std::vector<int>& vec);
    std::vector<int> createInsertionOrderVector(int n);
    int binarySearchVector(const std::vector<int>& arr, int value, int end);
    
    // Ford-Johnson pour deque
    void fordJohnsonSortDeque(std::deque<int>& deq);
    std::deque<int> createInsertionOrderDeque(int n);
    int binarySearchDeque(const std::deque<int>& arr, int value, int end);

    void display(int n);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    bool parseArgs(int argc, char** argv);
    void sort();
};

#endif