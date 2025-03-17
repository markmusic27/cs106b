// This file defines all functions for treemap creation, and multiway tree depth calculations as well as tests. LAST PSET LETS GOOO

#include "Treemap.h"
#include "GUI/SimpleTest.h"
#include "CSVReader.h"
using namespace std;


// Declare all constants used in the program below
const double kWeightCutoff = 0.5;

// Below, declare all functions that will be used later (described with comments later too)
Map<Node*, int> depthOfRec(Node* root, int depth);

Map<Node*, Rectangle> partitionChildrenOfRec(Vector<Node*> children, const Rectangle& bounds);

bool isNextMoveValid(double& currentWeight, double& totalWeight, int& stopAt, Vector<Node*>& children);

Map<Node*, Rectangle> formTreemapOfRec(Node* root, const Rectangle& bounds);

// Checks depth of a multiway tree by using recursion and depthOfRec helper function
Map<Node*, int> depthsOf(Node* root) {
    // Edge case: empty map
    if (root == nullptr) {
        return {};
    }

    // Remember the root has depth zero
    return depthOfRec(root, 0);
}

// Recursive helper function finds depth for each of the subtrees
Map<Node*, int> depthOfRec(Node* root, int depth) {
    // Base case: no children... no contribution to society I guess...
    if (root->children.isEmpty()) {
        return {{root, depth}};
    }

    Map<Node*, int> data = {};

    // Recursive case: go through each child and spawn depthOfRec
    for (Node* child: root->children) {
        data += depthOfRec(child, depth+1);
    }

    // Add parent
    data[root] = depth;

    return data;
}

// Gets the partitioning for the children of a given node
Map<Node*, Rectangle> partitionChildrenOf(Node* node, const Rectangle& bounds) {
    // Base case: No tree or leaf? Womp womp
    if (node == nullptr || node->children.isEmpty()) {
        error("Tree is either empty or has no children... SAD");
    }

    auto partition = partitionChildrenOfRec(node->children, bounds);

    return partition;
}

// Sums total weight of the children
double getTotalWeight(const Vector<Node*>& children){
    double total = 0;

    for (Node* child: children){
        total+=child->weight;
    }

    return total;
}

// Recursive helper function goes through and partitions children
Map<Node*, Rectangle> partitionChildrenOfRec(Vector<Node*> children, const Rectangle& bounds) {
    // Base case 1: No children, so return empty list
    if (children.isEmpty()) {
        return {};
    }

    // Base case 2: Only one element? More for me! (all space is allocated to leaf)
    if (children.size() == 1){
        return {{children[0], bounds}};
    }

    // Keeps track of fractional weight
    double totalWeight = getTotalWeight(children);
    double currentWeight = 0;
    int stopAt = 0;


    // Also it makes the move before to check (i.e., if next move is valid)
    while (isNextMoveValid(currentWeight, totalWeight, stopAt, children)) {
        currentWeight+=children[stopAt]->weight;

        stopAt++;
    }

    // Left are scanned and right are not scanned
    Vector<Node*> left = children.subList(0, stopAt);
    Vector<Node*> right = children.subList(stopAt, children.size() - stopAt);

    // Get rectangle split
    Rectangles rects = splitRectangle(bounds, currentWeight/totalWeight);

    // Call recursive function for children of children
    auto rectLeft = partitionChildrenOfRec(left, rects.one);
    auto rectRight = partitionChildrenOfRec(right, rects.two);

    return rectLeft + rectRight;
}

// Checks simple logic to see if next move is valid
bool isNextMoveValid(double& currentWeight, double& totalWeight, int& stopAt, Vector<Node*>& children) {
    // I know its complex, basically just note that not(a) || not(b) is not the same as not(a||b)
    return !(currentWeight/totalWeight >= kWeightCutoff || (stopAt == children.size() - 1));
}

// Forms treemap recursively using partition child and handles edge cases
Map<Node*, Rectangle> formTreemapOf(Node* root, const Rectangle& bounds) {
    // Base case: no tree (deforestation!!??)
    if (root == nullptr){
        error("Can't form treemap for null tree. cmon!");
    }

    // Return sole root
    if (root->children.isEmpty()){
        return {{root, bounds}};
    }

    auto treemap = formTreemapOfRec(root, bounds);

    // Add parent root
    treemap[root] = bounds;

    return treemap ;
}

// Recursive helper function to form treemap
Map<Node*, Rectangle> formTreemapOfRec(Node* root, const Rectangle& bounds) {
    // Base case: Leaf so no children (partitioning added in higher recursive call)
    if (root->children.isEmpty()){
        return {};
    }

    // Recursive case: add all of the roots to the treemap

    // Get children partitioning
    auto treemap = partitionChildrenOf(root, bounds);

    // Form treemap for children
    for (Node* child: root->children) {
        treemap+=formTreemapOf(child, treemap[child]);
    }

    // Remember that current node has been added in higher recursive call
    return treemap;
}

// Pertinent columns (seen by inspecting CSV)
const int kOwningAgencyNameCol = 0;
const int kFederalAccountNameCol = 4;
const int kGrossOutlayAmmountCol = 19;

// Function loads government data from CSV and returns it in a tree structure
Node* loadGovernmentData(istream& in, const string& name) {
    CSVReader csv = CSVReader::parse(in);

    // Initialize government
    Node* gov = new Node;
    gov->name = name;
    gov->weight = 0;
    gov->children = {};

    // Hash map of agencies
    Map<string, Node*> agencies = {};

    // Start at y=1 because we don't include header
    for (int i = 1; i < csv.numRows(); i++){
        // Key info
        double money = stod(csv[i][kGrossOutlayAmmountCol]);
        string accountName = csv[i][kFederalAccountNameCol];
        string agencyName = csv[i][kOwningAgencyNameCol];

        // Agency not seen yet so add node child
        if (!agencies.containsKey(csv[i][kOwningAgencyNameCol])) {
            Node* agency = new Node;
            agency->name = agencyName;
            agency->weight = 0;
            agency->children = {};

            // Add agency to map
            agencies[agencyName] = agency;

            // Add agency to gov children
            gov->children.add(agency);
        }

        // Increase government weight
        gov->weight += money;

        // Increase agency weight
        auto agency = agencies[agencyName];
        agency->weight += money;

        // Add account to agency
        Node* account = new Node;
        account->name = accountName;
        account->children = {};
        account->weight = money;
    }

    return gov;
}


/* * * * * Provided Helper Functions * * * * */

#include "GUI/TextUtils.h"

/* Recursively frees all the nodes in the given tree. */
void freeTree(Node* root) {
    /* Edge case: Empty trees require no work. */
    if (root == nullptr) return;

    /* Postorder traversal to ensure we don't read from dead objects. */
    for (Node* child: root->children) {
        freeTree(child);
    }

    delete root;
}

/* Rectangle-splitting algorithm. This algorithm splits along the long axis, so that wide
 * rectangles get split vertically and tall rectangles get split horizontally.
 */
Rectangles splitRectangle(const Rectangle& bounds, double alpha) {
    /* Validate input. */
    if (0 > alpha|| alpha > 1) {
        error("Invalid fractional split.");
    }

    /* Begin with the original bounds. */
    Rectangles result = { bounds, bounds };

    /* Unify code paths by selecting which fields of Rectangle
     * we'll be modifying.
     */
    int Rectangle::* coordinate;
    int Rectangle::* dimension;

    /* Wider than tall? */
    if (bounds.width >= bounds.height) {
        coordinate = &Rectangle::x;
        dimension  = &Rectangle::width;
    }
    /* Taller than wide? */
    else {
        coordinate = &Rectangle::y;
        dimension  = &Rectangle::height;
    }

    /* Shrink the smaller of the two rectangles so that its width (or
     * height) is a 1 - alpha fraction of what it started with. This
     * ensures the tiebreaking of sizes makes the smaller rectangle
     * indeed smaller. Then, subtract that width off of the larger
     * rectangle so that widths still sum up appropriately.
     */

    /* We use the round() function here to avoid floating-point shenanigans. */
    result.two.*dimension =  round(result.two.*dimension * (1 - alpha));
    result.one.*dimension -= result.two.*dimension;

    /* Adjust the x (or y) coordinate of the smaller rectangle so that
     * it remains flush with the first rectangle.
     */
    result.two.*coordinate += result.one.*dimension;

    return result;
}

bool operator== (const Rectangle& one, const Rectangle& two) {
    return one.x == two.x &&
           one.y == two.y &&
           one.width == two.width &&
           one.height == two.height;
}

ostream& operator<< (ostream& out, const Rectangle& rect) {
    return out << format("{ %s, %s, %s, %s }", rect.x, rect.y, rect.width, rect.height);
}


/* * * * * Test Cases Below This Point * * * * */

PROVIDED_TEST("depthsOf works in simple cases.") {
    EXPECT_EQUAL(depthsOf(nullptr), { });

    Node* singleton = new Node{ "Just Me!", 1 };
    EXPECT_EQUAL(depthsOf(singleton), { { singleton, 0 } });
    freeTree(singleton);
}

PROVIDED_TEST("depthsOf works on a tree of height 1.") {
    Node* childA = new Node{ "Child A", 3 };
    Node* childB = new Node{ "Child B", 2 };
    Node* childC = new Node{ "Child C", 1 };

    Node* tree = new Node{ "Root", 6, { childA, childB, childC } };

    auto map = depthsOf(tree);
    EXPECT_EQUAL(map.size(), 4);

    EXPECT_EQUAL(map[tree], 0);
    EXPECT_EQUAL(map[childA], 1);
    EXPECT_EQUAL(map[childB], 1);
    EXPECT_EQUAL(map[childC], 1);

    freeTree(tree);
}

PROVIDED_TEST("depthsOf works on a tree of unequal depths.") {
    /* The tree looks like this:
     *
     *                 A
     *               / | \
     *              B  E  F
     *             / \   /|\
     *            C   D G H I
     *                     / \
     *                    J   K
     */
    Node* nodeC = new Node{ "C", 1 };
    Node* nodeD = new Node{ "D", 1 };
    Node* nodeB = new Node{ "B", 2, { nodeC, nodeD } };
    Node* nodeE = new Node{ "E", 1 };
    Node* nodeG = new Node{ "G", 1 };
    Node* nodeH = new Node{ "H", 1 };
    Node* nodeJ = new Node{ "J", 1 };
    Node* nodeK = new Node{ "K", 1 };
    Node* nodeI = new Node{ "I", 2, { nodeJ, nodeK } };
    Node* nodeF = new Node{ "F", 4, { nodeG, nodeH, nodeI } };
    Node* nodeA = new Node{ "A", 7, { nodeB, nodeE, nodeF } };

    auto map = depthsOf(nodeA);
    EXPECT_EQUAL(map.size(), 11);

    EXPECT_EQUAL(map[nodeA], 0);
    EXPECT_EQUAL(map[nodeB], 1);
    EXPECT_EQUAL(map[nodeE], 1);
    EXPECT_EQUAL(map[nodeF], 1);
    EXPECT_EQUAL(map[nodeC], 2);
    EXPECT_EQUAL(map[nodeD], 2);
    EXPECT_EQUAL(map[nodeG], 2);
    EXPECT_EQUAL(map[nodeH], 2);
    EXPECT_EQUAL(map[nodeI], 2);
    EXPECT_EQUAL(map[nodeJ], 3);
    EXPECT_EQUAL(map[nodeK], 3);

    freeTree(nodeA);
}

PROVIDED_TEST("partitionChildrenOf splits two equal children 50/50.") {
    auto* tree = new Node { "A", 10, {
                     new Node{ "B", 5 },
                     new Node{ "C", 5 }
                 }
    };

    auto* nodeB = tree->children[0];
    auto* nodeC = tree->children[1];

    /* Twice as wide as tall. */
    Rectangle bounds = { 10, 20, 40, 20 };
    auto partition = partitionChildrenOf(tree, bounds);

    EXPECT_EQUAL(partition.size(), 2);
    EXPECT(partition.containsKey(nodeB));
    EXPECT(partition.containsKey(nodeC));

    EXPECT_EQUAL(partition[nodeB], { 10, 20, 20, 20 });
    EXPECT_EQUAL(partition[nodeC], { 30, 20, 20, 20 });

    freeTree(tree);
}

PROVIDED_TEST("partitionChildrenOf splits two unequal children 75/25.") {
    auto* tree = new Node { "A", 20, {
                     new Node{ "B", 15 },
                     new Node{ "C",  5 }
                 }
    };

    auto* nodeB = tree->children[0];
    auto* nodeC = tree->children[1];

    /* Four times as wide as tall. */
    Rectangle bounds = { 10, 20, 80, 20 };
    auto partition = partitionChildrenOf(tree, bounds);

    EXPECT_EQUAL(partition.size(), 2);
    EXPECT(partition.containsKey(nodeB));
    EXPECT(partition.containsKey(nodeC));

    EXPECT_EQUAL(partition[nodeB], { 10, 20, 60, 20 });
    EXPECT_EQUAL(partition[nodeC], { 70, 20, 20, 20 });

    freeTree(tree);
}

PROVIDED_TEST("partitionChildrenOf splits when half the weight exceeds the total.") {
    /* The "correct" way to do this is to first split into A/BCD, then
     * to split BCD into B/CD, then to split CD as C/D.
     */
    auto* tree = new Node { "X", 8, {
                     new Node{ "A",  4 },
                     new Node{ "B",  2 },
                     new Node{ "C",  1 },
                     new Node{ "D",  1 }
                 }
    };

    auto* nodeA = tree->children[0];
    auto* nodeB = tree->children[1];
    auto* nodeC = tree->children[2];
    auto* nodeD = tree->children[3];

    /* This rectangle is chosen so that the splits go like this:
     *
     * +-------+-------+
     * |       |       |
     * |       |   B   |
     * |       |       |
     * |   A   +---+---+
     * |       |   |   |
     * |       | C | D |
     * |       |   |   |
     * +-------+---+---+
     */
    Rectangle bounds = { 0, 0, 20, 16 };
    auto partition = partitionChildrenOf(tree, bounds);

    EXPECT_EQUAL(partition.size(), 4);
    EXPECT_EQUAL(partition[nodeA], {  0,  0, 10, 16 });
    EXPECT_EQUAL(partition[nodeB], { 10,  0, 10,  8 });
    EXPECT_EQUAL(partition[nodeC], { 10,  8,  5,  8 });
    EXPECT_EQUAL(partition[nodeD], { 15,  8,  5,  8 });

    freeTree(tree);
}

PROVIDED_TEST("partitionChildrenOf works on a trickier split.") {
    /* These end up getting split apart like this:
     *
     *             ABCDEFGHI
     *              /     \
     *           ABC     DEFGHI
     *           / \      /   \
     *          AB  C   DE   FGHI
     *         /  \     /\   /   \
     *        A   B    D  E FG    HI
     *                      /\    /\
     *                     F  G  H  I
     */
    auto* tree = new Node { "X", 32, {
                     new Node{ "A",  8 },
                     new Node{ "B",  7 },
                     new Node{ "C",  4 },
                     new Node{ "D",  4 },
                     new Node{ "E",  3 },
                     new Node{ "F",  2 },
                     new Node{ "G",  2 },
                     new Node{ "H",  1 },
                     new Node{ "I",  1 },
                 }
    };

    auto* nodeA = tree->children[0];
    auto* nodeB = tree->children[1];
    auto* nodeC = tree->children[2];
    auto* nodeD = tree->children[3];
    auto* nodeE = tree->children[4];
    auto* nodeF = tree->children[5];
    auto* nodeG = tree->children[6];
    auto* nodeH = tree->children[7];
    auto* nodeI = tree->children[8];

    /* This rectangle's dimensions are chosen so that all the boundaries
     * come out nicely as whole numbers.
     */
    Rectangle bounds = { 0, 0, 384, 78 };
    auto partition = partitionChildrenOf(tree, bounds);

    Map<Node*, Rectangle> expected = {
        { nodeA, {   0,   0,  96,  78 } },
        { nodeB, {  96,   0,  84,  78 } },
        { nodeC, { 180,   0,  48,  78 } },
        { nodeD, { 228,   0,  48,  78 } },
        { nodeE, { 276,   0,  36,  78 } },
        { nodeF, { 312,   0,  36,  52 } },
        { nodeG, { 348,   0,  36,  52 } },
        { nodeH, { 312,  52,  36,  26 } },
        { nodeI, { 348,  52,  36,  26 } },
    };

    EXPECT_EQUAL(partition.size(), expected.size());
    EXPECT(partition.containsKey(nodeA));
    EXPECT(partition.containsKey(nodeB));
    EXPECT(partition.containsKey(nodeC));
    EXPECT(partition.containsKey(nodeD));
    EXPECT(partition.containsKey(nodeE));
    EXPECT(partition.containsKey(nodeF));
    EXPECT(partition.containsKey(nodeG));
    EXPECT(partition.containsKey(nodeH));
    EXPECT(partition.containsKey(nodeI));

    EXPECT_EQUAL(partition[nodeA], expected[nodeA]);
    EXPECT_EQUAL(partition[nodeB], expected[nodeB]);
    EXPECT_EQUAL(partition[nodeC], expected[nodeC]);
    EXPECT_EQUAL(partition[nodeD], expected[nodeD]);
    EXPECT_EQUAL(partition[nodeE], expected[nodeE]);
    EXPECT_EQUAL(partition[nodeF], expected[nodeF]);
    EXPECT_EQUAL(partition[nodeG], expected[nodeG]);
    EXPECT_EQUAL(partition[nodeH], expected[nodeH]);
    EXPECT_EQUAL(partition[nodeI], expected[nodeI]);

    freeTree(tree);
}

PROVIDED_TEST("formTreemapOf works on a single node.") {
    Node* tree = new Node{ "Just Me", 1 };

    Rectangle bounds = { 0, 0, 2, 3 };
    auto treemap = formTreemapOf(tree, bounds);

    EXPECT_EQUAL(treemap.size(), 1);
    EXPECT_EQUAL(treemap[tree], bounds);

    freeTree(tree);
}

PROVIDED_TEST("formTreemapOf works on a depth-1 tree.") {
    /* Tree looks like this:
     *
     *          A
     *        / | \
     *       B  C  D
     *       3  2  1
     *
     * The correct split should look like this:
     *
     * +---------+---------+
     * |         |         |
     * |         |    C    |
     * |    B    |         |
     * |         +---------+
     * |         |    D    |
     * +---------+---------+
     *
     * Node A then encompasses the full bounds.
     */
    Node* nodeB = new Node{ "B", 3 };
    Node* nodeC = new Node{ "C", 2 };
    Node* nodeD = new Node{ "D", 1 };
    Node* nodeA = new Node{ "A", 6, { nodeB, nodeC, nodeD } };

    Rectangle bounds = { 10, 20, 50, 30 };
    auto treemap = formTreemapOf(nodeA, bounds);

    EXPECT_EQUAL(treemap.size(), 4);
    EXPECT_EQUAL(treemap[nodeA], bounds);
    EXPECT_EQUAL(treemap[nodeB], { 10, 20, 25, 30 });
    EXPECT_EQUAL(treemap[nodeC], { 35, 20, 25, 20 });
    EXPECT_EQUAL(treemap[nodeD], { 35, 40, 25, 10 });

    freeTree(nodeA);
}

PROVIDED_TEST("formTreemapOf works on a tree of uneven depths") {
    /* The tree looks like this:
     *
     *           A 8
     *          / \
     *       4 B   C 4
     *            / \
     *         2 D   E 2
     *          / \
     *         F   G
     *         1   1
     *
     * The treemap should split the original rectangle in half 50/50, then
     * recursively keep splitting one part of the rectangle 50/50 from there.
     */
    auto* nodeB = new Node{ "B", 4 };
    auto* nodeE = new Node{ "E", 2 };
    auto* nodeF = new Node{ "F", 1 };
    auto* nodeG = new Node{ "G", 1 };
    auto* nodeD = new Node{ "D", 2, { nodeF, nodeG } };
    auto* nodeC = new Node{ "C", 4, { nodeD, nodeE } };
    auto* nodeA = new Node{ "A", 8, { nodeB, nodeC } };

    Rectangle bounds = { 100, 200, 8, 6 };
    auto treemap = formTreemapOf(nodeA, bounds);
    EXPECT_EQUAL(treemap.size(), 7);

    /* Node A gets the full bounds. */
    EXPECT_EQUAL(treemap[nodeA], bounds);

    /* Node B gets the left half after a vertical split. */
    EXPECT_EQUAL(treemap[nodeB], { 100, 200, 4, 6 });

    /* Node C gets the right half after a vertical split. */
    EXPECT_EQUAL(treemap[nodeC], { 104, 200, 4, 6 });

    /* Node D gets the top half after a horizontal split. */
    EXPECT_EQUAL(treemap[nodeD], { 104, 200, 4, 3 });

    /* Node E gets the bottom half after a horizontal split. */
    EXPECT_EQUAL(treemap[nodeE], { 104, 203, 4, 3 });

    /* Node F gets the left half after a vertical split. */
    EXPECT_EQUAL(treemap[nodeF], { 104, 200, 2, 3 });

    /* Node G gets the right half after a vertical split. */
    EXPECT_EQUAL(treemap[nodeG], { 106, 200, 2, 3 });

    freeTree(nodeA);
}

#include <sstream>
#include <algorithm>

namespace {
    struct DataRow {
        string agency, account;
        double amount;
    };

    /* Utility function to let us call loadGovernmentData on a list of rows. */
    Node* loadGovernmentData(const Vector<DataRow>& rows, const string& name) {
        stringstream data;

        /* Write the header. */
        data << "owning_agency_name,federal_account_name,gross_outlay_amount" << endl;

        /* Write rows. */
        for (auto row: rows) {
            data << quotedVersionOf(row.agency)  << ","
                 << quotedVersionOf(row.account) << ","
                 << fixed << setprecision(2) << row.amount << endl;
        }

        return ::loadGovernmentData(data, name);
    }

    /* Utility function that compares two trees for equality. */
    bool areEqual(Node* one, Node* two) {
        if (one == nullptr || two == nullptr) return one == two;

        if (one->name != two->name) {
            return false;
        }

        if (!SimpleTest::Internal::areEqual(one->weight, two->weight)) {
            return false;
        }

        if (one->children.size() != two->children.size()) {
            return false;
        }

        for (int i = 0; i < one->children.size(); i++) {
            if (!areEqual(one->children[i], two->children[i])) {
                return false;
            }
        }

        return true;
    }
}

PROVIDED_TEST("loadGovernmentData reads a single account.") {
    auto* result   = loadGovernmentData({ { "A", "w", 1 } }, "Test Data");
    auto* expected = new Node{
        "Test Data", 1, {
            new Node{
                "A", 1, {
                    new Node{ "w", 1 }
                }
            }
        }
    };

    EXPECT(areEqual(result, expected));
    freeTree(result);
    freeTree(expected);
}

PROVIDED_TEST("loadGovernmentData reads multiple accounts in one agency.") {
    auto* result = loadGovernmentData({ { "A", "w", 4 },
                                       { "A", "x", 3 },
                                       { "A", "y", 2 },
                                       { "A", "z", 1 } }, "Test Data");

    auto* expected = new Node{
        "Test Data", 10, {
            new Node{
                "A", 10, {
                 new Node{ "w", 4 },
                 new Node{ "x", 3 },
                 new Node{ "y", 2 },
                 new Node{ "z", 1 },
                 }
            }
        }
    };

    EXPECT(areEqual(result, expected));
    freeTree(result);
    freeTree(expected);
}

PROVIDED_TEST("loadGovernmentData reads multiple accounts in multiple agencies.") {
    auto* result = loadGovernmentData({ { "A", "w", 4 },
                                       { "B", "w", 5 },
                                       { "A", "x", 3 },
                                       { "A", "y", 2 },
                                       { "B", "x", 2 },
                                       { "A", "z", 1 } }, "Test Data");

    auto* expected = new Node{
        "Test Data", 17, {
            new Node{
                "A", 10, {
                 new Node{ "w", 4 },
                 new Node{ "x", 3 },
                 new Node{ "y", 2 },
                 new Node{ "z", 1 },
                 }
            }, new Node {
                "B", 7, {
                 new Node{ "w", 5 },
                 new Node{ "x", 2 },
                 }
            }
        }
    };

    EXPECT(areEqual(result, expected));
    freeTree(result);
    freeTree(expected);
}

PROVIDED_TEST("loadGovernmentData discards accounts with zero or negative weight.") {
    auto* result = loadGovernmentData({ { "A", "w", 4 },
                                       { "B", "w", 5 },
                                       { "A", "x", 3 },
                                       { "A", "a", 0 },
                                       { "A", "y", 2 },
                                       { "B", "x", 2 },
                                       { "B", "y", -1 },
                                       { "C", "a", 0 },
                                       { "C", "b", -1 },
                                       { "A", "z", 1 } }, "Test Data");

    auto* expected = new Node{
        "Test Data", 17, {
            new Node{
                "A", 10, {
                 new Node{ "w", 4 },
                 new Node{ "x", 3 },
                 new Node{ "y", 2 },
                 new Node{ "z", 1 },
                 }
            }, new Node {
                "B", 7, {
                 new Node{ "w", 5 },
                 new Node{ "x", 2 },
                 }
            }
        }
    };

    EXPECT(areEqual(result, expected));
    freeTree(result);
    freeTree(expected);
}

PROVIDED_TEST("loadGovernmentData sorts entries by weight.") {
    auto* result = loadGovernmentData({
                                       { "B", "x", 2 },
                                       { "A", "x", 3 },
                                       { "A", "a", 0 },
                                       { "A", "y", 2 },
                                       { "B", "y", -1 },
                                       { "C", "a", 0 },
                                       { "C", "b", -1 },
                                       { "A", "w", 4 },
                                       { "B", "w", 5 },
                                       { "A", "z", 1 } }, "Test Data");

    auto* expected = new Node{
        "Test Data", 17, {
            new Node{
                "A", 10, {
                 new Node{ "w", 4 },
                 new Node{ "x", 3 },
                 new Node{ "y", 2 },
                 new Node{ "z", 1 },
                 }
            }, new Node {
                "B", 7, {
                 new Node{ "w", 5 },
                 new Node{ "x", 2 },
                 }
            }
        }
    };

    EXPECT(areEqual(result, expected));
    freeTree(result);
    freeTree(expected);
}

PROVIDED_TEST("loadGovernmentData reports errors if no valid data is read.") {
    Vector<DataRow> rows = {
        { "B", "x", -2 },
        { "A", "x", -3 },
        { "A", "a", 0 },
        { "A", "y", -2 },
        { "B", "y", -1 },
        { "C", "a", 0 },
        { "C", "b", -1 },
        { "A", "w", -4 },
        { "B", "w", -5 },
        { "A", "z", -1 }
    };

    EXPECT_ERROR(loadGovernmentData(rows, "Test Data"));
}

#include <fstream>
#include <cmath>
PROVIDED_TEST("loadGovernmentData passes basic checks on the real data file.") {
    ifstream input("res/FederalSpending2023.csv", ios::binary);
    if (!input) SHOW_ERROR("Internal error: Can't open federal spending file.");

    Node* data = loadGovernmentData(input, "Federal Spending 2023");
    EXPECT_NOT_EQUAL(data, nullptr);

    /* Confirm name matches. */
    EXPECT_EQUAL(data->name, "Federal Spending 2023");

    /* Confirm total weight. We round to the nearest integer here
     * to avoid rounding errors arising from inaccuracies resulting
     * from the order in which doubles are added - take CS107
     * for details!
     */
    EXPECT_EQUAL(round(data->weight), round(8904247541021.72));

    /* Confirm right number of children. */
    EXPECT_EQUAL(data->children.size(), 106);

    /* Spot-check the children. */
    EXPECT_NOT_EQUAL(data->children[0], nullptr);
    EXPECT_EQUAL(data->children[0]->name, "Department of Health and Human Services");
    EXPECT_EQUAL(round(data->children[0]->weight), round(2423434619295.49));

    EXPECT_NOT_EQUAL(data->children[39], nullptr);
    EXPECT_EQUAL(data->children[39]->name, "The Judicial Branch");
    EXPECT_EQUAL(round(data->children[39]->weight), round(613614690.93));

    EXPECT_NOT_EQUAL(data->children[46], nullptr);
    EXPECT_EQUAL(data->children[46]->name, "District of Columbia Courts");
    EXPECT_EQUAL(round(data->children[46]->weight), round(311707875.20));
    EXPECT_EQUAL(data->children[46]->children.size(), 3);
    EXPECT_NOT_EQUAL(data->children[46]->children[0], nullptr);
    EXPECT_EQUAL(data->children[46]->children[0]->name, "Salaries and Expenses, Federal Payment to the District of Columbia Courts");
    EXPECT_EQUAL(round(data->children[46]->children[0]->weight), round(270679505.30));

    freeTree(data);
}

PROVIDED_TEST("loadGovernmentData stress test.") {
    Vector<DataRow> rows;

    const int kSize = 250; // Number of agencies and accounts per agency
    Node* reference = new Node{ "Stress Test", 0.0 };

    for (int agencyID = 0; agencyID < kSize; agencyID++) {
        Node* agency = new Node{ to_string(agencyID), 0.0 };
        for (int account = 0; account < kSize; account++) {
            /* Ensures all weights are in reverse-sorted order. Add one to each
             * to ensure that we don't have zero weights.
             */
            double weight = agencyID * kSize + account + 1;
            rows.add({ to_string(agencyID), to_string(account), weight });

            reference->weight += weight;
            agency->weight    += weight;
            agency->children  += new Node{ to_string(account), weight };
        }

        /* Added everything in reverse, so flip things around. */
        reverse(agency->children.begin(), agency->children.end());
        reference->children += agency;
    }

    /* Our reference tree is reversed, so flip everything around. */
    reverse(reference->children.begin(), reference->children.end());

    /* Run loadGovernmentData on our rows. */
    auto* result = loadGovernmentData(rows, "Stress Test");
    EXPECT(areEqual(result, reference));

    freeTree(result);
    freeTree(reference);
}
