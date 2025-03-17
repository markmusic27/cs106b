/* Space for you to put your own custom data source. To create a custom
 * data source, you'll need to do the following:
 *
 * 1. Uncomment the class definition below.
 * 2. Rename the class to something more descriptive than "MyCustomDataSource."
 *    You will need to change the name of the class in two places - once in
 *    the name, and once in the 'public DataSouce<...>' bit.
 * 3. Implement the member function Node* data(). This function should return
 *    a pointer to the root of a newly-allocated tree. Your code SHOULD NOT
 *    clean up this memory; our graphics code will do that for you. Feel free
 *    to add as many member functions as you'd like, including a constructor/
 *    destructor.
 * 4. (Optionally) implement the member function string weightToString. This
 *    function takes in a weight and returns a human-readable representation
 *    of that weight. By default, it just converts the double to a string, but
 *    you may want to affix it with something (e.g. $2,718 or 1.21GW), only
 *    show a certain number of decimal places, etc.
 */
#pragma once

#include "Demos/DataSource.h"

// class MyCustomDataSource: public DataSource<MyCustomDataSource> {
// public:
//     /* Returns a pointer to the node of a tree containing the data set to
//      * visualize.
//      */
//     Node* data();

//     /* Uncomment this if you want to display weights more precisely than the
//      * default "convert the double to a string."
//      */
//     // string weightToString(double weight);

//     /* Add any other member functions you'd like. */

// private:
//     /* Add whatever you'd like here. */
// };
