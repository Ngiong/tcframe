#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "GridIOSegmentManipulator.hpp"
#include "LineIOSegmentManipulator.hpp"
#include "LinesIOSegmentManipulator.hpp"
#include "RawLineIOSegmentManipulator.hpp"
#include "RawLinesIOSegmentManipulator.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"

using std::istream;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class IOManipulator {
private:
    IOFormat ioFormat_;

public:
    virtual ~IOManipulator() {}

    IOManipulator(const IOFormat& ioFormat)
            : ioFormat_(ioFormat) {}

    virtual void printInput(ostream* out) {
        print(ioFormat_.inputFormat(), out);
    }

    virtual void parseInput(istream* in) {
        parse(ioFormat_.inputFormat(), in);
    }

    virtual void parseOutput(istream* in) {
        if (!ioFormat_.outputFormats().empty()) {
            ioFormat_.beforeOutputFormat()();

            long long initialPos = in->tellg();
            bool successful = false;
            string errorMessage;
            for (const IOSegments& outputFormat : ioFormat_.outputFormats()) {
                try {
                    parse(outputFormat, in);
                    successful = true;
                    break;
                } catch (runtime_error& e) {
                    in->clear();
                    in->seekg(initialPos);
                    if (errorMessage.empty()) {
                        errorMessage = e.what();
                    }
                }
            }

            if (!successful) {
                if (ioFormat_.outputFormats().size() == 1) {
                    throw runtime_error(errorMessage);
                }
                throw runtime_error("Test case output does not conform to any of the output formats");
            }
        }
    }

private:
    void print(const IOSegments& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                GridIOSegmentManipulator::print((GridIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINE) {
                LineIOSegmentManipulator::print((LineIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINES) {
                LinesIOSegmentManipulator::print((LinesIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::RAW_LINE) {
                RawLineIOSegmentManipulator::print((RawLineIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::RAW_LINES) {
                RawLinesIOSegmentManipulator::print((RawLinesIOSegment*) segment, out);
            }
        }
    }

    void parse(const IOSegments& segments, istream* in) {
        string lastVariableName;
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                lastVariableName = GridIOSegmentManipulator::parse((GridIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINE) {
                lastVariableName = LineIOSegmentManipulator::parse((LineIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINES) {
                lastVariableName = LinesIOSegmentManipulator::parse((LinesIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::RAW_LINE) {
                lastVariableName = RawLineIOSegmentManipulator::parse((RawLineIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::RAW_LINES) {
                lastVariableName = RawLinesIOSegmentManipulator::parse((RawLinesIOSegment*) segment, in);
            }
        }
        if (!lastVariableName.empty()) {
            WhitespaceManipulator::ensureEof(in, lastVariableName);
        } else {
            WhitespaceManipulator::ensureEof(in);
        }
    }
};

}
