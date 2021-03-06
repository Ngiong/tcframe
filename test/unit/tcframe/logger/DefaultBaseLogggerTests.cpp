#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockLoggerEngine.hpp"
#include "tcframe/logger/DefaultBaseLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class DefaultBaseLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    DefaultBaseLogger logger = DefaultBaseLogger(&engine);
};

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Sample) {
    EXPECT_CALL(engine, logHeading("SAMPLE TEST CASES"));
    logger.logTestGroupIntroduction(TestGroup::SAMPLE_ID);
}

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Official) {
    EXPECT_CALL(engine, logHeading("OFFICIAL TEST CASES"));
    logger.logTestGroupIntroduction(TestGroup::MAIN_ID);
}

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Official_WithGroups) {
    EXPECT_CALL(engine, logHeading("TEST GROUP 3"));
    logger.logTestGroupIntroduction(3);
}

TEST_F(DefaultBaseLoggerTests, TestCaseIntroduction) {
    EXPECT_CALL(engine, logHangingParagraph(1, "foo_1: "));
    logger.logTestCaseIntroduction("foo_1");
}

TEST_F(DefaultBaseLoggerTests, ExecutionResults) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "scorer1: diff: abc"));

        EXPECT_CALL(engine, logListItem1(2, "Execution of scorer2 failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit signal: " + string(strsignal(SIGSEGV))));

        EXPECT_CALL(engine, logListItem1(2, "Execution of solution failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit code: 1"));
        EXPECT_CALL(engine, logListItem2(3, "Standard error: err"));
    }

    map<string, ExecutionResult> executionResults = {
            {"solution", ExecutionResultBuilder().setExitCode(1).setStandardError("err").build()},
            {"scorer1", ExecutionResultBuilder().setExitCode(0).setStandardError("diff: abc").build()},
            {"scorer2", ExecutionResultBuilder().setExitSignal(SIGSEGV).build()}};

    logger.logExecutionResults(executionResults);
}

}
