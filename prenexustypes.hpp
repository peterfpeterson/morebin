#ifndef _PRENEXUSTYPES_HPP
#define _PRENEXUSTYPES_HPP 1

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

namespace prenexus { // need a single place to include for these definitions
#pragma pack(push, 4) //Make sure the structure is 8 bytes.
struct DasEvent
{
  /// Time of flight.
  uint32_t tof;
  /// Pixel identifier as published by the DAS/DAE/DAQ.
  uint32_t pid;
};
#pragma pack(pop)

#pragma pack(push, 4) //Make sure the structure is 16 bytes.
struct Pulse
{
  /// The number of nanoseconds since the seconds field. This is not necessarily less than one second.
  uint32_t nanoseconds;

  /// The number of seconds since January 1, 1990.
  uint32_t seconds;

  /// The index of the first event for this pulse.
  uint64_t event_index;

  /// The proton charge for the pulse.
  double pCurrent;
};
#pragma pack(pop)

#pragma pack(push, 4) //Make sure the structure is the correct number of bytes.
struct OldPulse
{
  /// The number of nanoseconds since the seconds field. This is not necessarily less than one second.
  uint32_t nanoseconds;

  /// The number of seconds since January 1, 1990.
  uint32_t seconds;

  /// The index of the first event for this pulse.
  uint64_t event_index;
};
#pragma pack(pop)

#pragma pack(push, 4) //Make sure the structure is the correct number of bytes.
struct Rtdl
{
  /// The number of nanoseconds since the seconds field. This is not necessarily less than one second.
  uint32_t nanoseconds;

  /// The number of seconds since January 1, 1990.
  uint32_t seconds;

  uint32_t pulseType;
  uint32_t vetoStatus;
  uint32_t pulseCurrent;
  uint32_t spare;
};
#pragma pack(pop)

} // namespace prenexus

#endif
