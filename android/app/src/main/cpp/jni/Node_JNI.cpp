//
//  Node_JNI.cpp
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
//
#include <iostream>
#include <jni.h>
#include <memory>
#include "VROGeometry.h"
#include "VRONode.h"
#include "PersistentRef.h"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_viro_renderer_jni_NodeJni_##method_name

namespace Node{
    inline jlong jptr(std::shared_ptr<VRONode> shared_node) {
        PersistentRef<VRONode> *native_node = new PersistentRef<VRONode>(shared_node);
        return reinterpret_cast<intptr_t>(native_node);
    }

    inline std::shared_ptr<VRONode> native(jlong ptr) {
        PersistentRef<VRONode> *persistentNode = reinterpret_cast<PersistentRef<VRONode> *>(ptr);
        return persistentNode->get();
    }
}

extern "C" {

JNI_METHOD(jlong, nativeCreateNode)(JNIEnv *env,
                                        jclass clazz) {
   std::shared_ptr<VRONode> node = std::make_shared<VRONode>();
   return Node::jptr(node);
}

JNI_METHOD(void, nativeDestroyNode)(JNIEnv *env,
                                        jclass clazz,
                                        jlong native_node_ref) {
  delete reinterpret_cast<PersistentRef<VRONode> *>(native_node_ref);
}

JNI_METHOD(void, nativeAddChildNode)(JNIEnv *env,
                             jobject obj,
                             jlong native_node_ref,
                             jlong child_node_ref) {
    Node::native(native_node_ref)->addChildNode(Node::native(child_node_ref));
}

JNI_METHOD(void, nativeRemoveFromParent)(JNIEnv *env,
                             jobject obj,
                             jlong native_node_ref) {
    Node::native(native_node_ref)->removeFromParentNode();
}

JNI_METHOD(void, nativeSetPosition)(JNIEnv *env,
                                    jobject obj,
                                    jlong native_node_ref,
                                    jfloat positionX,
                                    jfloat positionY,
                                    jfloat positionZ) {
    Node::native(native_node_ref)->setPosition({positionX, positionY, positionZ});
}

JNI_METHOD(void, nativeSetRotation)(JNIEnv *env,
                                    jobject obj,
                                    jlong native_node_ref,
                                    jfloat rotationRadiansX,
                                    jfloat rotationRadiansY,
                                    jfloat rotationRadiansZ) {
    Node::native(native_node_ref)->setRotation({rotationRadiansX, rotationRadiansY, rotationRadiansZ});
}

JNI_METHOD(void, nativeSetScale)(JNIEnv *env,
                                    jobject obj,
                                    jlong native_node_ref,
                                    jfloat scaleX,
                                    jfloat scaleY,
                                    jfloat scaleZ) {
    Node::native(native_node_ref)->setScale({scaleX, scaleY, scaleZ});
}

JNI_METHOD(void, nativeSetOpacity)(JNIEnv *env,
                                 jobject obj,
                                 jlong native_node_ref,
                                 jfloat opacity) {
    Node::native(native_node_ref)->setOpacity(opacity);
}

JNI_METHOD(void, nativeSetVisible)(JNIEnv *env,
                                   jobject obj,
                                   jlong native_node_ref,
                                   jfloat opacity) {
    Node::native(native_node_ref)->setOpacity(opacity);
}

JNI_METHOD(void, nativeSetGeometry)(JNIEnv *env,
                                    jobject obj,
                                    jlong native_node_ref,
                                    jlong native_geo_ref) {
    PersistentRef<VROGeometry> *baseGeometryRef = reinterpret_cast<PersistentRef<VROGeometry> *>(native_geo_ref);
    std::shared_ptr<VROGeometry> baseGeometry = baseGeometryRef->get();
    Node::native(native_node_ref)->setGeometry(baseGeometry);
}

}  // extern "C"