#!/usr/bin/python3
########################################################################################################################

import argparse
import logging
import socket
import struct
import sys
import time

LOG = logging.Logger(__name__)

########################################################################################################################

def main():
    """Main program"""

    parser = argparse.ArgumentParser(description = 'Use UBX-TIM-TM2 output from receiver to determine mains frequency')
    parser.add_argument('-v', '--verbose',
                        help     = 'Increase verbosity',
                        default  = 0,
                        action   = 'count')
    parser.add_argument('-r', '--receiver',
                        metavar  = '<host>:<port>',
                        help     = 'Receiver',
                        default  = None,
                        required = True)
    args = parser.parse_args()

    LOG.addHandler(logging.StreamHandler())
    if args.verbose >= 1:
        LOG.setLevel(logging.DEBUG)
    else:
        LOG.setLevel(logging.INFO)

    LOG.debug(args)

    # Run
    try:
        MainsFreq(input=args.receiver).run()
    except KeyboardInterrupt:
        pass
    except Exception as e:
        LOG.warning('Problem: %s', e)
        sys.exit(1)

    sys.exit(0)

# ----------------------------------------------------------------------------------------------------------------------

class UbxParser:
    """
    Parser to extract UBX messages from a data stream.
    """

    UBX_MAX_LEN = 2000 # maximum length of an UBX message we'll accept

    def __init__(self):
        self.buffer = bytearray()

    def __iter__(self):
        return self

    def __next__(self):
        msg = self.next()
        if not msg:
            raise StopIteration
        return msg

    def add(self, data):
        """Add data to parser"""
        self.buffer += data

    def _isUbx(self, buffer):
        """
        Checks if data in given buffer is a UBX message. Returns 0 if not, -1 if there is not yet enough data to
        determine, or > 0 if there is a message of that size.
        """
        size = len(buffer)

        # UBX frame start
        if size < 2:
            return -1 # wait
        if self.buffer[0] != 0xb5:
            return 0 # nada
        if self.buffer[1] != 0x62:
            return 0 # nada
        if size < 6:
            return -1 # wait
        ( payload_size, ) = struct.unpack('<H', buffer[4:6]);

        # Limit maximum payload size
        if payload_size > self.UBX_MAX_LEN:
            return 0 # nada

        # Wait for payload and frame end (checksum)
        msg_size = payload_size + 8
        if size < msg_size:
            return -1 # wait

        # Determine if message frame is correct
        ckA = 0
        ckB = 0
        for ckC in buffer[2:(msg_size - 2)]:
            ckA += ckC
            ckB += ckA
        ckA &= 0xff
        ckB &= 0xff
        if (ckA == buffer[msg_size - 2]) and (ckB == buffer[msg_size - 1]):
            return msg_size # have message
        else:
            return 0 # nada

    def next(self):
        """
        Get next UBX message. Returns None if there is none, and otherwise a bytearray object with the raw message
        frame.
        """

        while len(self.buffer) > 0:
            size = self._isUbx(self.buffer)
            # Not a UBX message, drop first byte from buffer
            if size == 0:
                del self.buffer[0]
            # It is a UBX message, remove it from the buffer and return it to the caller
            elif size > 0:
                msg = self.buffer[0:size]
                del self.buffer[0:size]
                return msg
            # Waiting for more data
            else: # size < 0
                break

        return None

# ----------------------------------------------------------------------------------------------------------------------

class UbxDecoder:
    """
    UBX message decoder base class
    """

    MSG_SIZE = 0     # Expected UBX message size
    MSG_CLS  = 0x00  # Expected UBX message class
    MSG_ID   = 0x00  # Expected UBX message ID

    def __init__(self, msg):
        """
        Create instance given a raw message (bytearray). Throws a RuntimeExcpetion in case the raw message is invalid.
        """
        if not self.checkMessage(msg):
            raise RuntimeError("This does not look like a %s message (%d != %d, 0x%02x != 0x%02x, 0x%02x != 0x%02x)!" %
                (type(self).__name__, self.MSG_SIZE, self._size, self._raw[2], self.MSG_CLS, self._raw[3], self.MSG_ID))
        self._raw = msg
        self._size = len(msg)

    @staticmethod
    def checkMessage(msg):
        """
        Check raw message frame validity. Returns true if raw message is as expected, false otherwise.
        To be implemented by each decoder class.
        """
        return False

    def decodePayload(self, spec):
        """
        Helper method to decode payload given a struct.unpack() spec, Returns tuple with all the decoded values.

        Use the following translation from the UBX types to struct.unpack() types. For example, UBX's 'U4' would be 'I'.

            | 1 2 4 8
        ----+--------
        U/X | B H I Q
        I   | b h i q
        R   |     f d
        """
        return struct.unpack('< ' + spec, self._raw[6:-2])

    def decodeBits(self, value, mask, shift):
        """
        Helper method to decode some bits given a field value, a mask and a shift.
        """
        return ( value & mask ) >> shift


class UbxTimTm2(UbxDecoder):
    """
    UBX-TIM-TM2 message decoder
    """

    MSG_NAME = 'UBX-TIM-TM2'
    MSG_SIZE = 36
    MSG_CLS  = 0x0d
    MSG_ID   = 0x03

    SCALE_TOWMS    = 1e-3 # [ms] to [s]
    SCALE_TOWSUBMS = 1e-9 # [ns] to [s]
    SCALE_ACCEST   = 1e-9 # [ns] to [s]

    @staticmethod
    def checkMessage(msg):
        return (len(msg) == UbxTimTm2.MSG_SIZE) and (msg[2] == UbxTimTm2.MSG_CLS) and (msg[3] == UbxTimTm2.MSG_ID)

    def __init__(self, msg):
        """
        msg (bytesarray): the raw UBX-TIM-TM2 message frame
        """
        UbxDecoder.__init__(self, msg)

        # Decode fields, see u-blox Interface Description document for details on the field names and descriptions
        ( self.ch, flags, self.count, self.wnR, self.wnF, self.towMsR, self.towSubMsR, self.towMsF, self.towSubMsF,
          self.accEst ) \
            = self.decodePayload('B B H H H I I I I I')

        self.mode           = self.decodeBits(flags,  0b00000001, 0)
        self.run            = self.decodeBits(flags,  0b00000010, 1)
        self.newFallingEdge = self.decodeBits(flags,  0b00000100, 2)
        self.timeBase       = self.decodeBits(flags,  0b00011000, 3)
        self.utc            = self.decodeBits(flags,  0b00100000, 5)
        self.time           = self.decodeBits(flags,  0b01000000, 6)
        self.newRisingEdge  = self.decodeBits(flags,  0b10000000, 7)

        self.scaledTowF = (self.towMsF * self.SCALE_TOWMS) + (self.towSubMsF * self.SCALE_TOWSUBMS) if self.newFallingEdge else None
        self.scaledTowR = (self.towMsR * self.SCALE_TOWMS) + (self.towSubMsR * self.SCALE_TOWSUBMS) if self.newRisingEdge else None


# ----------------------------------------------------------------------------------------------------------------------

class MainsFreq:

    def __init__(self, input=None):

        # Create and connect input handle
        self.input_handle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ix = input.index(':')
        self.input_handle.connect( ( input[0:ix], int(input[ix + 1:]) ) )
        self.input_handle.setblocking(False)

        # Parser
        self.parser = UbxParser()

        self.lastTowR = None
        self.lastTs = None

    def run(self):
        while True:

            # Get more data from TCP input
            data = None
            try:
                data = self.input_handle.recv(1000)
            except BlockingIOError:
                pass

            # Yield and try again if there is no data
            if data is None:
                time.sleep(10e-3)
                continue

            # Add data to parser and process all UBX messages found in it
            self.parser.add(data)
            for msg in self.parser:
                # Is it a UBX-TIM-TM2?
                if not UbxTimTm2.checkMessage(msg):
                    continue

                # Decode it
                try:
                    tm2 = UbxTimTm2(msg)
                except Exception as e:
                    LOG.warning("Failed decoding message: %s", e)
                    continue

                # Have rising edge?
                if tm2.scaledTowR is None:
                    continue

                #LOG.debug('%s %s %.3f', tm2.MSG_NAME, tm2.newRisingEdge, tm2.scaledTowR)

                # Have previous rising edge?
                if self.lastTowR is None:
                    self.lastTowR = tm2.scaledTowR
                    continue

                # Time delta should be around 1 second, account for TOW rollover
                dt = tm2.scaledTowR - self.lastTowR
                if dt < 0:
                    dt += 604800.0
                self.lastTowR = tm2.scaledTowR
                if (abs(1.0 - dt) > 0.5) or (abs(1.0 - dt) < 1e-12):
                    continue

                # The frequency should be around 50 Hz
                NUM_INTS = 100
                freq = (NUM_INTS / 2) / dt
                if (freq < 40.0) or (freq > 60.0):
                    continue

                # Output timestamp and frequency (JSONL) every 10 seconds
                LOG.debug('freq %.6f', freq)
                ts = time.time()
                if (self.lastTs is None) or ((ts - self.lastTs) > 9.5):
                    self.lastTs = ts
                    print('{ "ts": %.0f, "freq": %.3f }' % (ts, freq))

# ----------------------------------------------------------------------------------------------------------------------

if __name__ == '__main__':
    main()

########################################################################################################################
