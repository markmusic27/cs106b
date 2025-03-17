# Designing Abstractions

Type: Note
Description: Notes on lectures form class on 02/07/2025
Last Edited: February 24, 2025 6:47 PM

## What are Abstractions?

Things like maps, grids, sets, and queues. Broadly, a rich vocabulary of abstractions makes it possible to model and solve a wider class of problems.

- This will result in new types we can use to construct new algorithms

To achieve this, we will need classes.

## Classes

- Vector, Stack, Queue, Map, etc… are **classes** in C++

### Classes contain:

1. An Interface: specifies what operations can be performed on instances of the class.
2. An Implementation: specifying how those operations are performed.

### Defining classes

This usually takes two steps:

1. Create a header file (ends in `.h`) specifying the interface and a few implementation details
2. Create an implementation file (ends with `.cpp`) containing the class implementation

Clients of the class can then include the header file to use the class.

### Example Header: RandomBag.h

```cpp
#pragma once
#include "vector.h"

class RandomBag {
public: // Functions you can call on an object of this type
	void add(int value);
	int  removeRandom();
	
	int size() const;

private: // The class type will need to do their job (invisible to client)
	Vector<int> elems;
	

}; // NEED THE SEMICOLON
```

### Example Implementation File: RandomBag.cpp

```cpp
#include "RandomBag.h"
#include "random.h"
#include "error.h"

void RandomBag::add(int value){
	elems += value;
}

int RandomBag::removeRandom() {
	if (elems.isEmpty()){
		error("Can't remove an item from a random bag!");
	}

	int indexToRemove = randomInteger(0, elems.size()-1);
	int res = elems[indexToRemove];
	
	elems.remove(indexToRemove);
	return res;
}

int RandomBag::size() const {
	return elems.size();
}
```

- By calling `RandomBag::`, you are telling the compiler that you will be implementing a function in a class. the `::` is called the Scope Resolution operator.
- You can use the variables in the private section
- You have to explicitly state what functions do not update the value of the state within the class as done for the `size()` elements in the RandomBag class.