/* File: OlympicMedals.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Visualization of Olympic medal totals from 2020.
 */
#pragma once

#include "Demos/DataSource.h"

class OlympicMedals: public DataSource<OlympicMedals> {
public:
    Node* data();
    std::string weightToString(double weight);
};
