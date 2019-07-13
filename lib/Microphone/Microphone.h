#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>
#include <i2s.h>
#include <deque>

#ifndef DEBUG
#undef DEBUG_MICROPHONE
#endif

typedef int16_t BUFFER_ITEM_TYPE;
typedef std::deque<BUFFER_ITEM_TYPE> BUFFER_TYPE;

class BufferStats
{
public:
    BUFFER_ITEM_TYPE max;
    BUFFER_ITEM_TYPE min;
    BUFFER_ITEM_TYPE average;
};

class Microphone
{
private:
    BUFFER_TYPE *sampleBuffer;
    BUFFER_TYPE *volumeBuffer;

    void addSampleToBuffer(BUFFER_TYPE *buffer, uint16_t max_samples, BUFFER_ITEM_TYPE sample);
    void calculateStatsForBuffer(BUFFER_TYPE *buffer, BufferStats *bufferStats);

    BufferStats *sampleBufferStats;
    BufferStats *volumeBufferStats;

    BUFFER_ITEM_TYPE getVolume();

public:
    Microphone(uint16_t sampleRate = 16000);

    void loop(uint16_t samples = 12);
    uint8_t getValue();
};
#endif