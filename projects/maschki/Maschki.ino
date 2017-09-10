/* -*- mode: c; fill-column: 120; -*-

    Maschki -- flipflip industries 2015

    Credits: the Internets, see source

    $Id: Maschki.ino 4023 2017-07-09 10:45:25Z flip $
    $HeadURL: svn+ssh://fry/arduino/sketches/Maschki/Maschki.ino $
*/

#include <stdarg.h>
#include <stdint.h>
#include <math.h>


/* ***** Arduino (Uno) LED *********************************************************************** */

#define LED_PIN 13
#define LED_OFF digitalWrite(LED_PIN, LOW)
#define LED_ON  digitalWrite(LED_PIN, HIGH)


/* ***** debugging output ************************************************************************ */

#define SERIAL_BAUD 115200
#define SERIAL_MODE SERIAL_8N1
#define DEBUG(foo, args...) _DEBUG(F(foo), ## args)
void _DEBUG(const __FlashStringHelper *fmt, ...);


/* ***** useful macros & types ******************************************************************* */

#define BIT(bit) (1<<(bit))   //!< bit
#define UNUSED(foo) (void)foo //!< unused variable
#define TRUE  1               //!< boolean true
#define FALSE 0               //!< boolean false
#define SETBITS(port, bits)    port |= (bits)   //!< sets the bits
#define CLRBITS(port, bits)    port &= ~(bits)  //!< clears the bits
#define TOGBITS(port, bits)    port ^= (bits)   //!< toggles the bits
#define NUMOF(x) (sizeof(x)/sizeof(*(x)))       //!< number of elements in vector
#define ENDLESS TRUE          //!< for endless while loops
#define MIN(a, b)  ((b) < (a) ? (b) : (a)) //!< smaller value of a and b
#define MAX(a, b)  ((b) > (a) ? (b) : (a)) //!< bigger value of a and b
#define ABS(a) ((a) > 0 ? (a) : -(a)) //!< absolute value
#define CLIP(x, a, b) ((x) <= (a) ? (a) : ((x) >= (b) ? (b) : (x))) //!< clip value in range [a:b]


/* ***** WS2801 LED eyes ************************************************************************* */

#include "SPI.h"
#include "Adafruit_WS2801.h"

#define EYES_PIN_DATA  2 //!< data pin (yellow)
#define EYES_PIN_CLOCK 3 //!< clock pin (green)

class Eyes : public Adafruit_WS2801
{
  public:

    Eyes(void);

    enum which_e { Left, Right, Both };
    void setHsv(which_e which, const uint8_t hue, const uint8_t sat, const uint8_t val, const bool upd = true);

    void testHsv(void);

    void fadeHsv(which_e which, const uint8_t hue, const uint8_t sat, const uint8_t val, const uint16_t steps);
    bool fadeHsv(which_e which);

  private:

    struct state_s
    {
        struct { uint8_t hue; uint8_t sat; uint8_t val; } last, fade;
        struct { uint16_t n; uint16_t N; } pos;
    } mLeft, mRight;

    void hsv2rgb(const uint8_t hue, const uint8_t sat, const uint8_t val,
        uint8_t &R, uint8_t &G, uint8_t &B);
};

Eyes eyes;


/* ***** ultrasonic sensor *********************************************************************** */

#define SONIC_TRIG_PIN 6
#define SONIC_ECHO_PIN 5
#define SONIC_TIMEOUT  50000

class Sonic
{
  public:
    Sonic(void);
    uint32_t measure(void); // single raw measurement
    int8_t activity(void);  // measure, estimate distance and activity

  private:
    double distEst;
    double distAcc;

};

Sonic sonic;


/* ***** PIR sensor ****************************************************************************** */

#define PIR_SENSE_PIN 4

class PirSense
{
  public:
    PirSense(void);
    bool motion(void);

};

PirSense pir;


/* ***** Arduino setup & loop ******************************************************************** */

void setup(void)
{
    // initialise LED
    pinMode(LED_PIN, OUTPUT);
    LED_ON;

    // debugging output
    Serial.begin(SERIAL_BAUD, SERIAL_MODE);

    // say hello
    DEBUG("Maschki -- flipflip industries 2015");
    DEBUG("$Id: Maschki.ino 4023 2017-07-09 10:45:25Z flip $");

    // initialise random number generator
    randomSeed(sonic.measure());

    // initialise eyes
    eyes.begin();
    eyes.setHsv(Eyes::Left,  random(255), 255, 100);
    eyes.setHsv(Eyes::Right, random(255), 255, 100);
    //eyes.testHsv();

    LED_OFF;
}




void loop()
{
    static const unsigned speed = 50; // (delay) smaller is faster
    static const Eyes::which_e whichEye[] = { Eyes::Left, Eyes::Right };

    // human presence?
    if (pir.motion())
    {
        LED_ON;
    }
    else
    {
        LED_OFF;
    }

    // do we have activity?
    int act = sonic.activity();


    act *= 10;
    act = ABS(act);

    eyes.setHsv(Eyes::Both, 0, 255, CLIP(act, 1, 255));

#if 0

    //DEBUG("act=%i", act);
    if (act > 35)
    {
        for (int i = 0; i < 10; i++)
        {
            eyes.setHsv(Eyes::Both, 0, 0, 255);
            delay(30);
            eyes.setHsv(Eyes::Both, 0, 0, 0);
            delay(30);
        }
    }

    if (act > 2)
    {
        act *= 5;
        const uint8_t val = CLIP(act, 1, 255);
        const Eyes::which_e which = whichEye[ random(NUMOF(whichEye)) ];
        eyes.setHsv(which, random(40), 255, val);
        eyes.fadeHsv(which, 0, 255, 1, 20);
    }
    else
    {
        eyes.fadeHsv(Eyes::Both);
    }

#endif
#if 0

    eyes.setHsv(Eyes::Left, random(40), 255, random(40));
    delay(random(20));
    eyes.setHsv(Eyes::Right, random(40), 255, random(40));
    delay(random(20));
#endif

#if 0
    eyes.setHsv(Eyes::Both, 0, 0, 255);
#endif
#if 0
    eyes.setHsv(Eyes::Both, 0, 255, 30);

    eyes.fadeHsv(Eyes::Both, 0, 255, 1, 20);

    while (eyes.fadeHsv(Eyes::Both))
    {
        delay(25);
    }
#endif

    delay(speed);
}


/* ***** funky functions ************************************************************************* */

// http://playground.arduino.cc/Main/Printf
//void DEBUG(char *fmt, ...)
//{
//    char buf[128];
//    va_list args;
//    va_start(args, fmt );
//    vsnprintf(buf, 128, fmt, args);
//    va_end(args);
//    Serial.println(buf);
//}
void _DEBUG(const __FlashStringHelper *fmt, ...)
{
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args);
    va_end(args);
    Serial.println(buf);
}


/* ***** Eyes implementation ********************************************************************* */

Eyes::Eyes(void) : Adafruit_WS2801(2, EYES_PIN_DATA, EYES_PIN_CLOCK)
{
    setHsv(Both, 0, 0, 0, false);
}

void Eyes::hsv2rgb(const uint8_t hue, const uint8_t sat, const uint8_t val,
                   uint8_t &R, uint8_t &G, uint8_t &B)
{
    // Saturation/Value lookup table to compensate for the nonlinearity of human
    // vision.  Used in the getRGB function on saturation and brightness to make
    // dimming look more natural. Exponential function used to create values below
    // : x from 0 - 255 : y = round(pow( 2.0, x+64/40.0) - 1)  FIXME: this doesn't seem right
    // From: http://www.kasperkamperman.com/blog/arduino/arduino-programming-hsb-to-rgb/
    static const uint8_t skMatrixDimCurve[] =
    {
          0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
          3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
          4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
          6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
          8,   8,   9,   9,   9,   9,   9,   9,  10,  10,  10,  10,  10,  11,  11,  11,
         11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
         15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
         20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
         27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
         36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
         48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
         63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
         83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99, 101, 103, 105, 107, 109,
        110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
        146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
        193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255
    };

    const uint8_t H = hue;
    const uint8_t S = 255 - skMatrixDimCurve[255 - sat];
    const uint8_t V = skMatrixDimCurve[val];

    const uint32_t s = (6 * (uint32_t)H) >> 8;                   // the segment 0..5 (360/60 * [0..255] / 256)
    const uint32_t t = (6 * (uint32_t)H) & 0xff;                 // within the segment 0..255 (360/60 * [0..255] % 256)
    const uint32_t l = ((uint32_t)V * (255 - (uint32_t)S)) >> 8; // lower level
    const uint32_t r = ((uint32_t)V * (uint32_t)S * t) >> 16;    // ramp
    switch (s)
    {
        case 0: R = (byte)V;        G = (byte)(l + r);  B = (byte)l;        break;
        case 1: R = (byte)(V - r);  G = (byte)V;        B = (byte)l;        break;
        case 2: R = (byte)l;        G = (byte)V;        B = (byte)(l + r);  break;
        case 3: R = (byte)l;        G = (byte)(V - r);  B = (byte)V;        break;
        case 4: R = (byte)(l + r);  G = (byte)l;        B = (byte)V;        break;
        case 5: R = (byte)V;        G = (byte)l;        B = (byte)(V - r);  break;
    }
}

void Eyes::setHsv(which_e which, const uint8_t hue, const uint8_t sat, const uint8_t val, const bool upd)
{
    uint8_t R, G, B;
    hsv2rgb(hue, sat, val, R, G, B);
    switch (which)
    {
        case Left:
            mLeft.last = { hue, sat, val };
            setPixelColor(0, R, G, B);
            break;
        case Right:
            mRight.last = { hue, sat, val };
            setPixelColor(1, R, G, B);
            break;
        case Both:
            mLeft.last = mRight.last = { hue, sat, val };
            setPixelColor(0, R, G, B);
            setPixelColor(1, R, G, B);
            break;
    }
    if (upd)
    {
        show();
    }
}

void Eyes::testHsv(void)
{
    for (uint8_t h = 0; h < 255; h++)
    {
        setHsv(Left,  h,       255, 255);
        setHsv(Right, 255 - h, 255, 255);
        delay(10);
    }
}

// initialise fade
void Eyes::fadeHsv(which_e which, const uint8_t hue, const uint8_t sat, const uint8_t val, const uint16_t steps)
{
    switch (which)
    {
        case Left:
            mLeft.pos  = { 0, steps };
            mLeft.fade = { hue, sat, val };
            DEBUG("fade left  %u %u: %u %u %u -> %u %u %u",
                mLeft.pos.n, mLeft.pos.N,
                mLeft.last.hue, mLeft.last.sat, mLeft.last.val,
                mLeft.fade.hue, mLeft.fade.sat, mLeft.fade.val);
            break;
        case Right:
            mRight.pos  = { 0, steps };
            mRight.fade = { hue, sat, val };
            DEBUG("fade right %u %u: %u %u %u -> %u %u %u",
                mRight.pos.n, mRight.pos.N,
                mRight.last.hue, mRight.last.sat, mRight.last.val,
                mRight.fade.hue, mRight.fade.sat, mRight.fade.val);
            break;
        case Both:
            mLeft.pos  = mRight.pos  = { 0, steps };
            mLeft.fade = mRight.fade = { hue, sat, val };
            DEBUG("fade both %u %u: %u %u %u -> %u %u %u",
                mLeft.pos.n, mLeft.pos.N,
                mLeft.last.hue, mLeft.last.sat, mLeft.last.val,
                mLeft.fade.hue, mLeft.fade.sat, mLeft.fade.val);
            break;
    }
}

// fade next step
bool Eyes::fadeHsv(which_e which)
{
    struct state_s *pS;
    switch (which)
    {
        case Left:
            pS = &mLeft;
            break;
        case Right:
            pS = &mRight;
            break;
        case Both:
            bool l = fadeHsv(Left);
            bool r = fadeHsv(Right);
            return l ? l : r;
            break;
    }

    if (pS->pos.n <= pS->pos.N)
    {
        const double f = ( (double)pS->pos.n / (double)pS->pos.N );

        uint8_t newHue = (double)pS->last.hue + ( f * ((double)pS->fade.hue - (double)pS->last.hue) ) + 0.5;
        uint8_t newSat = (double)pS->last.sat + ( f * ((double)pS->fade.sat - (double)pS->last.sat) ) + 0.5;
        uint8_t newVal = (double)pS->last.val + ( f * ((double)pS->fade.val - (double)pS->last.val) ) + 0.5;
        uint8_t R, G, B;
        hsv2rgb(newHue, newSat, newVal, R, G, B);

        //DEBUG("fade %3u/%3u (%3i%%): %3u %3u %3u",
        //    mFadePos.n, mFadePos.N, (int16_t)(f * 100), newHue, newSat, newVal);

        switch (which)
        {
            case Left:
                setPixelColor(0, R, G, B);
                break;
            case Right:
                setPixelColor(1, R, G, B);
                break;
            default:
                break;
        }
        show();
        pS->pos.n++;
        return true;
    }
    else
    {
        return false;
    }
}

/* ***** Sonic implementation ******************************************************************** */

// initialise ultrasonic sensor
Sonic::Sonic(void)
{
    pinMode(SONIC_TRIG_PIN, OUTPUT);
    pinMode(SONIC_ECHO_PIN, INPUT);
    digitalWrite(SONIC_TRIG_PIN, LOW);

    distEst =    0.0; // initial distance estimate
    distAcc = 1000.0; // initial estimate covariance
}

// do distance measurement
uint32_t Sonic::measure(void)
{
    digitalWrite(SONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONIC_TRIG_PIN, LOW);
    //return pulseIn(SONIC_ECHO_PIN, HIGH);
    return pulseIn(SONIC_ECHO_PIN, HIGH, SONIC_TIMEOUT);
}


// measure and estimate distance, determine activity
// - < 0 = moving towards sensor
// - > 0 = moving away
// - = 0 no activity
int8_t Sonic::activity(void)
{
    // http://www.megunolink.com/articles/3-methods-filter-noisy-arduino-measurements/

    //LED_ON;

    // filter: F=1, B=0, H=1

    // the measurement
    unsigned long dist = measure();
    double z = (double)dist * 0.02; // approx. centimetres

    // time update
    const double Q = 1.0; // process noise
    double x = distEst;
    double P = distAcc + Q;

    // measurement update
    if (dist)
    {
        const double R = 0.5 + 0.5 * fabs(distEst - z); // observation noise
        double K = P * 1.0 / (P + R);             // gain
        distEst = x + K * (z - x);                // update distance estimate
        distAcc = (1.0 - K) * P;                  // update estimate covariance
    }
    else
    {
        distAcc = P;
    }

    // activity based on measurement residual and state covariance threshold
    //const double r = distEst - x;
    //int activity = fabs(r) > distAcc ? (int)(r - distAcc) : 0;

    const double r = distEst - z;
    int activity = fabs(r) > distAcc ? (int)(r - distAcc) : 0;


    DEBUG("%5li %5li %5li %5li %5i %5i",
        (int32_t)(z), (int32_t)(distEst), (int32_t)(distAcc),
        (int32_t)(distEst - z), activity, (int32_t)(distEst - x));

    //LED_OFF;

    return activity;
}


/* ***** PIR sensor implementation *************************************************************** */

PirSense::PirSense(void)
{
    pinMode(PIR_SENSE_PIN, INPUT);
}

bool PirSense::motion(void)
{
    return digitalRead(PIR_SENSE_PIN) == HIGH ? true : false;
}

/* eof */
