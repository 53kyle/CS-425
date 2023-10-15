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
#include <mutex>
#include <barrier>
#include <execution>

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
    std::mutex m;
    std::barrier b{MaxThreads};

    // Iterate across all available data values, processing them using the
    //   reverse-digits and sum technique described in class.
    
    const size_t chunkSize = int(data.size()/MaxThreads + 1);
    
    for (int id = 0; id < MaxThreads; id++) {
        std::thread t{[&,id]() {
            std::vector<Number> numbers;
            
            if (data.getNext(chunkSize,numbers)) {
                for (int i = 0; i < numbers.size(); i++) {
                    size_t iter = 0;
                    Number n = numbers[i];
                    
                    // The Lychrel loop - for any iteration, take the number, reverse
                    //   its digits, and sum those values together.  If that sum
                    //   is a palindrome, stop processing
                    while (!n.is_palindrome() && ++iter < MaxIterations) {
                        Number sum(n.size());   // Value used to store current sum of digits

                        // An iterator pointing to the first digit of the reversed
                        //   value.  This iterator will be incremented to basically
                        //   traverse the digits of the main number in reverse
                        auto rd = n.begin();
                        
                        bool carry = false;  // flag to indicate if we had a carry

                        // Sum the digits using the "transform" algorithm.  This
                        //   algorithm traverses a range of values (in our case,
                        //   starting with the least-siginificant [i.e., right most]
                        //   digit) of the original number, adding each digit to its
                        //   matching digit (by position) in the reversed number.
                        //
                        // The result is stored in the sum variable, which is
                        //   built up one digit at a time, respecting if a carry
                        //   digit was necessary for any iteration.
                        std::transform(n.rbegin(), n.rend(), sum.rbegin(),
                            [&](auto d) {
                                auto v = d + *rd++ + carry;

                                carry = v > 9;
                                if (carry) { v -= 10; }

                                return v;
                            }
                        );

                        // If there's a final carry value, prepend that to the sum
                        if (carry) { sum.push_front(1); }

                        // Transfer the sum making it the next number to be processed
                        //   (i.e., reversed, summed, and checked if it's a
                        //   palindrome)
                        
                        n = sum;
                    }
                    
                    std::lock_guard lock{m};
                    if (iter < MaxIterations && iter >= maxIter) {
                        Record record{numbers[i], n};
                        
                        if (iter > maxIter) {
                            records.clear();
                            maxIter = iter;
                        }
                        
                        records.push_back(record);
                    }
                }
            }
            b.arrive_and_wait();
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
