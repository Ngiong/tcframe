#pragma once

#include <vector>

#include "tcframe/io/LineIOSegmentScalarVariable.hpp"
#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/io/IOSegment.hpp"
#include "tcframe/io/IOSegmentVisitor.hpp"
#include "tcframe/type/Scalar.hpp"
#include "tcframe/util/StringUtils.hpp"

using std::vector;

namespace tcframe {

struct LineIOSegment : public IOSegment {
    friend class LineIOSegmentBuilder;

private:
    vector<LineIOSegmentVariable*> variables_;

public:
    const vector<LineIOSegmentVariable*>& variables() const {
        return variables_;
    }

    void accept(IOSegmentVisitor* visitor) {
        return visitor->visit(this);
    }
};

class LineIOSegmentBuilder {
private:
    LineIOSegment* subject_;

public:
    LineIOSegmentBuilder()
            : subject_(new LineIOSegment())
    {}

    template<typename T, typename = ScalarCompatible<T>>
    LineIOSegmentBuilder& addVariable(T& var, string name) {
        return addScalarVariable(Scalar::create(var, name));
    }

    LineIOSegmentBuilder& addScalarVariable(Scalar* variable) {
        subject_->variables_.push_back(new LineIOSegmentScalarVariable(variable));
        return *this;
    }

    LineIOSegment* build() {
        return subject_;
    }
};

}