#define COMMAND_MIC_DATA 0
#define COMMAND_NEXT_EFFECT 1
#define COMMAND_PREVIOUS_EFFECT 2

struct NetworkPacket
{
    uint8_t command;
    uint8_t data;
};