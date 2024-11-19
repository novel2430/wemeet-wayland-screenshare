#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <mutex>

#include "format.hpp"



struct FrameBuffer {

  FrameBuffer() = default;
  inline FrameBuffer(
    uint32_t height,
    uint32_t width,
    SpaVideoFormat_e const& format
  ){
    update_param(height, width, format);
  }
  
  inline void update_param(
    uint32_t height,
    uint32_t width,
    SpaVideoFormat_e const& format
  ){

    int bytes_per_pixel = spa_videoformat_bytesize(format);
    if (bytes_per_pixel == -1) {
      throw std::runtime_error("Invalid format");
    }

    // always store in (height, width):(stride, 1) layout
    uint32_t needed_stride = (width * bytes_per_pixel + 4 - 1) / 4 * 4;
    uint32_t needed_size = height * needed_stride;
    if (needed_size > data_size || data.get() == nullptr) {
      data_size = needed_size;
      data.reset(new uint8_t[data_size]);
    }
    this->height = height;
    this->width = width;
    this->row_byte_stride = needed_stride;
    this->format = format;
  }

  std::unique_ptr<uint8_t[]> data{nullptr};
  size_t data_size{0};
  uint32_t height{0};
  uint32_t width{0};
  uint32_t row_byte_stride{0};
  SpaVideoFormat_e format{SpaVideoFormat_e::INVALID};

};

// TODO: this framebuffer queue is WAY TOO INEFFICIENT right now
// we have to implement some double buffer'd version of this

struct SimpleZOHSingleFrameBufferQueue{

  SimpleZOHSingleFrameBufferQueue(
    uint32_t init_height,
    uint32_t init_width,
    SpaVideoFormat_e const& init_format
  ){
    framebuf.update_param(init_height, init_width, init_format);
  }

  inline bool try_acquire_write(){
    return true;
  }

  inline bool acquire_write(){
    mtx.lock();
    return true;
  }

  inline FrameBuffer& get_write(){
    return framebuf;
  }

  inline void release_write(){
    mtx.unlock();
    return;
  }

  inline bool try_acquire_read(){
    return true;
  }

  inline bool acquire_read(){
    mtx.lock();
    return true;
  }

  inline FrameBuffer& get_read(){
    return framebuf;
  }

  inline bool release_read(){
    mtx.unlock();
    return true;
  }

  std::mutex mtx;
  FrameBuffer framebuf;

};