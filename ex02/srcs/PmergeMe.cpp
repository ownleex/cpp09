/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:56 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/11 19:40:11 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <iomanip>
#include <sys/time.h>

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}

bool PmergeMe::parseArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        // Vérifier que c'est un nombre positif
        for (size_t j = 0; j < arg.length(); ++j) {
            if (!isdigit(arg[j])) {
                std::cerr << "Error" << std::endl;
                return false;
            }
        }

        // Convertir et vérifier la limite
        long long num = std::atoll(arg.c_str());
        if (num > INT_MAX) {
            std::cerr << "Error" << std::endl;
            return false;
        }

        // Ajouter aux conteneurs
        _vec.push_back(static_cast<int>(num));
        _deq.push_back(static_cast<int>(num));
    }
    return !_vec.empty();
}

void PmergeMe::display() {
    std::cout << "Before: ";
    size_t limit = (_vec.size() <= 5) ? _vec.size() : 4;
    for (size_t i = 0; i < limit; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << _vec[i];
    }
    if (_vec.size() > 5) std::cout << " [...]";
    std::cout << std::endl;
}

void PmergeMe::sort() {
    display();

    const int repeat = 1000;
    double total_time1 = 0;
    for (int i = 0; i < repeat; ++i) {
        std::vector<int> tmp = _vec;
        timeval start1, end1;
        gettimeofday(&start1, NULL);
        mergeInsertSort(tmp, 0, tmp.size() - 1);
        gettimeofday(&end1, NULL);
        total_time1 += (end1.tv_sec - start1.tv_sec) * 1e6 + (end1.tv_usec - start1.tv_usec);
    }
    double time1 = total_time1 / repeat;

    double total_time2 = 0;
    for (int i = 0; i < repeat; ++i) {
        std::deque<int> tmp = _deq;
        timeval start2, end2;
        gettimeofday(&start2, NULL);
        mergeInsertSort(tmp, 0, tmp.size() - 1);
        gettimeofday(&end2, NULL);
        total_time2 += (end2.tv_sec - start2.tv_sec) * 1e6 + (end2.tv_usec - start2.tv_usec);
    }
    double time2 = total_time2 / repeat;

    // Afficher après tri
    std::cout << "After: ";
    size_t limit = (_vec.size() <= 5) ? _vec.size() : 4;
    for (size_t i = 0; i < limit; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << _vec[i];
    }
    if (_vec.size() > 5) std::cout << " [...]";
    std::cout << std::endl;

    // Afficher les temps
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(5) 
              << time1 << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << std::fixed << std::setprecision(5) 
              << time2 << " us" << std::endl;
}


// ========== VECTOR IMPLEMENTATION ==========

void PmergeMe::insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> L(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;

    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < L.size())
        arr[k++] = L[i++];
    while (j < R.size())
        arr[k++] = R[j++];
}

void PmergeMe::mergeInsertSort(std::vector<int>& arr, int left, int right) {
    if (right - left <= THRESHOLD) {
        insertionSort(arr, left, right);
    } else {
        int mid = left + (right - left) / 2;
        mergeInsertSort(arr, left, mid);
        mergeInsertSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// ========== DEQUE IMPLEMENTATION ==========

void PmergeMe::insertionSort(std::deque<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::merge(std::deque<int>& arr, int left, int mid, int right) {
    std::deque<int> L(arr.begin() + left, arr.begin() + mid + 1);
    std::deque<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;

    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < L.size())
        arr[k++] = L[i++];
    while (j < R.size())
        arr[k++] = R[j++];
}

void PmergeMe::mergeInsertSort(std::deque<int>& arr, int left, int right) {
    if (right - left <= THRESHOLD) {
        insertionSort(arr, left, right);
    } else {
        int mid = left + (right - left) / 2;
        mergeInsertSort(arr, left, mid);
        mergeInsertSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
