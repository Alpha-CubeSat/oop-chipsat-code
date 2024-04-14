#ifndef RADIO_CONTROLTASK_HPP_
#define RADIO_CONTROLTASK_HPP_

#include "RadioLib.h"
#include "sfr.hpp"

class RadioControlTask
{
public:
    RadioControlTask();

    /**
     * @brief Executes the actions of the current radio state, as defined in the state machine
     */
    void execute();

private:
    /**
     * @brief Sets up the radio module
     */
    void init();

    /**
     * Resets the transmit window start time and picks a new slot
     */
    void downlinkSettings();

    /**
     * @brief Wrapper around RadioLib's transmit() to provide metadata (TODO: Redundant?)
     */
    bool transmit(uint8_t *packet, uint8_t size);

    /**
     * @brief Attempts to receive a 3 byte command (TODO: Redundant?)
     */
    bool receive();

    /**
     * @brief Transmits either a normal report packet or a callsign packet
     *
     * @return True on successful transmit, false otherwise
     */
    bool executeDownlink();

    /**
     * @brief Constructs and downlinks a normal report
     *
     * @return True on successful transmit, false otherwise
     */
    bool normalReportDownlink();

    /**
     * @brief Maps a floating point value to a bounded 8 bit integer
     *
     * @param value The value to map
     * @param min The minimum value of the mapped integer
     * @param max The maximum value of the mapped integer
     * @return The mapped 8 bit integer
     */
    uint8_t map_range(float value, int min, int max);

    /**
     * @brief Parses and processes uplinked commands
     */
    void processUplink();

    /**
     * @brief The radio module instance
     */
    RFM96 radio = new Module(constants::radio::radio_cs_pin, constants::radio::radio_di0_pin,
                             constants::radio::radio_rst_pin, constants::radio::radio_busy_pin);

    /**
     * @brief The received command
     */
    uint8_t *received;

    /**
     * @brief RadioLib status code
     */
    int16_t code;
};

#endif
