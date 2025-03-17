# Linked Lists pt. 1

Type: Note
Description: Notes from lecture on 02/24/2025
Last Edited: February 25, 2025 8:30 PM

## Linked Lists at a Glance

- It’s a data structure for storing a sequence of elements
- Each element is stored separately from the rest
- The elements are then chained together into a sequence
- The end of the list is marked with some special indicator

### Difference to arrays

If you want to modify an array, you have to copy the whole thing and then modify it. With linked lists, just changing what one cell points to changes the entire structure

Helps if you know recursion because every linked list is either an empty lust or a single cell that points to another linked lists.

<aside>
🗣

**REMEMBER:**
Every linked list is either an empty list or a single cell that points to another linked lists.

</aside>

### Example

```cpp
struct Cell {
	string value;
	Cell* next;
};
```

Note that the parameter `next` is not literally another cell but just a pointer that points towards another cell!

## Context on Pointers

You’re not making an array of cells and calling that a linked list. You’re creating them as you go like this:

```cpp
Cell* list = new Cell;
```

Using pointers gives us a ton of flexibility on where the cell points to.

```cpp
list->value = "pudu!";

list->next = new Cell;
list->next->value = "quokka!";

list->next->next = new Cell;
list->next->next->value = "dikdik!";
list->next->next->next = nullptr;
```

Use the arrow operator to assign the value. The value is assigned by the struct above!

![CleanShot 2025-02-25 at 19.56.19.png](Linked%20Lists%20pt%201%201a6b37deb65d80868c9ac4d92b052b3d/CleanShot_2025-02-25_at_19.56.19.png)

<aside>
🗣

**REMEMBER:** Every linked list is either…

1. An empty list represented by `nullptr` 
2. A single linked list cell `Cell`  that points at another linked list `list->next`
</aside>

## Finding Length

It’s super easy it do recursively. It’s just $1+(\text{length of the rest of linked list})$ recursively.

```cpp
int lengthOf (Cell* list) {
	// Base case
	if (list == nullptr) return 0;
	
	// Recursive case
	return 1 + lengthOf(list->next);
}
```

or it can be done iteratively…

```cpp
int lengthOf (Cell* list) {
	bool reachedEnd = false;
	int length = 0;
	Cell* current = list;
	
	while (!reachedEnd) {
		if (current == nullptr) {
			reachedEnd = true;
		} else {
			length++;
			current = current->next;
		}
	}
	
	return length;
}
```

## Printing Contents

Can be done recursively…

```cpp
void printContents(Cell* list) {
	// Base case
	if (list = nullptr) return;
	
	// Recursive case
	cout << list->value << endl;
	
	return printContents(list->next);
}
```

or can be done iteratively…

```cpp
void printContents(Cell* list) {
	bool reachedEnd = false;
	Cell* current = list;
	
	while (!reachedEnd) {
		if (current == nullptr) {
			reachedEnd = true;
		} else {
			cout << current->value << endl;
			current = current->next;
		}
	}
}
```

## Cleaning up a Linked List

To delete a allocated memory using the `new`  operator, you use the `delete`  keyword instead of the `delete[]`  keyword you use for `new[]` . You can recursively iterate through a linked list and delete all of the memory.

```cpp
void eraseList(Cell* list) {
	// Base case
	if (list = nullptr) return;
	
	// Recursive case
	delete list;
	
	return eraseList(list->next);
}
```

or you can do it iteratively…

```cpp
void eraseList(Cell* list) {
	while (list != nullptr) {
		Cell* helper = list->next;
		delete list
		list = helper;
	}
}
```