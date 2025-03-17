# Implementing Abstractions pt. 2

Type: Note
Description: Notes from lecture on 02/14/2025
Last Edited: February 24, 2025 6:45 PM

## Implementing a Stack

OurStack.h file

```cpp
class OurStack {
public:
	OurStack();
	
	void push(int value);
	int peek() const;
	int pop();
	
	int size() const;
	int isEmpty() const;

private:
	int* elems;
	int allocatedSize;
	int logicalSize;
};
```

OurStack.cpp

```cpp
// logicalSize is the space that is taken and allocatedSize is total space
OurStack::OurStack() {
	logicalSize = 0;
	allocatedSize = 4;
	
	elems = new int[allocatedSize];
}

OurStack::push(int value){
	if (allocatedSize == logicalSize){
		error("Not enough space to push new value.");
	}
	
	elems[logicalSize] = value;
	logicalSize++;
}

OurStack::peek() const {
	if (isEmpty()){
		error("No items in the array!")
	}
	
	return elems[logicalSize - 1]
}

OurStack::pop() {
	// We don't remove the value from the array, we just pretend that it isn't there;
	
	int result = peek();
	logicalSize--;
	
	return result;
}
```

## Destructor

- A destructor is a special member function responsible for cleaning up an object’s memory.
- It’s automatically called whenever an object’s lifetime ends (eg. if it’s a local variable that goes out of scope)
- The destructor for a class named `ClassName` has signature `~ClassName();`

They are made to clean up resources. You do not call them just like you don’t call constructors.

```cpp
// Add this to your header file in the OurStack class
class OurClass {
public:
	...
	~OurClass();
	...
};

// Add this to your implementation file in the OurStack.cpp file

OurStack::~OurStack(){
	// Delete all pointer memory
	delete[] elems;
}
```

<aside>
‼️

**TIP:**
If your program crashes and you don’t know why, run it with the debugger on and it will point to the line that caused the crash!

</aside>

## Making stacks with immutable sizes

What you do in theory is create a new array that is bigger than the older one. This will give you space for more things. This is implemented with the `grow()` function.

```cpp
class OurStack {
	...

private:
...
	void grow();
};
```

```cpp
void OurStack::grow() {
	allocatedSize = /* Bigger */
	int* helper = new[allocatedSize];
	
	/* Move elements into new array from old array */
	for (int i = 0; i < logicalSize; i++){
		helper[i] = elems[i];
	}
	
	delete[] elems;
	elems = helper;
}
```

But how much bigger do we make the new array?

- If we make it 1 slot bigger, the time complexity of one push is $O(n)$ and for $n$ pushes, it would be $O(n^2)$. This assumes that the push occurs when the old array is already full.
- In theory, you can make it bigger by two. This makes it twice as fast, but due to Big-O notation, it will still be the same as adding one.

You can theoretically add a million slots to the new array but this would use too much memory in the computer. How do you compromise and balance these competing forces?

### Balancing the forces

What you do is always allocate double the previous space. So say you have 4, allocate 16, then allocate 32.

```cpp
allocatedSize *= 2;
```

This results in exponential growth. The idea is that if you reached say 4096 slots, you will likely need an extra 4096.

### What is the time complexity?

- Cost for one push $O(1)$
- Cost for $n$ pushes $O(n)$

## Amortized Analysis

We reason about the total work done by allowing ourselves to backcharge work to the previous operations, then look at the “average” amount of work done per operation.

In an amortized sense, our implementation of a stack is extremely fast!

This is one of the most common approaches to implementing `Stack` (and `Vector`, for that matter)