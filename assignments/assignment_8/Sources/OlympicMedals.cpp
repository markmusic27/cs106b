#include "OlympicMedals.h"
using namespace std;

Node* OlympicMedals::data() {
    return loadDataFile("res/olympic-medals-2020.data");
}

string OlympicMedals::weightToString(double weight) {
    return to_string(int(weight));
}
