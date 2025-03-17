# Thinking Recursively pt. 2

Type: Note
Description: Notes on Lecture from 1/22/2025
Last Edited: February 6, 2025 4:03 PM

## Sets

Unordered set of unique elements. We only care about whether something exists or not.

- **Advantage over vector:** you can determine whether an element exists instantly.

```cpp
set<char> s1 = {'c', 'd'};
```

- If you try to add an item that already exists, it won’t be added because it already exists.
- If you add something to a set, it adds that thing as an item

```cpp
set<char> s1 = {'c', 'd'};

s1+='a'; // now is {'c', 'd', 'a'}
// or s1 = s1+'a'
```

- You can remove an item from a set with the minus

```cpp
set<char> s1 = {'c', 'd'};

s1-='d'; // now is {'c'}
// or s1 = s1-'d'
```

## How to Write Recursive Function

- Has two parts
    - Base case (normally when n==0)
    - Recursive Case (when function calls itself like n-1)

```cpp
string reverseOf(const string& input) {
		// Base Case
    if (input == "") {
        
    }
    
    // Recursive Case
    else {
        
    }
}
```

## The Recursive Leap of Faith

The idea is that when you’re writing a recursive function, you can trust that the function that you call within itself works. You have to trust that even though you haven’t finished the recursive function, you have to trust that it already works.

## Decision Trees

Often with recursion, you need the function to make a decision and then call itself based on it.

![CleanShot 2025-02-06 at 15.59.04.png](Thinking%20Recursively%20pt%202%20192b37deb65d80e59347f98702eff224/CleanShot_2025-02-06_at_15.59.04.png)

This builds a decision tree.