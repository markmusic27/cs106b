# Linked Lists pt. 2

Type: Note
Description: Notes on lecture from 02/26/2025
Last Edited: February 26, 2025 2:30 PM

### Prepending an Element

*Prepend means adding in front*

1. Create a new cell
2. Set its next to the start of the list
3. Make that cell the start of the list

```cpp
void prependTo(Cell*& list, const string& value) {
	Cell* cell = new Cell;
	
	cell->value = value;
	cell->next = list;
	
	list = cell;
}
```

Note the ampersand (`&`) so the pointer is passed by reference.

### Pointers by Reference

If you are changing the value of the pointer passed in as a parameter, pass it by reference. If it’s just telling you where to look, then don’t pass it by reference.

### Append an Element (`SLOW`)

*Prepend means adding to the end*

1. Find the last cell in the linked list
2. Create a new cell
3. Set next of the last cell to point at the new cell

```cpp
void appendTo(Cell*& list, const string& value) {
	Cell* cell = new Cell;
	cell->value = value;
	cell->next = nullptr;
	
	if (list == nullptr) {
		list = cell;
		return;
	}
	
	Cell* end = list;
	while (end->next != nullptr){
		end = end->next;
	}
	
	end->next = cell;
}
```

<aside>
‼️

**TIP:**

The debugger calls `0x0` as a nullptr. If you find it in a debugger, its often because you’re trying to access a nullptr cell which has no next or value.

</aside>

Bruh this shit ain’t efficient… the code above has time complexity of $O(n)$.

### Append an Element (`FAST`)

The way you make this faster is by adding a tail pointer that keeps track of the end of the pointer. When you create the linked list, keep track of both

```cpp
void appendTo(Cell*& head, Cell*& tail, const string& value) {
	Cell* cell = new Cell;
	cell->value = value;
	cell->next = nullptr;
	
	if (head = nullptr){
		tail = head = cell;
		return;
	}
	
	tail->next = cell;
	tail = cell;
}
```

This runs in constant time!

## Doubly-Linked List

With normal linked lists, its easy to go to the next cell but difficult to go to the previous test