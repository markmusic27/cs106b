#include "CaliforniaPowerGeneration.h"
#include "CSVReader.h"
#include "GUI/TextUtils.h"
#include "gthread.h"
#include "map.h"
#include "set.h"
#include "urlstream.h"
#include "strlib.h"
#include <sstream>
#include <functional>
using namespace std;

namespace {
    /* Number of times to try downloading something before giving up. */
    const int kMaxTries = 10;

    /* URL to pull from. */
    const string kURL = "https://www.caiso.com/outlook/current/fuelsource.csv";

    /* Map from types to sources. */
    const Map<string, Set<string>> kTypeSources = {
        { "Renewables",     { "Solar", "Wind", "Geothermal", "Biomass", "Biogas", "Small hydro" } },
        { "Non-Renewables", { "Coal",  "Natural Gas", "Large Hydro", "Batteries", "Nuclear" } }, // Should Large Hydro count as renewables? The CAISO site doesn't and I've followed their lead.
        { "Additional",     { "Other\r", "Imports" } } // The data set has a weird error where it appends a carriage-return to "Other" for some reason
    };

    /* Gets a CSV read of the raw data file from CAISO with current power generation. */
    CSVReader loadRawCSV() {
        for (size_t i = 0; i < kMaxTries; i++) {
            iurlstream downloader;
            downloader.setHeader("User-Agent", "A programming assignment for Stanford's CS106B course. Contact: htiek@cs.stanford.edu");
            downloader.open(kURL);

            if (!downloader) {
                /* Take a break to see if pausing helps. */
                GThread::getCurrentThread()->sleep(10);
                continue;
            }

            return CSVReader::parse(downloader);
        }

        error("Unable to download power generation data from CAISO website.");
    }

    void sortChildren(Vector<Node*>& children) {
        /* Use a very, very bad algorithm called Stooge Sort to sort the data.
         * You should NOT do this in your final version of the assignment -
         * it's a really bad idea!
         */
        function<void (int, int)> stoogesort = [&](int low, int high) {
            if (children[low]->weight < children[high]->weight) {
                iter_swap(children.begin() + low, children.begin() + high);
            }
            if (high - low > 1) {
                int length = (high - low + 1) / 3;
                stoogesort(low, high - length);
                stoogesort(low + length, high);
                stoogesort(low, high - length);
            }
        };

        stoogesort(0, children.size() - 1);
    }
}

Node* CaliforniaPowerGeneration::data() {
    auto csv = loadRawCSV();

    /* The last row holds the most recent data. */
    int lastRow = csv.numRows() - 1;

    /* See which types we need to pull from. */
    Set<string> usedTypes;
    for (string type: kTypeSources) {
        /* Include this area if it has a positive contribution. */
        bool foundPositive = false;
        for (string source: kTypeSources[type]) {
            if (stringToReal(csv[lastRow][source]) > 0) {
                foundPositive = true;
                break;
            }
        }
        if (foundPositive) {
            usedTypes += type;
        }
    }

    /* Need to have at least SOME power generation. */
    if (usedTypes.isEmpty()) {
        return nullptr;
    }

    /* Now build the tree based on what we have here. */
    Node* result = new Node { "Current CA Power Generation", 0 };
    for (string type: usedTypes) {
        Node* thisType = new Node { type, 0 };
        for (string source: kTypeSources[type]) {
            double amount = stringToReal(csv[lastRow][source]);
            if (amount > 0) {
                thisType->children += new Node { source, amount };
                thisType->weight += amount;
            }
        }

        result->children += thisType;
        result->weight += thisType->weight;
    }

    /* Now sort all the children. */
    sortChildren(result->children);
    for (Node* node: result->children) {
        sortChildren(node->children);
    }

    return result;
}

string CaliforniaPowerGeneration::weightToString(double weight) {
    return addCommasTo(int(weight)) + "MW";
}
