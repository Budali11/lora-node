#include "main.h"
#include "FreeRTOS.h"
#include "timers.h"
void Antenna_Phase_Flip(void);
uint16_t Antenna_Switch_Sequence_Generation(uint8_t bytes[], uint16_t num_bytes,
                                            uint8_t codebook_length,
                                            uint8_t preamble_length,
                                            uint8_t **seq);