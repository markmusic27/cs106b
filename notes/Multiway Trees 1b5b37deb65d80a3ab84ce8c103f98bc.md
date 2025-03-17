# Multiway Trees

Type: Note
Description: Notes from lecture on 03/05/2025
Last Edited: March 13, 2025 3:24 PM

### What is it?

Binary Search Trees can have max two nodes for each root and are ordered in a specific way.

There may be data that is not sorted in this way. This can be structured in a tree.

### Representing Multiway Trees

If you know that there is a max branching factor (like each root can have, *at most,*  4 children), you can represent each of these as a pointer.

```cpp
struct Node {
	string data;
	Node* left;
	Node* right;
	Node * down;
}
```

If you don’t know the max branching factor (like the # of children varies for each node), you can just have a list of children.

```cpp
struct Node {
	string data;
	Vector<Node*> children;
}
```

You could also have a `Set<Node*>` or `Map<string, Node*>` .

If you wanted to have something more first principles that uses only pointers, you could have a pointer by making a dynamically allocated array with `Node**`

```cpp
struct Node {
	string data;
	Node** children;
	int numChildren;
}
```

You can also do something where each child points to its parent which ultimately minimizes the number of pointers needed

![CleanShot 2025-03-12 at 21.10.00@2x.png](Multiway%20Trees%201b5b37deb65d80a3ab84ce8c103f98bc/CleanShot_2025-03-12_at_21.10.002x.png)

### Representing them in text

A good tool for debugging and delineating the shape of a tree is to write them in text where each node has brackets for their children and if there are no children, then there are no brackets.

![CleanShot 2025-03-13 at 14.50.13.png](Multiway%20Trees%201b5b37deb65d80a3ab84ce8c103f98bc/CleanShot_2025-03-13_at_14.50.13.png)

```cpp
struct Node {
	string value;
	Vector<Node*> children;
}

void prettyPrint(Node* tree){
	// No tree
	if (tree == nullptr){
		return;
	}
	
	prettyPrintRec(tree, "")
}

void prettyPrintRec(Node* tree, const string& prefix){
	// Base Case: Node has no children
	if (tree->children.is){
		cout << prefix << tree->value << endl;
		return;
	}
	
	// Recursive Case: Node has children, so print out children and braces
	
	// eg. A {
	cout << prefix << tree->value << " {" << endl;
	
	// Print children
	for (Node* subtree: tree->children){
		prettyPrintRec(subtree, prefix + "    ");
	}
	
	// Print end brace
	cout << prefix << "}" << endl;
}
```

## JSON

- JSON is a popular format for exporting structured data (JavaScript Object Notation)
- You can find all sorts of JSON data sets out there.

You can create a JSON object from a string or a stream:

```cpp
JSON str = JSON::parse("[1, 2, 3]");
JSON file = JSON::parse(input);
```

You can also check the type of JSON data and get a view of the data as that type:

```cpp
if (json.type() == JSON::Type::NUMBER){
	cout << json.asInteger() << endl;
}
```

You can also get elements in an array or a key in objects

```cpp
JSON first = json[0];
JSON cute = json["quokka"];
```

If you want to read a file, you can with `ifstream` which stands for input file stream

```cpp
int main() {
	ifstream input("Sample.json");
	JSON data = JSON::parse(input);
	
	cout << data << endl;
	
	return 0;
}
```

JSON data is a multiway tree!!!