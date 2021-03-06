#pragma once

#include <tuple>

#include "tcframe/os.hpp"
#include "tcframe/verdict.hpp"

using std::tie;

namespace tcframe {

struct ScoringResult {
private:
    Verdict verdict_;
    ExecutionResult executionResult_;

public:
    ScoringResult(const Verdict& verdict, const ExecutionResult& executionResult)
            : verdict_(verdict)
            , executionResult_(executionResult) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    bool operator==(const ScoringResult& o) const {
        return tie(verdict_, executionResult_) == tie(o.verdict_, o.executionResult_);
    }
};

}
