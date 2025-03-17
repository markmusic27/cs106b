/* File: DataSource.h
 * Author Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Type representing a data source - something that can produce a tree
 * for use in a treemapping visualization, plus auxiliary helper
 * functions that make that rendering look pretty.
 */
#pragma once

#include "Treemap.h"
#include <string>

/* Internal base class for all DataSources. You should not need to
 * use this type directly.
 */
class DataSourceImpl {
public:
    /* Polymorphic classes need virtual destructors. */
    virtual ~DataSourceImpl() = default;

    /* Where the data comes from. */
    virtual Node* data() = 0;

    /* Given a weight, returns a human-readable version of that
     * weight. By default, this uses C++'s default string conversion.
     */
    virtual std::string weightToString(double weight);

protected:
    /* Utility function that loads a .data file with the given name.
     * This is used by a few of our demos.
     */
    Node* loadDataFile(const std::string& filename);
};

/* * * * * Data source detection and registry. * * * * */

#include <functional>
#include <memory>
#include <cstdlib>
#include <cxxabi.h> // Nonstandard, but supported on g++ and clang.
#include <string>
#include <map>
#include "error.h"

namespace DataSourceRegistry {
    /* The actual registry. */
    using Constructor = std::function<std::shared_ptr<DataSourceImpl>()>;
    std::map<std::string, Constructor>& sources();
}

template <typename T> class DataSource: public DataSourceImpl {
public:
    ~DataSource() {
        _initializer++;
    }

private:
    static int _initializer;
    static int registerType() {
        /* std::type_info does not guarantee that .name() will be at all human-readable.
         * Use this g++/clang-specific logic to "demangle" the name back into a human-readable
         * format.
         */
        int statusCode;
        auto* realName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &statusCode);
        if (statusCode != 0) error("Internal error: Couldn't demangle name?");

        /* Store the constructor. */
        DataSourceRegistry::sources().insert(std::make_pair(std::string(realName), [] {
            return std::make_shared<T>();
        }));

        std::free(realName);
        return 137; // They said I could return anything, so I returned 137.
    }
};

template <typename T>
int DataSource<T>::_initializer = DataSource<T>::registerType();
