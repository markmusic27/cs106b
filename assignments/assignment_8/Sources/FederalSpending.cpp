#include "FederalSpending.h"
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

Node* FederalSpending::data() {
    ifstream input("res/FederalSpending2023.csv");
    if (!input) error("Cannot open data file.");

    return loadGovernmentData(input, "Federal Spending 2023");
}

string FederalSpending::weightToString(double weight) {
    ostringstream builder;
    builder << fixed << setprecision(2) << "$";

    if (weight >= 1e12) {
        builder << (weight / 1e12) << "T";
    } else if (weight >= 1e9) {
        builder << (weight / 1e9) << "B";
    } else if (weight >= 1e6) {
        builder << (weight / 1e6) << "M";
    } else if (weight >= 1e3) {
        builder << (weight / 1e3) << "k";
    } else {
        builder << weight;
    }

    return builder.str();
}
