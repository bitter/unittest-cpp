#ifndef UNITTEST_CHECKMACROS_H 
#define UNITTEST_CHECKMACROS_H

#include "Checks.h"
#include "AssertException.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"

#ifdef CHECK
    #error UnitTest++ redefines CHECK
#endif

#ifdef CHECK_EQUAL
	#error UnitTest++ redefines CHECK_EQUAL
#endif

#ifdef CHECK_CLOSE
	#error UnitTest++ redefines CHECK_CLOSE
#endif

#ifdef CHECK_ARRAY_EQUAL
	#error UnitTest++ redefines CHECK_ARRAY_EQUAL
#endif

#ifdef CHECK_ARRAY_CLOSE
	#error UnitTest++ redefines CHECK_ARRAY_CLOSE
#endif

#ifdef CHECK_ARRAY2D_CLOSE
	#error UnitTest++ redefines CHECK_ARRAY2D_CLOSE
#endif

#ifndef DEBUG_BREAK
    #define DEBUG_BREAK while (0)
#endif

#define CHECK(value) \
    do \
    { \
        try { \
            if (!UnitTest::Check(value)) \
            { \
                UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), #value); \
                DEBUG_BREAK; \
            } \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK(" #value ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_MSG(value, message) \
    do \
    { \
        try { \
            if (!UnitTest::Check(value)) \
            { \
                UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), message); \
                DEBUG_BREAK; \
            } \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_MSG(" #value ", " #message ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_EQUAL(expected, actual) \
    do \
    { \
        try { \
            if (!UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_CLOSE(expected, actual, tolerance) \
    do \
    { \
        try { \
            if (!UnitTest::CheckClose(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_RELATIVE_ERROR_NO_ZERO(expected, actual, relativeTolerance) \
    do \
    { \
        try { \
            if (!UnitTest::CheckCloseRelativeNoZero(*UnitTest::CurrentTest::Results(), expected, actual, relativeTolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_RELATIVE_ERROR_NO_ZERO(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_ARRAY_EQUAL(expected, actual, count) \
    do \
    { \
        try { \
            if (!UnitTest::CheckArrayEqual(*UnitTest::CurrentTest::Results(), expected, actual, count, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    do \
    { \
        try { \
            if (!UnitTest::CheckArrayClose(*UnitTest::CurrentTest::Results(), expected, actual, count, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)

#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
    do \
    { \
        try { \
            if (!UnitTest::CheckArray2DClose(*UnitTest::CurrentTest::Results(), expected, actual, rows, columns, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__))) \
                DEBUG_BREAK; \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
            DEBUG_BREAK; \
        } \
    } while (0)


#define CHECK_THROW(expression, ExpectedExceptionType) \
    do \
    { \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
        catch (...) {} \
        if (!caught_) \
        { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
            DEBUG_BREAK; \
        } \
    } while(0)

#define CHECK_ASSERT(expression) \
    CHECK_THROW(expression, UnitTest::AssertException);

#endif
