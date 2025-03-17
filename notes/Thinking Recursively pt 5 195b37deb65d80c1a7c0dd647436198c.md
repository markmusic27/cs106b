# Thinking Recursively pt. 5

Type: Note
Description: Lecture notes on Video from class on 01/27/25
Last Edited: February 19, 2025 10:33 AM

## Unconditional Return Statements

Careful with return statements in for loops. These end the function so you have to ensure that you don’t interrupt the loop with an unwanted return statement.

<aside>
‼️

**Tip:**

Guard return statements with an if block if it’s in a for loop.

</aside>

## What is Recursive Backtracking

The idea of backtracking is that in a decision tree, your program is backtracking to the decision above (i.e., the one it made before) based on the next decision. Example of is word shrinkable.

```bash
Is "cart" shrinkable?
--> is "art" shrinkable?
-- --> is "rt" shrinkable? No, not an English word, go back to "art"
-- --> is "at" shrinkable?
-- -- --> is "t" shrinkable? No, not an Englis word, go back to "at"
-- -- --> is "a" shrinkable? Yes, so "at" -> "art" -> "cart" is shrinkable
```

The process of going back to the previous word if the current word is not shrinkable is an example of backtracking. This is enabled often by having the recursive function return something.

<aside>
‼️

**Tip:**

You can draw out these decision trees to debug a recursive function.

</aside>

## Optional Type

You can wrap an optional type over any other type.

```cpp
Optional<String> name; // Either "Mark" or Nothing
```

It’s like nullable values in other languages.

```cpp
if (name != Nothing){
		cout << name << endl;
}
```

Good way to test if something exists. If there is nothing, then you can literally return it.

## Auto Type

You can let the compiler figure out the type of the variable for you with the type auto

```cpp
auto name = getName(); // This returns a string so name is type name
```

Normally used when you have a VERY long type and you don’t want to write it all so you can just say `auto` (like `Optional<Vector<Vector<String>>>`).