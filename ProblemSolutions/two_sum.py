"""
Problem: Two Sum
Source: Algorithmic Problem Practice
Category: Array, Hash Table
Difficulty: Easy

Problem Statement:
-----------------
Given an array of integers nums and an integer target, return indices of the 
two numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may 
not use the same element twice.

Example 1:
Input: nums = [2, 7, 11, 15], target = 9
Output: [0, 1]
Explanation: nums[0] + nums[1] = 2 + 7 = 9

Example 2:
Input: nums = [3, 2, 4], target = 6
Output: [1, 2]

Example 3:
Input: nums = [3, 3], target = 6
Output: [0, 1]

Constraints:
- 2 <= nums.length <= 10^4
- -10^9 <= nums[i] <= 10^9
- -10^9 <= target <= 10^9
- Only one valid answer exists
"""


def two_sum_brute_force(nums, target):
    """
    Brute force approach: Check all possible pairs.
    
    Time Complexity: O(n²)
    Space Complexity: O(1)
    
    Approach:
    - Use nested loops to check every pair
    - Return indices when sum equals target
    """
    n = len(nums)
    for i in range(n):
        for j in range(i + 1, n):
            if nums[i] + nums[j] == target:
                return [i, j]
    return []


def two_sum_hash_table(nums, target):
    """
    Optimized approach using hash table.
    
    Time Complexity: O(n)
    Space Complexity: O(n)
    
    Approach:
    - Use a hash table to store seen numbers and their indices
    - For each number, check if (target - number) exists in hash table
    - If found, return the pair of indices
    - Otherwise, add current number to hash table
    """
    seen = {}  # Dictionary to store {value: index}
    
    for i, num in enumerate(nums):
        complement = target - num
        
        if complement in seen:
            return [seen[complement], i]
        
        seen[num] = i
    
    return []


# Test cases
if __name__ == "__main__":
    test_cases = [
        ([2, 7, 11, 15], 9, [0, 1]),
        ([3, 2, 4], 6, [1, 2]),
        ([3, 3], 6, [0, 1]),
        ([1, 5, 3, 7, 9], 12, [2, 4]),
        ([-1, -2, -3, -4, -5], -8, [2, 4]),
    ]
    
    print("Two Sum Problem - Solutions")
    print("=" * 70)
    
    print("\nBrute Force Approach (O(n²)):")
    print("-" * 70)
    for nums, target, expected in test_cases:
        result = two_sum_brute_force(nums, target)
        status = "✓" if result == expected else "✗"
        print(f"{status} nums={nums}, target={target}")
        print(f"   Result: {result}, Expected: {expected}")
    
    print("\nHash Table Approach (O(n)):")
    print("-" * 70)
    for nums, target, expected in test_cases:
        result = two_sum_hash_table(nums, target)
        status = "✓" if result == expected else "✗"
        print(f"{status} nums={nums}, target={target}")
        print(f"   Result: {result}, Expected: {expected}")
    
    print("\n" + "=" * 70)
    print("Complexity Analysis:")
    print("-" * 70)
    print("Brute Force:")
    print("  - Time Complexity: O(n²) - nested loops")
    print("  - Space Complexity: O(1) - no extra space")
    print("\nHash Table:")
    print("  - Time Complexity: O(n) - single pass")
    print("  - Space Complexity: O(n) - hash table storage")
    print("\nKey Insight:")
    print("  Trading space for time - hash table approach is much faster")
    print("  for large inputs at the cost of extra memory.")
