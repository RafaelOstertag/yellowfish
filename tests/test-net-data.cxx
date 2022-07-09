#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../src/net/curl.hh"

class NetDataTest final : public CppUnit::TestFixture {
   public:
    void setUp() override { subject = std::make_unique<net::Data>(4); }

    void test_initialization() {
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), subject->get_length());
        CPPUNIT_ASSERT(subject->get() != nullptr);

        CPPUNIT_ASSERT_THROW(net::Data(0), net::CurlError);
    }

    void test_append() {
        uint8_t value = 42;
        subject->append(&value, sizeof(uint8_t));

        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), subject->get_length());
        CPPUNIT_ASSERT_EQUAL(value, *(subject->get()));
    }

    void test_growth() {
        for (uint8_t i = 0; i < 255; i++) {
            subject->append(&i, 1);
        }

        for (uint8_t i = 0; i < 255; i++) {
            CPPUNIT_ASSERT_EQUAL(i, subject->get()[i]);
        }
    }

    void test_big_append() {
        auto values = new uint8_t[128];

        for (uint8_t i = 0; i < 128; i++) {
            values[i] = i;
        }

        subject->append(values, 128);

        for (uint8_t i = 0; i < 128; i++) {
            CPPUNIT_ASSERT_EQUAL(i, subject->get()[i]);
        }

        delete[] values;
    }

   private:
    std::unique_ptr<net::Data> subject;

    CPPUNIT_TEST_SUITE(NetDataTest);
    CPPUNIT_TEST(test_initialization);
    CPPUNIT_TEST(test_append);
    CPPUNIT_TEST(test_growth);
    CPPUNIT_TEST(test_big_append);
    CPPUNIT_TEST_SUITE_END();
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(NetDataTest::suite());
    bool wasSucessful = runner.run("", false);
    return wasSucessful ? 0 : 1;
}
