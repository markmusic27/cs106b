# Search and Sorting pt. 1

Type: Note
Description: Notes on lecture from 02/03/2025
Last Edited: February 24, 2025 6:47 PM

# Search

## Linear Search

Go through the entire array to see if an element is in a list.

- Time Complexity: $O(n)$
- It’s fine for small n but quickly breaks down for large n (customer records, etc…)

If there is no ordering, then you do have to go through the entire array to find a record. But what if the data was ordered?

## Binary Search

This strategy works on ordered arrays. Look at the binary search strategy in pseudocode:

```bash
We're looking for 54 in a vector with entires 1 to 100.
1. Go to middle. Is middle bigger than 54? Then discard bottom half of array.
	     else: Discard top half of array.
2. Repeat until you find value (base case if entry == 54)
```

As you can see this uses recursion.

- Time Complexity: $O(log_2(n))$

We don’t use `.sublist()`  because this function copies the list into a new list.

### Derivation for Time Complexity

$$
\frac{n}{2^r}=\text{number of elements left}
$$

$$
\therefore \text{find when }\frac{n}{2^r}=1
$$

$$
n=2^r \therefore r=log_2(n)
$$

# Sort

## Selection Sort

Looks for elements in the array and finds the smallest, then swaps it with what was in its position.

![CleanShot 2025-02-19 at 13.15.10@2x.png](Search%20and%20Sorting%20pt%201%2019fb37deb65d807e898ffb125d129cc7/CleanShot_2025-02-19_at_13.15.102x.png)

- It is common to see arrays past by reference since it’s faster to rearrange the elements into a new order than to copy all of them into a new array.

<aside>
‼️

**How fast is it?**

- Finding the element that goes in position zero from **n** elements
- Finding the element that goes in position zero from **n-1** elements
- Repeat until all elements are scanned.

TOTAL: $n + (n-1)+(n-2)+\cdots +2+1$

Same as stacking blocks so really this rewrites to $\frac{n^2}{2}$

**Time complexity: $O(n^2)$**

</aside>