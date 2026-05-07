# Asymptotic Notation

Analysis of Algorithms Course - Yachay Tech University

## Introduction

Asymptotic notation is a mathematical notation used to describe the running time or space requirements of an algorithm as the input size grows toward infinity.

## Big O Notation (O)

Big O notation describes the **upper bound** of an algorithm's complexity.

**Definition**: f(n) = O(g(n)) if there exist positive constants c and n₀ such that:
```
0 ≤ f(n) ≤ c·g(n) for all n ≥ n₀
```

### Common Time Complexities

1. **O(1)** - Constant Time
   - Example: Array access, hash table lookup
   - Time doesn't depend on input size

2. **O(log n)** - Logarithmic Time
   - Example: Binary search
   - Divides problem in half each step

3. **O(n)** - Linear Time
   - Example: Linear search, array traversal
   - Processes each element once

4. **O(n log n)** - Linearithmic Time
   - Example: Merge sort, quick sort (average)
   - Efficient sorting algorithms

5. **O(n²)** - Quadratic Time
   - Example: Bubble sort, selection sort
   - Nested loops over input

6. **O(2ⁿ)** - Exponential Time
   - Example: Recursive Fibonacci (naive)
   - Extremely slow for large inputs

7. **O(n!)** - Factorial Time
   - Example: Generating all permutations
   - Intractable for even moderate inputs

## Omega Notation (Ω)

Omega notation describes the **lower bound** of an algorithm's complexity.

**Definition**: f(n) = Ω(g(n)) if there exist positive constants c and n₀ such that:
```
0 ≤ c·g(n) ≤ f(n) for all n ≥ n₀
```

## Theta Notation (Θ)

Theta notation describes the **tight bound** of an algorithm's complexity.

**Definition**: f(n) = Θ(g(n)) if:
```
f(n) = O(g(n)) AND f(n) = Ω(g(n))
```

## Properties

### Transitivity
- If f(n) = O(g(n)) and g(n) = O(h(n)), then f(n) = O(h(n))

### Reflexivity
- f(n) = O(f(n))

### Symmetry (for Theta)
- f(n) = Θ(g(n)) if and only if g(n) = Θ(f(n))

## Examples

### Example 1: Linear Search
```python
def linear_search(arr, target):
    for i in range(len(arr)):
        if arr[i] == target:
            return i
    return -1
```
- **Best Case**: O(1) - element at first position
- **Worst Case**: O(n) - element at last position or not found
- **Average Case**: O(n)

### Example 2: Bubble Sort
```python
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]
```
- **Time Complexity**: O(n²)
- **Space Complexity**: O(1)

## Growth Rate Comparison

From fastest to slowest:
```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2ⁿ) < O(n!)
```

## Practice Problems

1. Determine the time complexity of the following:
   ```python
   for i in range(n):
       for j in range(i, n):
           print(i, j)
   ```

2. What is the space complexity of merge sort?

3. Why is O(n log n) better than O(n²) for large n?

## Key Takeaways

- Big O describes worst-case behavior
- Focus on dominant terms as n → ∞
- Constants are dropped in asymptotic analysis
- Choose algorithms with better time complexity for large inputs
- Sometimes space complexity matters as much as time complexity
