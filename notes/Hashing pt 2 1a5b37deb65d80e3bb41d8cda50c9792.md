# Hashing pt. 2

Type: Note
Description: Notes from lecture on 02/21/2025
Last Edited: February 26, 2025 6:19 PM

## Making Faster Hash Tables

Hash tables, like the ones we saw last time, are among the most-commonly used data structure. Hence making them very fast is in our interest.

## Linear Probing

This is significantly faster than chained hashing. This uses open addressing. Instead of using buckets, you use slots. Slots can only have one item. Items want to go to their assigned slot.

- We maintain an array of slots, which we think of as forming a ring.

![CleanShot 2025-02-25 at 15.55.51.png](Hashing%20pt%202%201a5b37deb65d80e3bb41d8cda50c9792/CleanShot_2025-02-25_at_15.55.51.png)

<aside>
‼️

**General Premise:** All items want to go to their assigned slot based on their hash.

**Rules:**

1. If a slot is empty, the item will take the slot
2. If the slot is full, the item will move to the next available slot moving clockwise through the ring until it finds a free spot.
3. When removing an element from the table, it will be marked with a tombstone to let future elements to be searched because to search you look at where you intend to find something, and then move clockwise to check if it was displaced, only stopping if you see a blank space.
</aside>

### Procedure

- **To check if an element exists in the table…**
    1. Compute the hash code of the element
    2. Jump to that location in the table
    3. Move forward (scanning forward) until you find the item or an empty slot (you can look over tombstones).
- **To insert an element into the table…**
    1. If the item already exists, do nothing.
    2. Jump to the slot given by the hash code of the element and wrap forward until you find either a blank space or a tombstone. If you find either, place the item there.
- **To remove an element from the table…**
    1. Jump to the slot by the hash code of the element
    2. Walk forward until the item or an empty slot is found, and replace it with a tombstone.

### How fast is it?

Since, at most, every item will have a slot, then the load factor will be less than one ($\alpha \leq 1$). In this case the time complexity is constant ($O(1)$). This is the same big-O as a chained hash table with a different strategy!

## Robin Hood Hashing

With linear probing, you have long chains of items that had to be displaced because their slot was empty. Robin Hood hash tables implement a notion of fairness into the equation where you equally displace all items in the array. This makes the data-structure much faster.

### Procedure

- **To check if an element exists in the table…**
    1. Jump to the spot on the table with the elements hash code
    2. Scan forward while you keep track of the steps you’ve taken. Stop when you find the item, you find a blank slot, or you find a filled slot closer to home than the number of steps you’ve taken
- **To insert an element into the table…**
    1. If the element is already in the table, do nothing!
    2. Jump to the spot given by the elements hash code. Scan forward and keep track of the steps taken. If you find an empty slot, place the element there.
    3. Otherwise, if the current slot is taken and closer to home than the element you’re inserting, place the item to insert there, displacing the element that was at that spot, and continue the insertion as if you were inserting the displaced element. Recursion!
- **To remove an element from the table…**
    1. Jump to the slot given by the hash code of the element
    2. Walk forward until the item or an empty slot is found. If the item is found, remove it. Then, keep moving forward, moving elements backward in the table one slot until an empty slot or item in its home position is found.