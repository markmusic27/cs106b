/* File: CSDegrees.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Data source for visualizing CS degrees granted in the 2023-2024 academic
 * year at Stanford.
 */
#pragma once

#include "Demos/DataSource.h"

class CSDegrees: public DataSource<CSDegrees> {
public:
    Node* data();
    std::string weightToString(double weight);
};
