"""
This module contains functions for Lab Assignment 03 of Lecture 3.
Q1. Practice Operations on a Python list: append(), clear(), copy(),
count(), extend(), index(), insert(), pop(), remove(), reverse(), sort()

Q2. Generate a random list L of length N compare the performace of the following
operations by measuring times with increasing N: N, 2N, 4N, 8N, . . . explain what you observe.
1. Cost of removing elements from lists
 1.1 L.pop()
 1.2 L.pop(0)
2. Slicing vs explicit copy of a list:
 2.1 A = L[:]
 2.2 B = list(L)
3. In-place vs out-of-place modification:
 3.1 L.reverse()
 3.2 R = L[::-1]

Update your git repository by creating a folder of lecture 03, adding files and pushing to GitHub.
Submit both code and screenshots."""


import random
import time

def generate_random_list(N, lower_bound=0, upper_bound=1000):

    """ Generates a random list of Length N with values between lower_bound and upper_bound. 
    Args:
        N (int): Length of the list.
        lower_bound (int): Lower bound for random values.
        upper_bound (int): Upper bound for random values."""
    b = []
    
    for x in range (N):
        b.append(random.randint(lower_bound, upper_bound))
    return b

#==============================================================================
# Q1: Practice Operations on a Python list
#==============================================================================
python_list = generate_random_list(10)
print("Original python_list:", python_list)

# Demonstrating list operations
python_list.append(999)
print("After append(999):", python_list)

python_list_copy = python_list.copy()
print("Copy of python_list:", python_list_copy)

python_list_count = python_list.count(999)
print("Count of 999 in python_list:", python_list_count)

python_list.extend([1001, 1007])
print("After extend([1001, 1007]):", python_list)

index_of_1001 = python_list.index(1001)
print("Index of 1001 in python_list:", index_of_1001)

python_list.insert(3, -1)
print("After insert(3, -1):", python_list)

python_list.insert(len(python_list), "Hello")  
print("After insert (" + str(len(python_list)) + ", 'Hello'):", python_list)

popped_element = python_list.pop()
print("After pop():", python_list, "Popped element:", popped_element)

python_list.remove(1007)
print("After remove(1007):", python_list)

python_list.reverse()
print("After reverse():", python_list)  

python_list.sort()
print("After sort():", python_list)

python_list.clear()
print("After clear():", python_list)

print("\n")
print("====================================================================================================")
print("\n")

#==============================================================================
# Q2: Performance Comparison of List Operations
#==============================================================================

# Generate a Random List L of Length N
N = 1000
N_values = [N * (2 ** i) for i in range(5)]  # N, 2N, 4N, 8N, 16N
print("N values for performance testing:", N_values)
print("------------------------------------------------------------------------------------------------------------\n")

for N in N_values:
    L = generate_random_list(N)

    print(f"Performance comparison for N={N}:")

    #Copy list for different operations
    #List for pop operations
    #List for L.pop() poping from end
    L1_1 = L.copy() 
    #List for L.pop(0) poping from start           
    L1_2 = L.copy()

    #List for slicing and explicit copy
    #List for slicing copy
    L2_1 = L.copy()
    #List for explicit copy
    L2_2 = L.copy()


    #List for in-place and out-of-place modification
    #List for in-place reverse
    L3_1 = L.copy()
    #List for out-of-place reverse
    L3_2 = L.copy()

    # 1. Measuring time for removing elements from lists 
    start_time_pop_from_end = time.time()
    for e in range(len(L1_1)):
        L1_1.pop()
    end_time_pop_from_end = time.time()
    time_pop_from_end = end_time_pop_from_end - start_time_pop_from_end

    start_time_pop_from_start = time.time()
    for e in range(len(L1_2)):
        L1_2.pop(0)
    end_time_pop_from_start = time.time()
    time_pop_from_start = end_time_pop_from_start - start_time_pop_from_start
    
    print(f"Time for L.pop() = {time_pop_from_end:.6f} seconds, Time for L.pop(0) = {time_pop_from_start:.6f} seconds")
    
    # 2. Slicing vs explicit copy of a list
    start_time_slice_copy = time.time()
    A = L2_1[:]
    end_time_slice_copy = time.time()
    time_slice_copy = end_time_slice_copy - start_time_slice_copy

    start_time_explicit_copy = time.time()
    B = list(L2_2)
    end_time_explicit_copy = time.time()
    time_explicit_copy = end_time_explicit_copy - start_time_explicit_copy
    
    print(f"Time for slicing copy = {time_slice_copy:.6f} seconds, Time for explicit copy = {time_explicit_copy:.6f} seconds")
    
    # 3. In-place vs out-of-place modification
    start_time_Inplace_reverse = time.time()
    L3_1.reverse()
    end_time_Inplace_reverse = time.time()
    time_in_place_reverse = end_time_Inplace_reverse - start_time_Inplace_reverse
    
    start_time_out_of_place_reverse = time.time()
    R = L3_2[::-1]
    end_time_out_of_place_reverse = time.time()
    time_out_of_place_reverse = end_time_out_of_place_reverse - start_time_out_of_place_reverse
    
    print(f"Time for in-place reverse = {time_in_place_reverse:.6f} seconds, Time for out-of-place reverse = {time_out_of_place_reverse:.6f} seconds")
    print("------------------------------------------------------------------------------------------------------------")