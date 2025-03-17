# Binary Search Trees pt. 2

Type: Note
Description: Notes for lecture from 03/03/2025
Last Edited: March 3, 2025 3:38 PM

## Deleting a Tree

Recursively delete the left tree, then delete the right tree, then delete the node.

```cpp
void deleteTree(Node* root){
	// Empty BST
	if (node == nullptr) return;
	
	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}
```

## Postorder Traversal

1. Recursively visit all of the nodes in the two subtrees in whichever order you’d like
2. Visit the code itself

Different to inorder traversal where you visit the left tree, then the root, and then the right node

## Preorder Traversal

You can also travel to the root, and then travel to the children left and then right.

## Tree Terminology

You can describe how big a tree is like the amount of nodes and you can use other terminology.

- The **height** of a tree is the number of links in the longest path from the root to a leaf (no children).

![CleanShot 2025-03-03 at 13.56.58@2x.png](Binary%20Search%20Trees%20pt%202%201abb37deb65d80709d99d103492b2a6a/CleanShot_2025-03-03_at_13.56.582x.png)

This BST has a height two.

![CleanShot 2025-03-03 at 13.57.28@2x.png](Binary%20Search%20Trees%20pt%202%201abb37deb65d80709d99d103492b2a6a/CleanShot_2025-03-03_at_13.57.282x.png)

This BST has height 6.

<aside>
‼️

**Important Notes:**

1. A BST with one item has height of `0`
2. An empty BST has a height of `-1` by convention
</aside>

## Building a Tree (Why height matters)

The order in which you add items to a BST changes the shape of the tree. You can have trees with the same items but completely different values. This matters because of efficiency.

![CleanShot 2025-03-03 at 14.05.46@2x.png](Binary%20Search%20Trees%20pt%202%201abb37deb65d80709d99d103492b2a6a/CleanShot_2025-03-03_at_14.05.462x.png)

Note that these two trees have the same values but one has a `h=2` and the other has `h=6`. Hence, the runtime of the tree is $O(h)$.

You can maximize or minimize the BST height all the way to $h=n$.

- **Maximum height:** all nodes are in a chain. Height is $O(n)$
- **Minimum height:** tree is complete as possible. Height is $O(log_2 n)$. This is because you can only double something $O(log_2 n)$ times before it exceeds $n$.

<aside>
🗣

**Fun fact:**

If you added all of the protons into a BST, the height of that tree would be $300$ since there is $2^{300}$ protons in the universe. This is crazy.

</aside>

## How to Balance Trees? (Height Optimization)

A self-balancing tree is a BST that reshapes itself on insertions and deletions to stay balanced (keep the height low). Take a look at these!

- **Red/black trees** (take CS161 or CS166!)
- **AVL trees** (covered in the textbook.)
- **Splay trees** (trees that reshape on lookups – take CS166!)
- **Scapegoat trees** (yes, that’s what they’re called.)
- **Treaps** (half binary heap, half binary search tree!)
- **Zip trees** (and their cousins the zip-zip trees.)

## Data and how balanced a tree is

The more random the data, the more balanced the tree. If there is a pattern, like sinusoidal data, the tree will be very unbalanced. **Takeaway:** if you’re adding elements to a BST and add their values randomly, you’re tree is likely balanced.

## Why use BST?

Hash tables are $O(1)$ for most operations. So why would you pick BSTs over hash tables? You would use BSTs if you need your data to be sorted and need to conduct range searches.

### Range Search

This finds all values in a BST withs some range. Examples:

- If the values in the BST are dates, we can find all events that occurred within some time window.
- If the values in the BST are number of diagnostic scans ordered, we can find all doctors who order a disproportionate number of scans.

### Range Search Function

```cpp
void printInRange(Node* root, const string& low, const string& high){
	if (root == nullptr){
		return;
	}
	
	if (high < root->value){
		printInRange(root->left, low, hight);
	} else if (root->value < low) {
		printInRange(root->right, low, high);
	} else {
		printInRange(root->left, low, hight);
		cout << root->value << endl; 
	}
}
```