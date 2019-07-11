#include "Microphone.h"

#define ERROR_FAILED_TO_INITIALIZE_I2S 1

Microphone::Microphone(uint16_t sampleRate)
{
    if (!i2s_rxtx_begin(true, false))
    {
#ifdef DEBUG
        Serial.println("Failed to initialize I2S!");
#endif
        throw ERROR_FAILED_TO_INITIALIZE_I2S;
    }

    i2s_set_rate(sampleRate);

    sampleBuffer = new BUFFER_TYPE();
    volumeBuffer = new BUFFER_TYPE();

    sampleBufferStats = new BufferStats();
    volumeBufferStats = new BufferStats();
}

void Microphone::loop(uint16_t samples)
{
    int16_t l;
    if (i2s_read_sample(&l, 0, false))
        addSampleToBuffer(sampleBuffer, samples, l);

    addSampleToBuffer(volumeBuffer, samples, getVolume());
}

void Microphone::addSampleToBuffer(BUFFER_TYPE *buffer, uint16_t max_samples, BUFFER_ITEM_TYPE sample)
{
    //make room for new sample
    while (buffer->size() >= max_samples)
        buffer->pop_front();

    //insert new sample
    buffer->push_back(sample);
}

BUFFER_ITEM_TYPE Microphone::getVolume()
{
    calculateStatsForBuffer(sampleBuffer, sampleBufferStats);
    int16_t diff = sampleBufferStats->max - sampleBufferStats->min;
    return diff > 0 ? diff : -diff;
}

void Microphone::calculateStatsForBuffer(BUFFER_TYPE *buffer, BufferStats *bufferStats)
{
    bufferStats->max = std::numeric_limits<BUFFER_ITEM_TYPE>::min();
    bufferStats->min = std::numeric_limits<BUFFER_ITEM_TYPE>::max();
    float average = 0;
    for (size_t i = 0; i < buffer->size(); i++)
    {
        bufferStats->max = _max((*buffer)[i], bufferStats->max);
        bufferStats->min = _min((*buffer)[i], bufferStats->min);
        average += (*buffer)[i] / buffer->size();
    }
    bufferStats->average = average;
}

uint8_t Microphone::getValue()
{
    calculateStatsForBuffer(volumeBuffer, volumeBufferStats);
    uint8_t value =
        constrain(
            map(volumeBufferStats->average, 30, 600, 0, 255),
            0,
            255);

#ifdef DEBUG_MICROPHONE
    USE_SERIAL.print(255);
    USE_SERIAL.print(",");
    USE_SERIAL.print(value);
    USE_SERIAL.print(",");
    USE_SERIAL.print(getVolume());
    USE_SERIAL.print(",");
    USE_SERIAL.print(volumeBufferStats->average);
    USE_SERIAL.print(",");
    USE_SERIAL.print(volumeBufferStats->max);
    USE_SERIAL.print(",");
    USE_SERIAL.print(volumeBufferStats->min);
    USE_SERIAL.println();
#endif

    return value;
}