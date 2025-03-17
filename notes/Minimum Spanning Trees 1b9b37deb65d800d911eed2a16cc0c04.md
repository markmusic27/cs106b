# Minimum Spanning Trees

Type: Note
Description: Notes on lecture from 03/12/2025
Last Edited: March 16, 2025 7:55 PM

## Terminology

- **Connected Graph:** A graph is connected if you can start at any node and walk to every node in the graph
- **Cycles:** There is a cycle in the graph when multiple nodes link to each other in a cyclic fashion. (i.e., a triangle)
- Spanning Trees: A spanning tree is a way to make a connected graph with no cycles.
- Minimum Spanning Tree: The spanning tree that has the smallest cost

### Finding an MST (Kruskal’s Algorithm)

This algorithm takes a graph as an input and outputs the MST of that graph.

1. Remove all edges from the graph
2. Repeatedly find the cheapest edge that doesn’t create a cycle and add it back.

### Finding Cycles

If adding an edge results in two nodes being connected that are already somehow connected, then that edge would result in a cycle

The typical approach: break the nodes apart into clusters/

- Initially, each node is in its own cluster.
- Whenever an edge is added, the cluster for the endpoints are merged together into a new cluster

To check if a cycle will be created, check if the two nodes you want to connect are in the same cluster. If they are in the same cluster, then that edge would result in a cycle.

<aside>
🗣

**Implementation:**

- Place every node into its own cluster (i.e., you have as many clusters as nodes)
- Place all edges into a priority queue
- While there are two or more clusters remaining…
    - Dequeue an edge from the priority queue
    - If its endpoints are not in the same cluster
        - Merge the clusters containing the endpoints
        - Add the edge to the resulting spanning tree
- Return the resulting spanning tree.
</aside>

## Data Clustering

Given a set of points, break those points apart into clusters. This could be in $n\text{-dimensions}$.

Something you could do is maximum-separation clustering. This maximizes the distance between the resulting clusters in as large as possible. Specifically, it maximizes the minimum distance between two points in different clusters. It’s not always idea.

### Implementing using Kruskal’s Algorithm

You specify the number of clusters you want, and Kruskal’s Algorithm will continue to treat the data where the weight of each edge is their spatial distance calculated by Pythagorean Theorem. The algorithm keeps running until it’s clustered the data into three buckets. Hence, you treat the data as a graph.

```python
Pseudocode Implementation

clusterData(int numClusters, Node* graph):
	calculate the weight for each edge with distance
	feed that into Kruskal’s Algorithm with numClusters
	return the MST
```

## Mazes with Kruskal’s Algorithm

1. Create a grid graph
2. Give each edge a random weight
3. Compute an MST of the graph