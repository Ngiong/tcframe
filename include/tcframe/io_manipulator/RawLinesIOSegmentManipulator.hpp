#pragma once

#include <iostream>
#include <stdexcept>

#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class RawLinesIOSegmentManipulator {
public:
    RawLinesIOSegmentManipulator() = delete;

    static string parse(RawLinesIOSegment* segment, istream* in) {
        string lastVariableName;

        Vector* variable = segment->variable();
        int size = segment->size()();
        for (int j = 0; j != size; j++) {
            if (size == NO_SIZE && WhitespaceManipulator::isEof(in)) {
                break;
            }

            variable->parseAndAddElementFrom(in);
            lastVariableName = TokenFormatter::formatVectorElement(variable->name(), j);

            WhitespaceManipulator::parseNewline(in, lastVariableName);
        }

        return lastVariableName;
    }

    static void print(RawLinesIOSegment* segment, ostream* out) {
        checkVectorSize(segment);
        Vector* variable = segment->variable();
        for (int j = 0; j < variable->size(); j++) {
            variable->printElementTo(j, out);
            *out << endl;
        }
    }

private:
    static void checkVectorSize(RawLinesIOSegment* segment) {
        Vector* variable = segment->variable();
        int expectedSize = segment->size()();
        if (expectedSize != NO_SIZE && expectedSize != variable->size()) {
            throw runtime_error(
                    "Number of elements of " + TokenFormatter::formatVariable(variable->name())
                    + " unsatisfied. Expected: " + StringUtils::toString(expectedSize)
                    + ", actual: " + StringUtils::toString(variable->size()));
        }
    }
};

}
