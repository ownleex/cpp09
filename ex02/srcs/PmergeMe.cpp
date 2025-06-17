/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:56 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/06/17 20:18:05 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/* ======================== CONSTRUCTEURS ET DESTRUCTEUR ======================== */

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

/* ======================== MÉTHODES PUBLIQUES ======================== */

/**
 * Parse les arguments du programme et vérifie qu'ils sont valides
 * @param argc: nombre d'arguments
 * @param argv: tableau des arguments
 * @return true si tous les arguments sont valides, false sinon
 */
bool PmergeMe::parseArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        
        for (size_t j = 0; j < arg.length(); ++j) {
            if (!std::isdigit(arg[j])) {
                std::cerr << "Error" << std::endl;
                return false;
            }
        }
        
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

/**
 * Affiche la séquence avant et après le tri
 */
void PmergeMe::display(int n) {
    if (n == 0)
        std::cout << "Before: ";
    else
        std::cout << "After: ";
        
        
    size_t limit = (_vec.size() <= 10) ? _vec.size() : 10;    
    for (size_t i = 0; i < limit; ++i) {
        std::cout << _vec[i] << " ";
    }

    if (_vec.size() > 10) std::cout << "[...]";
    std::cout << std::endl;
}

/**
 * Lance le tri avec les deux conteneurs et mesure les temps d'exécution
 */
void PmergeMe::sort() {
    display(0);
    
    clock_t start1 = std::clock();
    fordJohnsonSortVector(_vec);
    clock_t end1 = std::clock();
    double time1 = static_cast<double>(end1 - start1) / CLOCKS_PER_SEC * 1000.0;
    
    clock_t start2 = std::clock();
    fordJohnsonSortDeque(_deq);
    clock_t end2 = std::clock();
    double time2 = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC * 1000.0;
    
    display(1);
    
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << time1 << " ms" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << time2 << " ms" << std::endl;
}

/* ======================== FORD-JOHNSON POUR VECTOR ======================== */

/**
 * Génère la séquence d'index jusqu'à size
 * La séquence d'index : 1, 3, 5, 11, 21, 43, 85, 171, ...
 * Formule : J[0]=1, J[1]=3 puis J[i] = J[i-1] + 2*J[i-2]
 * Cette séquence détermine l'ordre optimal d'insertion pour minimiser les comparaisons
 */
std::vector<int> PmergeMe::createInsertionOrderVector(int size) {
    std::vector<int> insertionOrder;
    if (size >= 1)
        insertionOrder.push_back(1);
    
    if (size >= 3) 
        insertionOrder.push_back(3);
    
    int prev2 = 1, prev1 = 3;
    while (true) {
        int next = prev1 + 2 * prev2;
        if (next > size) break;
        insertionOrder.push_back(next);
        prev2 = prev1;
        prev1 = next;
    }
    return insertionOrder;
}

/**
 * Recherche binaire (dichotomique) pour trouver la position d'insertion
 * @param vec: le vecteur dans lequel chercher
 * @param value: la valeur à insérer
 * @param end: l'index de fin de recherche
 * @return l'index où insérer la valeur
 * ici on compare tout le temps à la moitié puis à la moitié restante et ainsi de suite
 */
int PmergeMe::binarySearchVector(const std::vector<int>& vec, int value, int end) {
    int left = 0, right = end;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (vec[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

/**
 * ALGORITHME FORD-JOHNSON (MERGE-INSERT SORT) POUR VECTOR
 * 
 * L'algorithme Ford-Johnson est un algorithme de tri par comparaison qui minimise
 * le nombre de comparaisons nécessaires. Il procède en plusieurs étapes :
 * 
 * 1. Groupement par paires et tri local
 * 2. Tri récursif des éléments maximaux
 * 3. Construction de la séquence principale
 * 4. Insertion optimale selon la séquence d'insertion de Ford et Johnson
 */
void PmergeMe::fordJohnsonSortVector(std::vector<int>& vec) {
    int size = vec.size();
    if (size <= 1) return;
    
    std::vector<std::pair<int, int> > pairs;
    bool hasAlone = (size % 2 == 1);
    int alone = hasAlone ? vec[size - 1] : 0;
    
    for (int i = 0; i < size - (hasAlone ? 1 : 0); i += 2) {
        if (vec[i] > vec[i + 1])
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        else
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
    }
    
    std::vector<int> maxElements;
    for (size_t i = 0; i < pairs.size(); ++i)
        maxElements.push_back(pairs[i].first);
    
    if (maxElements.size() > 1)
        fordJohnsonSortVector(maxElements);
    
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
    
    std::vector<int> mainSequence;
    std::vector<int> pendingElements;
    
    for (size_t i = 0; i < sortedPairs.size(); ++i) {
        mainSequence.push_back(sortedPairs[i].first);
        pendingElements.push_back(sortedPairs[i].second);
    }
    
    if (!pendingElements.empty()) {
        mainSequence.insert(mainSequence.begin(), pendingElements[0]);
        
        std::vector<int> insertionOrder = createInsertionOrderVector(pendingElements.size());
        std::vector<bool> inserted(pendingElements.size(), false);
        inserted[0] = true;
        
        for (size_t i = 0; i < insertionOrder.size(); ++i) {
            int idx = insertionOrder[i] - 1;
            if (!inserted[idx]) {
                int pos = binarySearchVector(mainSequence, pendingElements[idx], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[idx]);
                inserted[idx] = true;
            }
            
            if (i > 0) {
                int prevIdx = insertionOrder[i - 1] - 1;
                for (int j = idx - 1; j > prevIdx; --j) {
                    if (j >= 0 && !inserted[j]) {
                        int pos = binarySearchVector(mainSequence, pendingElements[j], mainSequence.size());
                        mainSequence.insert(mainSequence.begin() + pos, pendingElements[j]);
                        inserted[j] = true;
                    }
                }
            }
        }
        
        for (size_t i = 0; i < inserted.size(); ++i) {
            if (!inserted[i]) {
                int pos = binarySearchVector(mainSequence, pendingElements[i], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[i]);
            }
        }
    }
    
    if (hasAlone) {
        int pos = binarySearchVector(mainSequence, alone, mainSequence.size());
        mainSequence.insert(mainSequence.begin() + pos, alone);
    }
    
    vec = mainSequence;
}

/* ======================== FORD-JOHNSON POUR DEQUE ======================== */

/**
 * Même logique que pour vector mais avec deque
 * Les deques permettent des insertions efficaces au début et à la fin
 */
std::deque<int> PmergeMe::createInsertionOrderDeque(int size) {
    std::deque<int> insertionOrder;
    if (size >= 1)
        insertionOrder.push_back(1);
    
    if (size >= 3) 
        insertionOrder.push_back(3);
    
    int prev2 = 1, prev1 = 3;
    while (true) {
        int next = prev1 + 2 * prev2;
        if (next > size) break;
        insertionOrder.push_back(next);
        prev2 = prev1;
        prev1 = next;
    }
    return insertionOrder;
}

/**
 * Recherche binaire adaptée pour deque
 */
int PmergeMe::binarySearchDeque(const std::deque<int>& deq, int value, int end) {
    int left = 0, right = end;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (deq[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

/**
 * ALGORITHME FORD-JOHNSON POUR DEQUE
 * 
 * Implémentation identique à celle du vector mais utilisant les spécificités
 * du deque (insertion efficace en début avec push_front)
 */
void PmergeMe::fordJohnsonSortDeque(std::deque<int>& deq) {
    int size = deq.size();
    if (size <= 1) return;
    
    // Même logique que pour vector...
    std::deque<std::pair<int, int> > pairs;
    bool hasAlone = (size % 2 == 1);
    int alone = hasAlone ? deq[size - 1] : 0;
    
    for (int i = 0; i < size - (hasAlone ? 1 : 0); i += 2) {
        if (deq[i] > deq[i + 1])
            pairs.push_back(std::make_pair(deq[i], deq[i + 1]));
        else
            pairs.push_back(std::make_pair(deq[i + 1], deq[i]));
    }
    
    std::deque<int> maxElements;
    for (size_t i = 0; i < pairs.size(); ++i)
        maxElements.push_back(pairs[i].first);
    
    if (maxElements.size() > 1)
        fordJohnsonSortDeque(maxElements);
    
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
    
    std::deque<int> mainSequence;
    std::deque<int> pendingElements;
    
    for (size_t i = 0; i < sortedPairs.size(); ++i) {
        mainSequence.push_back(sortedPairs[i].first);
        pendingElements.push_back(sortedPairs[i].second);
    }
    
    if (!pendingElements.empty()) {
        mainSequence.insert(mainSequence.begin(), pendingElements[0]);
        
        std::deque<int> insertionOrder = createInsertionOrderDeque(pendingElements.size());
        std::deque<bool> inserted(pendingElements.size(), false);
        inserted[0] = true;
        
        for (size_t i = 0; i < insertionOrder.size(); ++i) {
            int idx = insertionOrder[i] - 1;
            if (!inserted[idx]) {
                int pos = binarySearchDeque(mainSequence, pendingElements[idx], mainSequence.size());
                mainSequence.insert(mainSequence.begin() + pos, pendingElements[idx]);
                inserted[idx] = true;
            }
            
            if (i > 0) {
                int prevIdx = insertionOrder[i - 1] - 1;
                for (int j = idx - 1; j > prevIdx; --j) {
                    if (j >= 0 && !inserted[j]) {
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
    
    if (hasAlone) {
        int pos = binarySearchDeque(mainSequence, alone, mainSequence.size());
        mainSequence.insert(mainSequence.begin() + pos, alone);
    }
    
    deq = mainSequence;
}