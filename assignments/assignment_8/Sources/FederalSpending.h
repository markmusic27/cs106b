/* File: FederalSpending.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * DataSource for visualizing federal spending data.
 */
#pragma once

#include "Demos/DataSource.h"

class FederalSpending: public DataSource<FederalSpending> {
public:
    Node* data();
    std::string weightToString(double weight);
};
