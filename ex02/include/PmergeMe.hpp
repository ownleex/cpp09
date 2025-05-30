/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:48 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/05/30 22:50:17 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <climits>

class PmergeMe
{
private:
    std::vector<int> _vectorData;
    std::deque<int> _dequeData;
    
    // Algorithme Ford-Johnson pour vector
    void fordJohnsonSort(std::vector<int>& arr);
    void mergeInsertVector(std::vector<int>& arr, int left, int right);
    void insertionSortVector(std::vector<int>& arr, int left, int right);
    
    // Algorithme Ford-Johnson pour deque
    void fordJohnsonSort(std::deque<int>& arr);
    void mergeInsertDeque(std::deque<int>& arr, int left, int right);
    void insertionSortDeque(std::deque<int>& arr, int left, int right);
    
    // Fonctions utilitaires
    bool isValidNumber(const std::string& str) const;
    void printContainer(const std::string& prefix, const std::vector<int>& container) const;
    double getTimeInMicroseconds(clock_t start, clock_t end) const;
    
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    bool parseInput(int argc, char* argv[]);
    void sortAndMeasure();
};

#endif