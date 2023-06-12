#pragma once

#include <string>

#define DECIBEL_MINIMUM -60.f // Equal to -Inf dB

#define MAX_VOICES 8
#define NUM_LANES 2

// Tempo
#define BPM_PARAM_ID "BPM"
#define BPM_PARAM_NAME "Tempo"
constexpr float BPM_MIN = 20.f;
constexpr float BPM_MAX = 300.f;

// Global parameters
#define GLOBAL_VOLUME_ID "globalVolume"
#define GLOBAL_VOLUME_NAME "Global Volume"
constexpr float GLOBAL_VOLUME_MIN = -30.f;
constexpr float GLOBAL_VOLUME_MAX =   6.f;

// Lane parameters
inline std::string getLaneParameter(const std::string& prefix, const int laneIndex)
{
	return prefix + std::to_string(laneIndex);
}

#define LANE_VOLUME_ID "laneVolume"
#define LANE_VOLUME_NAME "Lane Volume "

#define LANE_SOUND_SELECT_ID "laneSoundSelect"
#define LANE_SOUND_SELECT_NAME "Sound Selection "

#define LANE_PLUCK_ID "lanePluck"
#define LANE_PLUCK_NAME "Pluck "

#define LANE_RELEASE_ID "laneRelease"
#define LANE_RELEASE_NAME "Release "

#define LANE_REVERB_ID "laneReverb"
#define LANE_REVERB_NAME "Reverb "