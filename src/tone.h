/*!
    \file
    \brief flipflip's Arduino Uno stuff: tone and melody generator (see \ref TONE)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup TONE Tone and Melody Generator
    \ingroup FF

    This implements a tone and melody generator. It uses Timer/Counter2 (TCCR2) while tone(s) are
    output.

    Configuration:
    - #FF_TONE_ENABLE  must be set to 1 to enable this
    - #FF_TONE_PIN     pin to use

    See \ref EXAMPLES_EX11 for a demonstration.

    @{
*/

#ifndef __TONE_H__
#define __TONE_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise tone and melody generator
void toneInit(void);

//! generate a tone
/*!
    Generates a tone of given frequency for the given duration.
    Does not block while the tone is generated.

    \param[in] freq  the frequency [Hz]
    \param[in] dur   the duration [ms]
*/
void toneGenerate(const uint16_t freq, const uint16_t dur);

//! play melody (from RAM or PROGMEM)
/*!
    Plays a melody. A melody is a list of tone frequencies (#TONE_NOTE_t) and durations (in [ms] and
    an end marker (#TONE_END). The special "frequency" #TONE_PAUSE can be used for a pause
    (silence).

    See \ref TONE for more info.

    \param[in] pkMelody   list of frequencies and durations pairs, terminated with #TONE_END
    \param[in] isProgmem  set to true if \c pkFreqDur is a #PROGMEM pointer

    \returns the total duration of the melody [ms]

    \note Note that \c pkFreqDur must be available during the execution (i.e. static const, or
          malloc()'ed) or a #PROGMEM pointer.
*/
uint32_t toneMelody(const uint16_t *pkMelody, const bool isProgmem);


//! notes and their frequencies (rounded, from Wikipedia:Piano_key_frequencies)
typedef enum TONE_NOTES_e
{
    TONE_NOTE_C1  =   33, //!< note C1
    TONE_NOTE_CS1 =   35, //!< note CS1
    TONE_NOTE_D1  =   37, //!< note D1
    TONE_NOTE_DS1 =   39, //!< note DS1
    TONE_NOTE_E1  =   41, //!< note E1
    TONE_NOTE_F1  =   44, //!< note F1
    TONE_NOTE_FS1 =   46, //!< note FS1
    TONE_NOTE_G1  =   49, //!< note G1
    TONE_NOTE_GS1 =   52, //!< note GS1
    TONE_NOTE_A1  =   55, //!< note A1
    TONE_NOTE_AS1 =   58, //!< note AS1
    TONE_NOTE_B1  =   62, //!< note B1

    TONE_NOTE_C2  =   65, //!< note C2
    TONE_NOTE_CS2 =   69, //!< note CS2
    TONE_NOTE_D2  =   73, //!< note D2
    TONE_NOTE_DS2 =   78, //!< note DS2
    TONE_NOTE_E2  =   82, //!< note E2
    TONE_NOTE_F2  =   87, //!< note F2
    TONE_NOTE_FS2 =   93, //!< note FS2
    TONE_NOTE_G2  =   98, //!< note G2
    TONE_NOTE_GS2 =  104, //!< note GS2
    TONE_NOTE_A2  =  110, //!< note A2
    TONE_NOTE_AS2 =  117, //!< note AS2
    TONE_NOTE_B2  =  123, //!< note B2

    TONE_NOTE_C3  =  131, //!< note C3
    TONE_NOTE_CS3 =  139, //!< note CS3
    TONE_NOTE_D3  =  147, //!< note D3
    TONE_NOTE_DS3 =  156, //!< note DS3
    TONE_NOTE_E3  =  165, //!< note E3
    TONE_NOTE_F3  =  175, //!< note F3
    TONE_NOTE_FS3 =  185, //!< note FS3
    TONE_NOTE_G3  =  196, //!< note G3
    TONE_NOTE_GS3 =  208, //!< note GS3
    TONE_NOTE_A3  =  220, //!< note A3
    TONE_NOTE_AS3 =  233, //!< note AS3
    TONE_NOTE_B3  =  247, //!< note B3

    TONE_NOTE_C4  =  262, //!< note C4
    TONE_NOTE_CS4 =  277, //!< note CS4
    TONE_NOTE_D4  =  294, //!< note D4
    TONE_NOTE_DS4 =  311, //!< note DS4
    TONE_NOTE_E4  =  330, //!< note E4
    TONE_NOTE_F4  =  349, //!< note F4
    TONE_NOTE_FS4 =  370, //!< note FS4
    TONE_NOTE_G4  =  392, //!< note G4
    TONE_NOTE_GS4 =  415, //!< note GS4
    TONE_NOTE_A4  =  440, //!< note A4
    TONE_NOTE_AS4 =  466, //!< note AS4
    TONE_NOTE_B4  =  494, //!< note B4

    TONE_NOTE_C5  =  523, //!< note C5
    TONE_NOTE_CS5 =  554, //!< note CS5
    TONE_NOTE_D5  =  587, //!< note D5
    TONE_NOTE_DS5 =  622, //!< note DS5
    TONE_NOTE_E5  =  659, //!< note E5
    TONE_NOTE_F5  =  698, //!< note F5
    TONE_NOTE_FS5 =  740, //!< note FS5
    TONE_NOTE_G5  =  784, //!< note G5
    TONE_NOTE_GS5 =  831, //!< note GS5
    TONE_NOTE_A5  =  880, //!< note A5
    TONE_NOTE_AS5 =  932, //!< note AS5
    TONE_NOTE_B5  =  988, //!< note B5

    TONE_NOTE_C6  = 1047, //!< note C6
    TONE_NOTE_CS6 = 1109, //!< note CS6
    TONE_NOTE_D6  = 1175, //!< note D6
    TONE_NOTE_DS6 = 1245, //!< note DS6
    TONE_NOTE_E6  = 1319, //!< note E6
    TONE_NOTE_F6  = 1397, //!< note F6
    TONE_NOTE_FS6 = 1480, //!< note FS6
    TONE_NOTE_G6  = 1568, //!< note G6
    TONE_NOTE_GS6 = 1661, //!< note GS6
    TONE_NOTE_A6  = 1760, //!< note A6
    TONE_NOTE_AS6 = 1865, //!< note AS6
    TONE_NOTE_B6  = 1976, //!< note B6

    TONE_NOTE_C7  = 2093, //!< note C7
    TONE_NOTE_CS7 = 2217, //!< note CS7
    TONE_NOTE_D7  = 2349, //!< note D7
    TONE_NOTE_DS7 = 2489, //!< note DS7
    TONE_NOTE_E7  = 2637, //!< note E7
    TONE_NOTE_F7  = 2794, //!< note F7
    TONE_NOTE_FS7 = 2960, //!< note FS7
    TONE_NOTE_G7  = 3136, //!< note G7
    TONE_NOTE_GS7 = 3322, //!< note GS7
    TONE_NOTE_A7  = 3520, //!< note A7
    TONE_NOTE_AS7 = 3729, //!< note AS7
    TONE_NOTE_B7  = 3951, //!< note B7

    TONE_PAUSE    = 1,  //!< pause (silence) for melodies
    TONE_END      = 0,  //!< end of melody

} TONE_NOTE_t;


/* *************************************************************************** */

#endif // __TONE_H__

//@}
// eof
