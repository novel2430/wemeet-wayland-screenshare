#pragma once

#include <cstdio>

#include <stdexcept>
#include <dlfcn.h>
#include <opencv4/opencv2/core/types_c.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>

struct OpencvDLFCNSingleton{

  using THIS_CLASS = OpencvDLFCNSingleton;

private:
  OpencvDLFCNSingleton(){
    libopencv_core_handle = dlopen("libopencv_core.so", RTLD_LOCAL|RTLD_LAZY);
    if(libopencv_core_handle == nullptr){
      throw std::runtime_error("Failed to open library libopencv_core.so");
    }
    libopencv_imgproc_handle = dlopen("libopencv_imgproc.so", RTLD_LOCAL|RTLD_LAZY);
    if(libopencv_imgproc_handle == nullptr){
      throw std::runtime_error("Failed to open library libopencv_imgproc.so");
    }
  }

public:

  inline static OpencvDLFCNSingleton& getSingleton(){
    static OpencvDLFCNSingleton singleton;
    return singleton;
  }

  ~OpencvDLFCNSingleton(){
    if (libopencv_core_handle != nullptr){
      dlclose(libopencv_core_handle);
    }
    if (libopencv_imgproc_handle != nullptr){
      dlclose(libopencv_imgproc_handle);
    }
  }

  static CvMat* cvInitMatHeader(
    CvMat* mat, int rows, int cols,
    int type, void* data = NULL,
    int step = CV_AUTOSTEP
  ){
    using FType = CvMat*(CvMat*, int, int, int, void*, int);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvInitMatHeader");
    return func(mat, rows, cols, type, data, step);
  }

  static CvMat* cvCreateMat(
    int rows, int cols, int type
  ){
    using FType = CvMat*(int, int, int);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvCreateMat");
    return func(rows, cols, type);
  }
  
  static void cvReleaseMat(CvMat** mat){
    using FType = void(CvMat**);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvReleaseMat");
    return func(mat);
  }

  static CvMat* cvGetSubRect(
    const CvArr* arr, CvMat* submat, CvRect rect
  ){
    using FType = CvMat*(const CvArr*, CvMat*, CvRect);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvGetSubRect");
    return func(arr, submat, rect);
  }

  static void cvResize(
    const CvArr* src, CvArr* dst,
    int interpolation
  ){
    using FType = void(const CvArr*, CvArr*, int);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_imgproc_handle, "cvResize");
    return func(src, dst, interpolation);
  }

  static void cvSetZero(CvArr* arr){
    using FType = void(CvArr*);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvSetZero");
    return func(arr);
  }

  static void cvCopy(const CvArr* src, CvArr* dst){
    using FType = void(const CvArr*, CvArr*);
    auto& singleton = getSingleton();
    auto func = (FType*)dlsym(singleton.libopencv_core_handle, "cvCopy");
    return func(src, dst);
  }


  void* libopencv_core_handle{nullptr};
  void* libopencv_imgproc_handle{nullptr};


};