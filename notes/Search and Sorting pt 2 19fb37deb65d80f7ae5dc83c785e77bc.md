# Search and Sorting pt. 2

Type: Note
Description: Notes on lecture from 02/05/2025
Last Edited: February 19, 2025 2:14 PM

## Merge Sort

### Derivation pt. 1: Sorting two smaller arrays is faster than sorting one big array

Note from last lecture that selection sort had time complexity of $O(n^2)$. This means that if you have a unsorted list, you can break it into two and sorting these two new lists will take half as long to sort than sorting the full list. Proof below:

![CleanShot 2025-02-19 at 13.36.50@2x.png](Search%20and%20Sorting%20pt%202%2019fb37deb65d80f7ae5dc83c785e77bc/CleanShot_2025-02-19_at_13.36.502x.png)

Squaring the one-half input results in one-fourth. Hence, the list in the bottom turns to…

![CleanShot 2025-02-19 at 13.37.13@2x.png](Search%20and%20Sorting%20pt%202%2019fb37deb65d80f7ae5dc83c785e77bc/CleanShot_2025-02-19_at_13.37.132x.png)

We can exploit this fact to make a very fast sorting algorithm.

### Derivation pt. 2: Merging two arrays

Start at the beginning of both arrays and start by comparing the first element in each, then place the smallest one into the new array. Then compare the second element in the smaller array with the first, and place the smallest into the new array. Repeat this until both algorithms have been merged.

**Time complexity:** $O(n)$ where $n$ is the number of elements in both arrays.

### Procedure

Split the array all the way until you only have one unit, and then merge the arrays all the way to the top

![CleanShot 2025-02-19 at 14.03.27@2x.png](Search%20and%20Sorting%20pt%202%2019fb37deb65d80f7ae5dc83c785e77bc/CleanShot_2025-02-19_at_14.03.272x.png)

Note that this is a recursive sorting algorithm!

<aside>
⌨️

**Merge Sort Procedure:**

- **Base Case**
    - An empty or single-element list is already sorted
- **Recursive Case**
    - Break the list in half and recursively sort each part
    - Use `merge` to combine them back into a single sorted list
</aside>

![CleanShot 2025-02-19 at 14.06.25@2x.png](Search%20and%20Sorting%20pt%202%2019fb37deb65d80f7ae5dc83c785e77bc/CleanShot_2025-02-19_at_14.06.252x.png)

### Time Complexity

Its $O(n\cdot log(n))$!!!