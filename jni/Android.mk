LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# OpenCV
include C:\Development\android\opencv-2.4.9\sdk\native\jni\OpenCV.mk

# DOLPHIN
LOCAL_MODULE    := dolphin
LOCAL_SRC_FILES := imgproc_HomomorphicFilter.c\
				   imgproc_FrameDiff.c\
				   imgproc_SkinDetect.c\
				   standardlibrary_GlColorSpace.c\
				   utils.c
				   
LOCAL_LDLIBS    := -ljnigraphics

include $(BUILD_SHARED_LIBRARY)