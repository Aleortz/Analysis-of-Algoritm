"""
Binary Search Implementation
Analysis of Algorithms Course - Yachay Tech University

Algorithm: Binary Search
Category: Searching
Time Complexity: O(log n)
Space Complexity: O(1) for iterative, O(log n) for recursive
"""

def binary_search_iterative(arr, target):
    """
    Performs binary search on a sorted array (iterative approach).
    
    Args:
        arr: A sorted list of comparable elements
        target: The element to search for
        
    Returns:
        Index of target if found, -1 otherwise
        
    Time Complexity: O(log n)
    Space Complexity: O(1)
    """
    left, right = 0, len(arr) - 1
    
    while left <= right:
        mid = left + (right - left) // 2  # Avoids potential overflow
        
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    
    return -1


def binary_search_recursive(arr, target, left=0, right=None):
    """
    Performs binary search on a sorted array (recursive approach).
    
    Args:
        arr: A sorted list of comparable elements
        target: The element to search for
        left: Left boundary of search space
        right: Right boundary of search space
        
    Returns:
        Index of target if found, -1 otherwise
        
    Time Complexity: O(log n)
    Space Complexity: O(log n) due to recursive call stack
    """
    if right is None:
        right = len(arr) - 1
    
    if left > right:
        return -1
    
    mid = left + (right - left) // 2
    
    if arr[mid] == target:
        return mid
    elif arr[mid] < target:
        return binary_search_recursive(arr, target, mid + 1, right)
    else:
        return binary_search_recursive(arr, target, left, mid - 1)


# Example usage and test cases
if __name__ == "__main__":
    # Test array
    test_array = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
    
    # Test cases
    test_cases = [
        (5, 2),      # Element in the middle
        (1, 0),      # Element at the start
        (19, 9),     # Element at the end
        (10, -1),    # Element not in array
        (20, -1),    # Element larger than all
        (0, -1),     # Element smaller than all
    ]
    
    print("Binary Search - Iterative Approach")
    print("=" * 50)
    for target, expected in test_cases:
        result = binary_search_iterative(test_array, target)
        status = "✓" if result == expected else "✗"
        print(f"{status} Searching for {target:2d}: Found at index {result:2d} (Expected: {expected:2d})")
    
    print("\nBinary Search - Recursive Approach")
    print("=" * 50)
    for target, expected in test_cases:
        result = binary_search_recursive(test_array, target)
        status = "✓" if result == expected else "✗"
        print(f"{status} Searching for {target:2d}: Found at index {result:2d} (Expected: {expected:2d})")
    
    print("\n" + "=" * 50)
    print("Complexity Analysis:")
    print("- Time Complexity: O(log n)")
    print("- Space Complexity: O(1) iterative, O(log n) recursive")
    print("- Best Case: O(1) - element at middle")
    print("- Worst Case: O(log n) - element not found or at end")
