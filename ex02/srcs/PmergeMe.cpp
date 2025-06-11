/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:56 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/11 23:52:57 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}

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
        if (num > INT_MAX || num <= 0) {
            std::cerr << "Error" << std::endl;
            return false;
        }
        
        int value = static_cast<int>(num);
        _vec.push_back(value);
        _deq.push_back(value);
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
    
    // Tri avec vector
    std::vector<int> vecCopy = _vec;
    clock_t start1 = clock();
    fordJohnsonSortVector(vecCopy);
    clock_t end1 = clock();
    double time1 = static_cast<double>(end1 - start1) / CLOCKS_PER_SEC * 1000000; // microsecondes
    
    // Tri avec deque
    std::deque<int> deqCopy = _deq;
    clock_t start2 = clock();
    fordJohnsonSortDeque(deqCopy);
    clock_t end2 = clock();
    double time2 = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC * 1000000; // microsecondes
    
    _vec = vecCopy;
    _deq = deqCopy;
    
    // Afficher après tri
    std::cout << "After: ";
    size_t limit = (_vec.size() <= 5) ? _vec.size() : 4;
    for (size_t i = 0; i < limit; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << _vec[i];
    }
    if (_vec.size() > 5) std::cout << " [...]";
    std::cout << std::endl;
    
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << time1 << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << time2 << " us" << std::endl;
}

// ========== FORD-JOHNSON POUR VECTOR ==========

std::vector<int> PmergeMe::createJacobsthalSequenceVector(int n) {
    std::vector<int> jacobsthal;
    if (n <= 0) return jacobsthal;
    
    jacobsthal.push_back(1);
    if (n == 1) return jacobsthal;
    
    jacobsthal.push_back(3);
    if (n <= 3) return jacobsthal;
    
    // Générer la séquence de Jacobsthal : J(n) = J(n-1) + 2*J(n-2)
    int prev2 = 1, prev1 = 3;
    while (true) {
        int next = prev1 + 2 * prev2;
        if (next > n) break;
        jacobsthal.push_back(next);
        prev2 = prev1;
        prev1 = next;
    }
    return jacobsthal;
}

int PmergeMe::binarySearchVector(const std::vector<int>& arr, int value, int end) {
    int left = 0, right = end;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

void PmergeMe::fordJohnsonSortVector(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    
    // Étape 1: Grouper par paires et trier chaque paire
    std::vector<std::pair<int, int> > pairs;
    bool hasStraggler = (n % 2 == 1);
    int straggler = hasStraggler ? arr[n - 1] : 0;
    
    for (int i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    // Étape 2: Trier les paires par leur plus grand élément (récursion)
    std::vector<int> maxElements;
    for (size_t i = 0; i < pairs.size(); ++i)
        maxElements.push_back(pairs[i].first);
    
    if (maxElements.size() > 1)
        fordJohnsonSortVector(maxElements);
    
    // Réorganiser les paires selon l'ordre des maxElements triés
    std::vector<std::pair<int, int> > sortedPairs;
    for (size_t i = 0; i < maxElements.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == maxElements[i]) {
                sortedPairs.push_back(pairs[j]);
                pairs.erase(pairs.begin() + j);
                break;
            }
        }
    }
    pairs = sortedPairs;
    
    // Étape 3: Construire la séquence principale avec les plus grands éléments
    std::vector<int> mainSequence;
    std::vector<int> pendingElements;
    
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainSequence.push_back(pairs[i].first);
        pendingElements.push_back(pairs[i].second);
    }
    
    // Étape 4: Insérer les éléments selon la séquence de Jacobsthal
    if (!pendingElements.empty()) {
        // Insérer le premier élément en tête
        mainSequence.insert(mainSequence.begin(), pendingElements[0]);
        
        // Générer la séquence de Jacobsthal pour les insertions optimales
        std::vector<int> jacobsthal = createJacobsthalSequenceVector(pendingElements.size());
        std::vector<bool> inserted(pendingElements.size(), false);
        inserted[0] = true;
        
        for (size_t i = 0; i < jacobsthal.size(); ++i) {
            int idx = jacobsthal[i] - 1; // Ajuster pour l'index 0
            if (idx < static_cast<int>(pendingElements.size()) && !inserted[idx]) {
                int pos = binarySearchVector(mainSequence, pendingElements[idx], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[idx]);
                inserted[idx] = true;
            }
            
            // Insérer les éléments entre les indices Jacobsthal
            if (i > 0) {
                int prevIdx = jacobsthal[i - 1] - 1;
                for (int j = idx - 1; j > prevIdx; --j) {
                    if (j >= 0 && j < static_cast<int>(pendingElements.size()) && !inserted[j]) {
                        int pos = binarySearchVector(mainSequence, pendingElements[j], mainSequence.size());
                        mainSequence.insert(mainSequence.begin() + pos, pendingElements[j]);
                        inserted[j] = true;
                    }
                }
            }
        }
        
        // Insérer les éléments restants
        for (size_t i = 0; i < inserted.size(); ++i) {
            if (!inserted[i]) {
                int pos = binarySearchVector(mainSequence, pendingElements[i], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[i]);
            }
        }
    }
    
    // Insérer l'élément isolé s'il existe
    if (hasStraggler) {
        int pos = binarySearchVector(mainSequence, straggler, mainSequence.size());
        mainSequence.insert(mainSequence.begin() + pos, straggler);
    }
    
    arr = mainSequence;
}

// ========== FORD-JOHNSON POUR DEQUE ==========

std::deque<int> PmergeMe::createJacobsthalSequenceDeque(int n) {
    std::deque<int> jacobsthal;
    if (n <= 0) return jacobsthal;
    
    jacobsthal.push_back(1);
    if (n == 1) return jacobsthal;
    
    jacobsthal.push_back(3);
    if (n <= 3) return jacobsthal;
    
    int prev2 = 1, prev1 = 3;
    while (true) {
        int next = prev1 + 2 * prev2;
        if (next > n) break;
        jacobsthal.push_back(next);
        prev2 = prev1;
        prev1 = next;
    }
    return jacobsthal;
}

int PmergeMe::binarySearchDeque(const std::deque<int>& arr, int value, int end) {
    int left = 0, right = end;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

void PmergeMe::fordJohnsonSortDeque(std::deque<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    
    // Étape 1: Grouper par paires et trier chaque paire
    std::deque<std::pair<int, int> > pairs;
    bool hasStraggler = (n % 2 == 1);
    int straggler = hasStraggler ? arr[n - 1] : 0;
    
    for (int i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    // Étape 2: Trier les paires par leur plus grand élément (récursion)
    std::deque<int> maxElements;
    for (size_t i = 0; i < pairs.size(); ++i)
        maxElements.push_back(pairs[i].first);
    
    if (maxElements.size() > 1)
        fordJohnsonSortDeque(maxElements);
    
    // Réorganiser les paires selon l'ordre des maxElements triés
    std::deque<std::pair<int, int> > sortedPairs;
    for (size_t i = 0; i < maxElements.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == maxElements[i]) {
                sortedPairs.push_back(pairs[j]);
                pairs.erase(pairs.begin() + j);
                break;
            }
        }
    }
    pairs = sortedPairs;
    
    // Étape 3: Construire la séquence principale
    std::deque<int> mainSequence;
    std::deque<int> pendingElements;
    
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainSequence.push_back(pairs[i].first);
        pendingElements.push_back(pairs[i].second);
    }
    
    // Étape 4: Insérer les éléments selon la séquence de Jacobsthal
    if (!pendingElements.empty()) {
        mainSequence.push_front(pendingElements[0]);
        
        std::deque<int> jacobsthal = createJacobsthalSequenceDeque(pendingElements.size());
        std::deque<bool> inserted(pendingElements.size(), false);
        inserted[0] = true;
        
        for (size_t i = 0; i < jacobsthal.size(); ++i) {
            int idx = jacobsthal[i] - 1;
            if (idx < static_cast<int>(pendingElements.size()) && !inserted[idx]) {
                int pos = binarySearchDeque(mainSequence, pendingElements[idx], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[idx]);
                inserted[idx] = true;
            }
            
            if (i > 0) {
                int prevIdx = jacobsthal[i - 1] - 1;
                for (int j = idx - 1; j > prevIdx; --j) {
                    if (j >= 0 && j < static_cast<int>(pendingElements.size()) && !inserted[j]) {
                        int pos = binarySearchDeque(mainSequence, pendingElements[j], mainSequence.size());
                        mainSequence.insert(mainSequence.begin() + pos, pendingElements[j]);
                        inserted[j] = true;
                    }
                }
            }
        }
        
        for (size_t i = 0; i < inserted.size(); ++i) {
            if (!inserted[i]) {
                int pos = binarySearchDeque(mainSequence, pendingElements[i], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[i]);
            }
        }
    }
    
    if (hasStraggler) {
        int pos = binarySearchDeque(mainSequence, straggler, mainSequence.size());
        mainSequence.insert(mainSequence.begin() + pos, straggler);
    }
    
    arr = mainSequence;
}