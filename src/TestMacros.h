#ifndef UNITTEST_TESTMACROS_H
#define UNITTEST_TESTMACROS_H

#include "Config.h"
#include "ExecuteTest.h"
#include "AssertException.h"
#include "TestDetails.h"
#include "MemoryOutStream.h"

#ifndef UNITTEST_POSIX
	#define UNITTEST_THROW_SIGNALS
#else
	#include "Posix/SignalTranslator.h"
#endif

#ifdef TEST
    #error UnitTest++ redefines TEST
#endif

#ifdef TEST_EX
	#error UnitTest++ redefines TEST_EX
#endif

#ifdef TEST_FIXTURE_EX
	#error UnitTest++ redefines TEST_FIXTURE_EX
#endif

#ifndef CREATE_TEST_ATTRIBUTES
    #define CREATE_TEST_ATTRIBUTES(...)
#endif

#ifndef DESTROY_TEST_ATTRIBUTES
    #define DESTROY_TEST_ATTRIBUTES()
#endif

namespace UnitTestCategory
{
	inline const char* GetCategoryName()
	{
		return "";
	}
}

#define SUITE(Name)                                                         \
	namespace Suite##Name {                                                        \
        namespace UnitTestSuite {                                           \
            inline char const* GetSuiteName () {                            \
                return #Name ;                                              \
            }                                                               \
        }                                                                   \
    }                                                                       \
	namespace Suite##Name

#define TEST_EX(Name, List, ...)                                           \
    class Test##Name : public UnitTest::Test                               \
    {                                                                      \
    public:                                                                \
		Test##Name() : Test(#Name, UnitTestSuite::GetSuiteName(), UnitTestCategory::GetCategoryName(), __FILE__, __LINE__) {}  \
        virtual void CreateAttributes(std::vector<const UnitTest::TestAttribute*>& attributes) const \
            { CREATE_TEST_ATTRIBUTES(__VA_ARGS__); }                       \
        virtual void DestroyAttributes(std::vector<const UnitTest::TestAttribute*>& attributes) const   \
            { DESTROY_TEST_ATTRIBUTES(); }                                 \
    private:                                                               \
        virtual void RunImpl() const;                                      \
    } test##Name##Instance;                                                \
																		   \
    UnitTest::ListAdder adder##Name (List, &test##Name##Instance);         \
																		   \
    void Test##Name::RunImpl() const


#define TEST(Name, ...) TEST_EX(Name, UnitTest::Test::GetTestList(), __VA_ARGS__)


#define TEST_FIXTURE_EX(Fixture, Name, List, ...)                                    \
    class Fixture##Name##Helper : public Fixture									 \
	{																				 \
	public:																			 \
        explicit Fixture##Name##Helper(UnitTest::TestDetails const& details) : m_details(details) {} \
        void RunImpl();                           \
        UnitTest::TestDetails const& m_details;                                      \
    private:                                                                         \
        Fixture##Name##Helper(Fixture##Name##Helper const&);                         \
        Fixture##Name##Helper& operator =(Fixture##Name##Helper const&);             \
    };                                                                               \
																					 \
    class Test##Fixture##Name : public UnitTest::Test                                \
    {                                                                                \
    public:                                                                          \
	    Test##Fixture##Name() : Test(#Name, UnitTestSuite::GetSuiteName(), UnitTestCategory::GetCategoryName(), __FILE__, __LINE__) {} \
        virtual void CreateAttributes(std::vector<const UnitTest::TestAttribute*>& attributes) const \
            { CREATE_TEST_ATTRIBUTES(__VA_ARGS__); }                                 \
        virtual void DestroyAttributes(std::vector<const UnitTest::TestAttribute*>& attributes) const   \
            { DESTROY_TEST_ATTRIBUTES(); }                                           \
    private:                                                                         \
        virtual void RunImpl() const;             \
    } test##Fixture##Name##Instance;                                                 \
																					 \
    UnitTest::ListAdder adder##Fixture##Name (List, &test##Fixture##Name##Instance); \
																					 \
    void Test##Fixture##Name::RunImpl() const	 \
	{																				 \
		bool ctorOk = false;														 \
		try {																		 \
			Fixture##Name##Helper fixtureHelper(m_details);							 \
			ctorOk = true;															 \
			UnitTest::ExecuteTest(fixtureHelper, m_details);					 \
		}																			 \
		catch (UnitTest::AssertException const& e)											 \
		{																			 \
            HANDLE_FORCED_NO_EXCEPTIONS(e);                                          \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details.testName, m_details.suiteName, m_details.category, e.Filename(), e.LineNumber()), e.what()); \
		}																			 \
		catch (std::exception const& e)												 \
		{																			 \
            HANDLE_FORCED_NO_EXCEPTIONS(e);                                          \
			UnitTest::MemoryOutStream stream;													 \
			stream << "Unhandled exception: " << e.what();							 \
			UnitTest::CurrentTest::Results()->OnTestFailure(m_details, stream.GetText());				 \
		}																			 \
		catch (...) {																 \
			if (ctorOk)																 \
			{																		 \
	            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __FILE__, __LINE__),	 \
					"Unhandled exception while destroying fixture " #Fixture);		 \
			}																		 \
			else																	 \
			{																		 \
				UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __FILE__, __LINE__),   \
					"Unhandled exception while constructing fixture " #Fixture);         \
			}																		 \
		}																			 \
    }                                                                                \
    void Fixture##Name##Helper::RunImpl()

#define TEST_FIXTURE(Fixture,Name,...) TEST_FIXTURE_EX(Fixture, Name, UnitTest::Test::GetTestList(),__VA_ARGS__)


#endif
