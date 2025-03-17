# Binary Search Trees pt. 1

Type: Note
Description: Notes from lecture on 02/28/2025
Last Edited: March 1, 2025 5:48 PM

## Time Complexity to Find Item in Linked List

It’s $O(n)$ because on average you need to go through the entire linked list to find the item. The most common way to do this is by checking if its either in the current cell or the next cell.

### Key Idea of Binary Search Tree

You can combine elements to be greater than or less than and store them in a binary tree to make searching more efficiency.

Think of storing these items:

![CleanShot 2025-03-01 at 17.15.21@2x.png](Binary%20Search%20Trees%20pt%201%201aab37deb65d80b184fef45ac70af46a/CleanShot_2025-03-01_at_17.15.212x.png)

Pick an arbitrary value and set it as the root. Then put all of the items smaller than the root in one side and all of the ones greater on the right side.

![CleanShot 2025-03-01 at 17.16.34@2x.png](Binary%20Search%20Trees%20pt%201%201aab37deb65d80b184fef45ac70af46a/CleanShot_2025-03-01_at_17.16.342x.png)

Then perform this recursively until no items are left.

![CleanShot 2025-03-01 at 17.17.16@2x.png](Binary%20Search%20Trees%20pt%201%201aab37deb65d80b184fef45ac70af46a/CleanShot_2025-03-01_at_17.17.162x.png)

Hence you can know the path by asking yourself a series of questions. Is it greater than or less than the current node.

### Components

- It’s a data structure called a binary search tree or BST
- Consists of nodes each of which stores at most two children
- All nodes to the left of the root are less than and all of the nodes to the right are greater than

![CleanShot 2025-03-01 at 17.23.51@2x.png](Binary%20Search%20Trees%20pt%201%201aab37deb65d80b184fef45ac70af46a/e71d8e36-2fd1-4bec-af86-5e027128a21b.png)

Binary Search Trees can either by empty, represented by `nullptr`, a single node whose left node is a BST of smaller values and right node is a BST of greater values.

```cpp
struct Node {
	**Type** value;
	Node* left; // Smaller values
	Node* right; // Greater values
}
```

## Searching a Tree

1. We have to check if the tree is `nullptr`  or if its empty
2. Check if the value is the root
3. If not, check if its greater than the root, then look at the right node
4. If its less than the root, then look at the left node.
5. Repeat recursively.

```cpp
bool contains(Node* root, int key) {
	// Base case: BST is empty or is value
	if (root == nullptr){
		return false;
	}
	
	if (root->value == key){
		return true;
	}
	
	// Recursive case: check subtrees
	if (root->value > key){
		return contains(root->left, key);
	} else {
		return contains(root->right, key);
	}
}
```

## Walking Trees (Inorder Traversals)

How to visit every value in a BST in a sorted value. Recursive implementation looks like this:

1. Print `node->left` value (call the function recursively)
2. Print the `root` value
3. Print `node->right` value (call the function recursively)

```cpp
void printInOrder(Node* root) {
	
	// Base case: empty BST
	if (root == nullptr){
		return;
	}
	
	// Recursive case:
	printInOrder(root->left);
	cout << root->value << endl;
	printInOrder(root->right);
}
```

## Adding in to a tree

We always add values where we’d typically fall off (at the end of the tree)

```cpp
bool add(Node* root, int key) {
	// Base case:
	if (root == nullptr){
		root = new Node;
		root->value = key;
		root->left = nullptr;
		root->right = nullptr;
		return;
	}
	
	if (root->value == key){
		// No duplicates!
		return;
	}
	
	// Recursive case: check subtrees
	if (root->value > key){
		add(root->left, key);
	} else {
		add(root->right, key);
	}
}
```