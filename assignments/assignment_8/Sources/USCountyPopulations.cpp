#include "USCountyPopulations.h"
#include "GUI/TextUtils.h"
using namespace std;

Node* USCountyPopulations::data() {
    return loadDataFile("res/us-county-populations-2020.data");
}

string USCountyPopulations::weightToString(double weight) {
    return addCommasTo(int(weight));
}
