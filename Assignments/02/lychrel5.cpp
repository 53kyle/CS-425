/////////////////////////////////////////////////////////////////////////////
//
//  lychrel.cpp
//
//  A program that searches for the largest (in terms of number of
//    iterations without exceeding a specified maximum) palindrome value.
//
//  The program reports a list of numbers that share the maximum number of
//    iterations, along with the size and final palindrome number
//

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <thread>
#include <array>

#include "LychrelData.h"

// A structure recording the starting number, and its final palindrome.
//   An vector of these records (typedefed to "Records") is built during
//   program execution, and output before the program terminates.
struct Record {
    Number n;
    Number palindrome;
};
using Records = std::vector<Record>;

// Application specific constants
const size_t MaxIterations = 7500;
const size_t MaxThreads = 10;

//
// --- main ---
//
int main() {
    LychrelData data;

    std::cerr << "Processing " << data.size() << " values ...\n";

    std::atomic<size_t> maxIter = 0;  // Records the current maximum number of iterations
    Records records; // list of values that took maxIter iterations

    // Iterate across all available data values, processing them using the 
    //   reverse-digits and sum technique described in class.
    
    const size_t chunkSize = int(data.size()/MaxThreads + 1);
    
    for (int id = 0; id < MaxThreads; id++) {
        std::thread t{[=,&maxIter,&data,&records]() {
            std::vector<Number> numbers;
            
            if (data.getNext(chunkSize,numbers)) {
                const size_t secondaryChunkSize = int(numbers.size()/8);
                int i = 0;
                for (int j = 0; j < secondaryChunkSize; j++) {
                    size_t iter1 = 0;
                    size_t iter2 = 0;
                    size_t iter3 = 0;
                    size_t iter4 = 0;
                    size_t iter5 = 0;
                    size_t iter6 = 0;
                    size_t iter7 = 0;
                    size_t iter8 = 0;
                    Number n1 = numbers[i];
                    Number n2 = numbers[i+1];
                    Number n3 = numbers[i+2];
                    Number n4 = numbers[i+3];
                    Number n5 = numbers[i+4];
                    Number n6 = numbers[i+5];
                    Number n7 = numbers[i+6];
                    Number n8 = numbers[i+7];

                    while (!n1.is_palindrome() && ++iter1 < MaxIterations) {
                        Number sum(n1.size());   // Value used to store current sum of digits
                        Number r = n1.reverse(); // reverse the digits of the value

                        auto rd = n1.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n1.rbegin(), n1.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n1 = sum;
                    }
                    
                    while (!n2.is_palindrome() && ++iter2 < MaxIterations) {
                        Number sum(n2.size());   // Value used to store current sum of digits
                        Number r = n2.reverse(); // reverse the digits of the value

                        auto rd = n2.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n2.rbegin(), n2.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n2 = sum;
                    }
                    
                    while (!n3.is_palindrome() && ++iter3 < MaxIterations) {
                        Number sum(n3.size());   // Value used to store current sum of digits
                        Number r = n3.reverse(); // reverse the digits of the value

                        auto rd = n3.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n3.rbegin(), n3.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n3 = sum;
                    }
                    
                    while (!n4.is_palindrome() && ++iter4 < MaxIterations) {
                        Number sum(n4.size());   // Value used to store current sum of digits
                        Number r = n4.reverse(); // reverse the digits of the value

                        auto rd = n4.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n4.rbegin(), n4.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n4 = sum;
                    }
                    
                    while (!n5.is_palindrome() && ++iter5 < MaxIterations) {
                        Number sum(n5.size());   // Value used to store current sum of digits
                        Number r = n5.reverse(); // reverse the digits of the value

                        auto rd = n5.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n5.rbegin(), n5.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n5 = sum;
                    }
                    
                    while (!n6.is_palindrome() && ++iter6 < MaxIterations) {
                        Number sum(n6.size());   // Value used to store current sum of digits
                        Number r = n6.reverse(); // reverse the digits of the value

                        auto rd = n6.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n6.rbegin(), n6.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n6 = sum;
                    }
                    
                    while (!n7.is_palindrome() && ++iter7 < MaxIterations) {
                        Number sum(n7.size());   // Value used to store current sum of digits
                        Number r = n7.reverse(); // reverse the digits of the value

                        auto rd = n7.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n7.rbegin(), n7.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n7 = sum;
                    }

                    while (!n8.is_palindrome() && ++iter8 < MaxIterations) {
                        Number sum(n8.size());   // Value used to store current sum of digits
                        Number r = n8.reverse(); // reverse the digits of the value

                        auto rd = n8.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        std::transform(n8.rbegin(), n8.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;
                
                                carry = v > 9;
                                if (carry) { v -= 10; }
                
                                return v;
                            }
                        );

                        if (carry) { sum.push_front(1); }

                        n8 = sum;
                    }
                    
                    if (iter1 >= maxIter && iter1 < MaxIterations) {
                        Record record{numbers[i], n1};
                        
                        if (iter1 > maxIter) {
                            records.clear();
                            maxIter = iter1;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter2 >= maxIter && iter2 < MaxIterations) {
                        Record record{numbers[i+1], n2};
                        
                        if (iter2 > maxIter) {
                            records.clear();
                            maxIter = iter2;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter3 >= maxIter && iter3 < MaxIterations) {
                        Record record{numbers[i+2], n3};
                        
                        if (iter3 > maxIter) {
                            records.clear();
                            maxIter = iter3;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter4 >= maxIter && iter4 < MaxIterations) {
                        Record record{numbers[i+3], n4};
                        
                        if (iter4 > maxIter) {
                            records.clear();
                            maxIter = iter4;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter5 >= maxIter && iter5 < MaxIterations) {
                        Record record{numbers[i+4], n5};
                        
                        if (iter5 > maxIter) {
                            records.clear();
                            maxIter = iter5;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter6 >= maxIter && iter6 < MaxIterations) {
                        Record record{numbers[i+5], n6};
                        
                        if (iter6 > maxIter) {
                            records.clear();
                            maxIter = iter6;
                        }

                        records.push_back(record);
                    }
                    
                    if (iter7 >= maxIter && iter7 < MaxIterations) {
                        Record record{numbers[i+6], n7};
                        
                        if (iter7 > maxIter) {
                            records.clear();
                            maxIter = iter7;
                        }

                        records.push_back(record);
                    }
                    
                    i += 8;
                }
            }
        }};
        
        (id < MaxThreads-1) ? t.detach() : t.join();
    }

    // Output our final results
    
    std::cout << "\nmaximum number of iterations = " << maxIter << "\n";
    for (auto& [number, palindrome] : records) {
        std::cout 
            << "\t" << number 
            << " : [" << palindrome.size() << "] "
            << palindrome << "\n";
    }
}
