#define COMMAND_MIC_DATA 0
#define COMMAND_SYNC_EFFECT_INDEX 1

struct NetworkPacket
{
    uint8_t command;
    uint8_t data;
};