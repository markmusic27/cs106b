# Implementing Abstractions

Type: Note
Description: Notes on lecture from 02/12/2025
Last Edited: March 2, 2025 2:49 PM

## Dynamic Memory Allocation

It’s a technique to get space where it can put elements. How this happens is the next major topic. These are the building blocks that Vectors, Maps, etc. are built upon.

## The Basics

The code may seem very different than what we’re used to.

```cpp
string* ptr;
ptr = new string[3]
```

The variable `ptr` is called a pointer. This points towards a certain value. In this way, helps us find where it is stored. **Remember that the pointer is not the value.** Here, the pointer points to an array of string. But it is a dance of two things. The star indicates its a pointer

```cpp
ptr[0] = "Coffee Shop"
ptr[1] = "Office Space"
ptr[2] = "Residential"
```

Think of the pointer as directing traffic. It tells things where to be and where to go. It’s just pointing at something.

This does not work like a vector where if you need to put more entries into the array, it will just take them. If you ask for three strings, you will only get space for three strings.

Think of the building analogy.

```cpp
int* ptr = new int[4];
cout << ptr[0] << endl;
```

When you initialize but don’t assign integers in C++ they just requisition the space but don’t reset it to zero. So by default they can be whatever.

<aside>

**Initialize all of your pointers!!!**

</aside>

If you try to write something into memory that you have not been allocated, it may work! There is no nice error to tell you that you did something wrong. You should also check if you’re reading a part of the array that is yours!

```cpp
string* ptr2 = ptr;
```

This will create a new pointer that points at the same memory!! Will not copy the memory itself.

```cpp
ptr2[0] = "Mark's House"
cout << ptr[0] << endl;
```

This will print “Mark’s House” since its the same memory!

Uninitialized pointers, often called Garbage Pointers, point somewhere, but there’s no way to predict exactly where! You need to initialize your pointer!

### Why does the language not check that all pointers are initialized?

It’s faster to trust that the developer knows what they’re doing and not check. This is one of the reasons why C++ is so performant!

## Cleaning Up

Normally, when a function is called, all the local variables cleaned up after the function is returned. With dynamic allocation, the program trusts you to clean up the variables you create with pointers.

- When using `new[]` , you are responsible for deallocating the memory you allocate. If you don’t, you get a memory leak. Your program will never be able to use that memory again.

To clean up, you…

```cpp
delete[] ptr;
```

Once you delete the object assigned to the pointer, it becomes uninitialized once again. This is called a dangling pointer.

<aside>
⌨️

**To summarize:**

1. Pointers point. There are two partners in the dance.
2. You can create arrays of a fixed size at runtime by using `new[]` 
3. C++ arrays don’t know their lengths and have no bounds-checking. With great power comes great responsibility
4. You are responsible for freeing any memory you explicitly allocate by calling `delete[]` 
5. Once you’ve deleted the memory pointed at by a pointer, you have a dangling pointer and shouldn’t read or write from it
</aside>

## Constructors

- A constructor is a special member function used to set up the class before it is used.
- The constructor is automatically called when the object is created.
- The constructor for a class named `ClassName` has signature `ClassName(args)`.