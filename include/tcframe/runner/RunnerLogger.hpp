#pragma once

#include <string>
#include <vector>

#include "tcframe/logger.hpp"

using std::string;
using std::vector;

namespace tcframe {

class RunnerLogger {
private:
    LoggerEngine* engine_;

public:
    virtual ~RunnerLogger() {}

    RunnerLogger(LoggerEngine* engine)
            : engine_(engine) {}

    virtual void logSpecificationFailure(const vector<string>& messages) {
        engine_->logHeading("SPECIFICATIONS");
        engine_->logParagraph(1, "The specifications contain errors:");
        for (const string& message : messages) {
            engine_->logListItem1(1, message);
        }
    }
};

class RunnerLoggerFactory {
public:
    virtual ~RunnerLoggerFactory() {}

    virtual RunnerLogger* create(LoggerEngine* engine) {
        return new RunnerLogger(engine);
    }
};

}
