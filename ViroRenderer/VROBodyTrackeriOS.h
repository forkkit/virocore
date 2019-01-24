//
//  VROBodyTrackeriOS.h
//  ViroKit
//
//  Created by vik.advani on 9/4/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROBodyTrackeriOS_h
#define VROBodyTrackeriOS_h

#include "VROBodyTracker.h"
#include <map>
#include "VROCameraTexture.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#include "VROMatrix4f.h"

class VRODriver;

class VROBodyTrackeriOS : public VROBodyTracker {
    
public:
    
    VROBodyTrackeriOS();
    virtual ~VROBodyTrackeriOS() {}
    
    bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
    void startBodyTracking();
    void stopBodyTracking();
    void update(const VROARFrame &frame);
    
private:
    
    MLModel *_model;
    VNCoreMLModel *_coreMLModel;
    VNCoreMLRequest *_coreMLRequest;
    double _lastTimestamp;
    int32_t _fps;
    dispatch_queue_t bodyMeshingQueue;
    
    VROMatrix4f _transform;
    static std::map<VROBodyJointType, std::vector<VROInferredBodyJoint>> convertHeatmap(MLMultiArray *heatmap, VROMatrix4f transform);

};

#endif


