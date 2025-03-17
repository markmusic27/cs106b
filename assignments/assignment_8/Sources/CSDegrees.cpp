#include "CSDegrees.h"
#include "GUI/TextUtils.h"
using namespace std;

Node* CSDegrees::data() {
    return loadDataFile("res/cs-degrees-2024.data");
}

string CSDegrees::weightToString(double weight) {
    return addCommasTo(int(weight));
}
