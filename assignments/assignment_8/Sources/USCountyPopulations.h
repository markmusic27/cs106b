/* File: USCountyPopulations.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Visualization of US county populations from 2020.
 */
#pragma once

#include "Demos/DataSource.h"

class USCountyPopulations: public DataSource<USCountyPopulations> {
public:
    Node* data();
    std::string weightToString(double weight);
};
