#include "IntegrationTest.h"

#include "drivers/ADC.h"

class TestADC : public IntegrationTest {
public:
    TestADC() :
        IntegrationTest("ADC", true)
    {}

    void init() {
        adc.init();
    }

    void update() {
        for (int c = 0; c < ADC::Channels; ++c) {
            DBG("adc%d: %d", c, adc.channel(c));
        }
    }

private:
    ADC adc;
};

INTEGRATION_TEST(TestADC)
