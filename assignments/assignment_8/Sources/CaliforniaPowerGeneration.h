/* File: CaliforniaPowerGeneration.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Data source that shows the most recent power generation data from
 * California's Independent System Operator (CAISO). Data is pulled
 * from their public website and categorized by type.
 */
#pragma once

#include "Demos/DataSource.h"

class CaliforniaPowerGeneration: public DataSource<CaliforniaPowerGeneration> {
public:
    Node* data();
    std::string weightToString(double weight);
};
