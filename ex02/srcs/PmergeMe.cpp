/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:49:56 by ayarmaya          #+#    #+#             */
/*   Updated: 2025/05/30 22:50:25 by ayarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other) : _vectorData(other._vectorData), _dequeData(other._dequeData)
{
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vectorData = other._vectorData;
        _dequeData = other._dequeData;
    }
    return *this;
}

PmergeMe::~PmergeMe()
{
}

bool PmergeMe::isValidNumber(const std::string& str) const
{
    if (str.empty() || str[0] == '-')
        return false;
    
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    
    // Vérifier que le nombre n'est pas trop grand
    std::stringstream ss(str);
    long long num;
    ss >> num;
    
    if (ss.fail() || !ss.eof() || num > INT_MAX || num < 0)
        return false;
    
    return true;
}

bool PmergeMe::parseInput(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error" << std::endl;
        return false;
    }
    
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (!isValidNumber(arg))
        {
            std::cerr << "Error" << std::endl;
            return false;
        }
        
        int num = std::atoi(arg.c_str());
        _vectorData.push_back(num);
        _dequeData.push_back(num);
    }
    
    return true;
}

void PmergeMe::printContainer(const std::string& prefix, const std::vector<int>& container) const
{
    std::cout << prefix;
    for (size_t i = 0; i < container.size() && i < 5; i++)
    {
        if (i > 0) std::cout << " ";
        std::cout << container[i];
    }
    if (container.size() > 5)
        std::cout << " [...]";
    std::cout << std::endl;
}

double PmergeMe::getTimeInMicroseconds(clock_t start, clock_t end) const
{
    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

// Algorithme Ford-Johnson pour vector
void PmergeMe::insertionSortVector(std::vector<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeInsertVector(std::vector<int>& arr, int left, int right)
{
    if (right - left <= 10) // Seuil pour l'insertion sort
    {
        insertionSortVector(arr, left, right);
        return;
    }
    
    int mid = left + (right - left) / 2;
    mergeInsertVector(arr, left, mid);
    mergeInsertVector(arr, mid + 1, right);
    
    // Merge
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    
    while (j <= right)
        temp[k++] = arr[j++];
    
    for (int i = 0; i < k; i++)
        arr[left + i] = temp[i];
}

void PmergeMe::fordJohnsonSort(std::vector<int>& arr)
{
    if (arr.size() <= 1)
        return;
    
    mergeInsertVector(arr, 0, arr.size() - 1);
}

// Algorithme Ford-Johnson pour deque
void PmergeMe::insertionSortDeque(std::deque<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeInsertDeque(std::deque<int>& arr, int left, int right)
{
    if (right - left <= 10) // Seuil pour l'insertion sort
    {
        insertionSortDeque(arr, left, right);
        return;
    }
    
    int mid = left + (right - left) / 2;
    mergeInsertDeque(arr, left, mid);
    mergeInsertDeque(arr, mid + 1, right);
    
    // Merge
    std::deque<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    
    while (j <= right)
        temp[k++] = arr[j++];
    
    for (int i = 0; i < k; i++)
        arr[left + i] = temp[i];
}

void PmergeMe::fordJohnsonSort(std::deque<int>& arr)
{
    if (arr.size() <= 1)
        return;
    
    mergeInsertDeque(arr, 0, arr.size() - 1);
}

void PmergeMe::sortAndMeasure()
{
    // Afficher la séquence avant tri
    printContainer("Before: ", _vectorData);
    
    // Copier les données pour préserver l'original
    std::vector<int> vectorCopy = _vectorData;
    std::deque<int> dequeCopy = _dequeData;
    
    // Mesurer le temps pour vector
    clock_t startVector = clock();
    fordJohnsonSort(vectorCopy);
    clock_t endVector = clock();
    
    // Mesurer le temps pour deque
    clock_t startDeque = clock();
    fordJohnsonSort(dequeCopy);
    clock_t endDeque = clock();
    
    // Afficher la séquence après tri (utiliser vectorCopy)
    std::vector<int> sortedForDisplay(vectorCopy.begin(), vectorCopy.end());
    printContainer("After:  ", sortedForDisplay);
    
    // Afficher les temps
    double vectorTime = getTimeInMicroseconds(startVector, endVector);
    double dequeTime = getTimeInMicroseconds(startDeque, endDeque);
    
    std::cout << "Time to process a range of " << _vectorData.size() 
              << " elements with std::vector : " << vectorTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _dequeData.size() 
              << " elements with std::deque : " << dequeTime << " us" << std::endl;
}