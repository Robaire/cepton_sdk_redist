/*
  Copyright Cepton Technologies Inc. 2017, All rights reserved.

  Cepton Sensor SDK C++ interface.
*/
#pragma once

#include "cepton_sdk.h"

#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace cepton_sdk {

//------------------------------------------------------------------------------
// Errors
//------------------------------------------------------------------------------
/// Error code returned by most sdk functions.
/**
 * Most getter functions do not return an error, because they cannot fail.
 */
typedef CeptonSensorErrorCode SensorErrorCode;

/// Returns string name of error code.
/**
 * Returns empty string if error code is invalid.
 */
static std::string get_error_code_name(SensorErrorCode error_code) {
  return cepton_get_error_code_name(error_code);
}

static bool is_error_code(SensorErrorCode error_code) {
  return (bool)cepton_is_error_code(error_code);
}

static bool is_fault_code(SensorErrorCode error_code) {
  return (bool)cepton_is_fault_code(error_code);
}

/// Type checking for error callback data. Currently, not used.
/**
 * If specified type is correct, returns pointer to data, otherwise returns
 * nullptr.
 */
template <typename T>
const T *get_error_data(SensorErrorCode error_code, const void *error_data,
                        std::size_t error_data_size) {
  if (error_data_size == 0) {
    return nullptr;
  }

  switch (error_code) {
    default:
      return nullptr;
  }

  return dynamic_cast<const T *>(error_data);
}

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
/// Sensor identifier.
typedef CeptonSensorHandle SensorHandle;

/// Indicates that handle was generated by capture replay.
static const SensorHandle SENSOR_HANDLE_FLAG_MOCK =
    CEPTON_SENSOR_HANDLE_FLAG_MOCK;

typedef CeptonSensorModel SensorModel;

typedef CeptonSensorInformation SensorInformation;

typedef CeptonSensorImagePoint SensorImagePoint;

//------------------------------------------------------------------------------
// SDK Setup
//------------------------------------------------------------------------------
/// SDK control flags.
typedef CeptonSDKControl Control;

/// Controls frequency of points being reported.
typedef CeptonSDKFrameMode FrameMode;

typedef CeptonSDKFrameOptions FrameOptions;

/// SDK initialization options.
typedef CeptonSDKOptions Options;

/// Create default frame options.
static FrameOptions create_frame_options() {
  return cepton_sdk_create_frame_options();
}

/// Create default options.
static Options create_options() { return cepton_sdk_create_options(); }

/// Callback for receiving sdk and sensor errors.
/**
 * Currently, error_data is not used.
 */
typedef void (*FpSensorErrorCallback)(SensorHandle handle,
                                      SensorErrorCode error_code,
                                      const char *error_msg,
                                      const void *error_data,
                                      size_t error_data_size, void *user_data);

static bool is_initialized() { return (bool)cepton_sdk_is_initialized(); }

/// Initializes settings and networking.
/**
 * Must be called before any other sdk function listed below.
 */
static SensorErrorCode initialize(int ver, const Options &options,
                                  const FpSensorErrorCallback &cb,
                                  void *const user_data) {
  return cepton_sdk_initialize(ver, &options, cb, user_data);
}

/// Resets everything and deallocates memory.
/**
 * Called automatically on program exit.
 */
static SensorErrorCode deinitialize() { return cepton_sdk_deinitialize(); }

static SensorErrorCode set_control_flags(Control mask, Control flags) {
  return cepton_sdk_set_control_flags(mask, flags);
}

static Control get_control_flags() { return cepton_sdk_get_control_flags(); }

static bool has_control_flag(Control flag) {
  return (bool)cepton_sdk_has_control_flag(flag);
}

/// Sets network listen port.
/**
 * Default: 8808.
 */
static SensorErrorCode set_port(uint16_t port) {
  return cepton_sdk_set_port(port);
}

static uint16_t get_port() { return cepton_sdk_get_port(); }

static SensorErrorCode set_frame_options(const CeptonSDKFrameOptions &options) {
  return cepton_sdk_set_frame_options(&options);
}

static FrameMode get_frame_mode() { return cepton_sdk_get_frame_mode(); }

static float get_frame_length() { return cepton_sdk_get_frame_length(); }

/// Reset frame caches.
/**
 * Used when seeking in a capture file.
 */
static SensorErrorCode clear_cache() { return cepton_sdk_clear_cache(); }

//------------------------------------------------------------------------------
// Points
//------------------------------------------------------------------------------
/// Callback for receiving image points.
/**
 * Set the frame length to control the callback rate.
 */
typedef void (*FpSensorImageDataCallback)(SensorHandle handle,
                                          std::size_t n_points,
                                          const SensorImagePoint *c_points,
                                          void *user_data);

/// Sets image frames callback.
/**
 * Returns error if callback already registered.
 */
static SensorErrorCode listen_image_frames(FpSensorImageDataCallback cb,
                                           void *const user_data) {
  return cepton_sdk_listen_image_frames(cb, user_data);
}

static SensorErrorCode unlisten_image_frames() {
  return cepton_sdk_unlisten_image_frames();
}

//------------------------------------------------------------------------------
// Sensors
//------------------------------------------------------------------------------
/**
 * Get number of sensors attached.
 * Use to check for new sensors. Sensors are not deleted until deinitialization.
 */
static std::size_t get_n_sensors() { return cepton_sdk_get_n_sensors(); }

/**
 * Returns error if sensor not found.
 */
static SensorErrorCode get_sensor_handle_by_serial_number(
    uint64_t serial_number, SensorHandle &handle) {
  return cepton_sdk_get_sensor_handle_by_serial_number(serial_number, &handle);
}

/**
 * Valid indices are in range [0, n_sensors).
 * Returns error if index invalid.
 */
static SensorErrorCode get_sensor_information_by_index(
    std::size_t idx, SensorInformation &info) {
  return cepton_sdk_get_sensor_information_by_index(idx, &info);
}

/**
 * Returns error if sensor not found.
 */
static SensorErrorCode get_sensor_information(SensorHandle handle,
                                              SensorInformation &info) {
  return cepton_sdk_get_sensor_information(handle, &info);
}

//------------------------------------------------------------------------------
// Networking
//------------------------------------------------------------------------------
/// Callback for receiving network packets.
/**
 * \param handle Unique sensor identifier (e.g. IP address).
 * Returns error if callback already set.
 */
typedef void (*FpNetworkReceiveCallback)(SensorHandle handle,
                                         uint8_t const *buffer,
                                         size_t buffer_size, void *user_data);

/// Sets network packets callback.
/**
 * Only 1 callback can be registered.
 */
static SensorErrorCode listen_network_packets(FpNetworkReceiveCallback cb,
                                              void *const user_data) {
  return cepton_sdk_listen_network_packet(cb, user_data);
}

static SensorErrorCode unlisten_network_packets() {
  return cepton_sdk_unlisten_network_packet();
}

/// Sets current sdk time used for mocked sensors (unix time [microseconds]).
/**
 * If 0, uses pc clock. Set automatically by capture replay.
 */
static SensorErrorCode set_mock_time_base(int64_t time_base) {
  return cepton_sdk_set_mock_time_base(time_base);
}

/// Manually passes packets to sdk.
/**
 * Blocks while processing, and calls listener callbacks synchronously before
 * returning.
 */
static SensorErrorCode mock_network_receive(SensorHandle handle,
                                            const uint8_t *const buffer,
                                            std::size_t buffer_size) {
  return cepton_sdk_mock_network_receive(handle, buffer, buffer_size);
}
//------------------------------------------------------------------------------
// Capture Replay
//------------------------------------------------------------------------------
namespace capture_replay {

static bool is_open() { return (bool)cepton_sdk_capture_replay_is_open(); }

/// Opens capture file.
/**
 * Must be called before any other replay functions listed below.
 */
static SensorErrorCode open(const std::string &path) {
  return cepton_sdk_capture_replay_open(path.c_str());
}

static SensorErrorCode close() { return cepton_sdk_capture_replay_close(); }

/// Returns capture start timestamp (unix time [microseconds]).
static uint64_t get_start_time() {
  return cepton_sdk_capture_replay_get_start_time();
}

/// Returns capture file position [seconds].
static float get_position() { return cepton_sdk_capture_replay_get_position(); }

/// Returns capture file time (unix time [microseconds])
static uint64_t get_time() {
  return get_start_time() + uint64_t(1e6f * get_position());
}

/// Returns capture file length [seconds].
static float get_length() { return cepton_sdk_capture_replay_get_length(); }

/// Returns true if at end of capture file.
/**
 * This is only relevant when using `resume_blocking` methods.
 */
static bool is_end() { return (bool)cepton_sdk_capture_replay_is_end(); }

/// Seek to start of capture file.
static SensorErrorCode rewind() { return cepton_sdk_capture_replay_rewind(); }

/// Seek to capture file position [seconds].
/**
 * Position must be in range [0.0, capture length).
 * Returns error if position is invalid.
 */
static SensorErrorCode seek(float position) {
  return cepton_sdk_capture_replay_seek(position);
}

/// Seek to relative capture file position [seconds].
/**
 * Returns error if position is invalid.
 */
static SensorErrorCode seek_relative(float position) {
  position += get_position();
  return cepton_sdk_capture_replay_seek(position);
}

/// If enabled, replay will automatically rewind at end.
static SensorErrorCode set_enable_loop(bool value) {
  return cepton_sdk_capture_replay_set_enable_loop((int)value);
}

static bool get_enable_loop() {
  return (bool)cepton_sdk_capture_replay_get_enable_loop();
}

/// Replay speed multiplier for asynchronous replay.
static SensorErrorCode set_speed(float speed) {
  return cepton_sdk_capture_replay_set_speed(speed);
}

static float get_speed() { return cepton_sdk_capture_replay_get_speed(); }

/// Replay next packet in current thread without sleeping.
/**
 * Pauses replay thread if it is running.
 */
static SensorErrorCode resume_blocking_once() {
  return cepton_sdk_capture_replay_resume_blocking_once();
}

/// Replay multiple packets synchronously.
/**
 * No sleep between packets. Resume duration must be non-negative.
 * Pauses replay thread if it is running.
 */
static SensorErrorCode resume_blocking(float duration) {
  return cepton_sdk_capture_replay_resume_blocking(duration);
}

/// Returns true if replay thread is running.
static bool is_running() { return cepton_sdk_capture_replay_is_running(); }

// Resumes asynchronous replay thread.
/**
 * Packets are replayed in realtime. Replay thread sleeps in between packets.
 */
static SensorErrorCode resume() { return cepton_sdk_capture_replay_resume(); }

/// Pauses asynchronous replay thread.
static SensorErrorCode pause() { return cepton_sdk_capture_replay_pause(); }

}  // namespace capture_replay
}  // namespace cepton_sdk
