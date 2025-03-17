#include "DataSource.h"
#include "GUI/TextUtils.h"
#include "queue.h"
#include <iomanip>
#include <fstream>
using namespace std;

/***** Default implementations *****/
string DataSourceImpl::weightToString(double weight) {
    return to_string(weight);
}

/***** .data File Parsing *****/
namespace {
    /* Tokenization logic: breaks a string apart into its constituent tokens. */
    Queue<string> tokenize(istream& in) {
        Queue<string> result;
        while (in >> ws && !in.eof()) {
            double value;
            if (in.peek() == '(' || in.peek() == ')') {
                result.enqueue(string(1, in.get()));
            } else if (in >> value) {
                result.enqueue(to_string(value));
            } else {
                /* Previous operation must have failed to read a double, so we need
                 * to reset the stream state.
                 */
                in.clear();
                result.enqueue(readQuotedVersionOf(in));
                if (!in) error("Sin!");
            }
        }

        return result;
    }

    /* Deserialization logic. */
    Node* deserialize(Queue<std::string>& tokens) {
        if (tokens.isEmpty()) error("No more tokens!");
        if (tokens.dequeue() != "(") error("Expected open parenthesis.");

        string name = tokens.dequeue();

        if (stringIsDouble(tokens.peek())) {
            double weight = stringToReal(tokens.dequeue());

            /* Read closing parenthesis. */
            tokens.dequeue();
            if (weight < 0) return nullptr;

            return new Node{ name, weight };
        }

        /* Otherwise, keep reading children. */
        Vector<Node*> children;
        double totalWeight = 0.0;
        while (tokens.peek() != ")") {
            auto* next = deserialize(tokens);
            if (next != nullptr) {
                children += next;
                totalWeight += next->weight;
            }
        }

        tokens.dequeue(); // Remove close paren

        if (children.isEmpty()) return nullptr;
        sort(children.begin(), children.end(), [](Node* lhs, Node* rhs) {
            return lhs->weight > rhs->weight;
        });

        Node* result = new Node{ name, totalWeight, children };
        return result;
    }
}

Node* DataSourceImpl::loadDataFile(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input) error("Cannot open file '" + filename + '"');

    auto tokens = tokenize(input);
    return deserialize(tokens);
}

/***** Registry *****/
namespace DataSourceRegistry {
    map<string, Constructor>& sources() {
        static map<string, Constructor> theRegistry;
        return theRegistry;
    }
}
