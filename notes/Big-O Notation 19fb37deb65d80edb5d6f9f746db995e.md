# Big-O Notation

Type: Note
Description: Notes on lecture from 01/31/2025
Last Edited: February 19, 2025 12:19 PM

## Big-O Notation

Way to quantify the rate at which something grows or shrinks.

*Does something scale linearly, quadratically, exponentially, logarithmically, etc…*

### Example

- A square’s and a circle’s area scales at the same rate when you increase their radius              so `O($r^2$)`
- A cube’s and sphere’s volume increases at the same rate as `O($r^3$)`

### Example: Metcalfe’s Law

The value of a communications network with $n$ users is $O(n^2)$

Imagine a social network with 10mil ppl with value of $10mil. → You would need an extra 90mil ppl to have it be valued at $1billion bc 10x input results in 100x output by Metcalfe’s law.

## Salient Features

- Remove all leading coefficients. This is because as something scales, you don’t quite need them. Take things to infinity.
- Remove any non-dominant terms (*Math 21*)

**Note: These are all estimates.**

## Application in Code

Used to determine how fast algorithms are. Computers have different processors and operating systems so determining exactly how fast an algorithm is can be difficult. Hence, you can imagine that **any individual statement takes one unit of time to execute.** How many units of time will the program take?

- If there is one for loop and is not recursive, then it’s **linear ($O(n)$)!**
- If it has two for loop with n elements, then its **quadratic ($O(n^2)$)!**

<aside>
‼️

**Tip:**

Work inside out from the most-nested for loop.

</aside>

What’s cool is you can just look at code and estimate how long it will take to run! You can also determine if a piece of code is even a good idea to run. (eg. exponential runtime on Facebook database will never end).