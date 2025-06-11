/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:48 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/11 21:06:52 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:48 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/11 18:58:41 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <ctime>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int> _deq;
    static const int THRESHOLD = 10;
    
    // Fonction utilitaire pour mesurer le temps
    double getTime();
    
    // Merge-insert sort pour vector
    void mergeInsertSort(std::vector<int>& arr, int left, int right);
    void merge(std::vector<int>& arr, int left, int mid, int right);
    void insertionSort(std::vector<int>& arr, int left, int right);
    
    // Merge-insert sort pour deque  
    void mergeInsertSort(std::deque<int>& arr, int left, int right);
    void merge(std::deque<int>& arr, int left, int mid, int right);
    void insertionSort(std::deque<int>& arr, int left, int right);

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