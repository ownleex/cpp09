/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:48 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/11 21:16:31 by ayarmaya         ###   ########.fr       */
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
    std::vector<int> _vec;
    std::deque<int> _deq;
    
    // Ford-Johnson pour vector
    void fordJohnsonSort(std::vector<int>& arr);
    std::vector<int> createJacobsthalSequence(int n);
    int binarySearch(const std::vector<int>& arr, int value, int end);
    
    // Ford-Johnson pour deque
    void fordJohnsonSort(std::deque<int>& arr);
    std::vector<int> createJacobsthalSequenceDeque(int n);
    int binarySearchDeque(const std::deque<int>& arr, int value, int end);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    bool parseArgs(int argc, char** argv);
    void sort();
    void display();
};

#endif