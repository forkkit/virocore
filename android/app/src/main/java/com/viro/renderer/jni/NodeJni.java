/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viro.renderer.jni;

import android.content.Context;

/**
 * Java JNI wrapper for linking the following classes below across the bridge.
 *
 * Java JNI Wrapper     : com.viro.renderer.NodeJni.java
 * Cpp JNI wrapper      : Node_JNI.cpp
 * Cpp Object           : VRONode.cpp
 */
public class NodeJni {
    final protected long mNativeRef;

    public NodeJni(Context context) {
        mNativeRef = nativeCreateNode();
    }

    public void destroyNode(){
        nativeDestroyNode(mNativeRef);
    }

    public void addChildNode(NodeJni childNode){
        nativeAddChildNode(mNativeRef, childNode.mNativeRef);
    }

    public void removeChildNode(NodeJni childNode){
        nativeRemoveFromParent(childNode.mNativeRef);
    }

    public void setPosition(float[] position){
        if (position.length != 3){
            throw new IllegalArgumentException("Missing a position coordinate: All three coordinates are needed [x,y,z]");
        }

        nativeSetPosition(mNativeRef, position[0], position[1], position[2]);
    }

    public void setRotation(float[] rotation){
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation coordinate: All three coordinates are needed [x,y,z]");
        }
        nativeSetRotation(mNativeRef, rotation[0], rotation[1], rotation[2]);
    }

    public void setScale(float[] scale){
        if (scale.length != 3){
            throw new IllegalArgumentException("Missing a scale coordinate: All three coordinates are needed [x,y,z]");
        }
        nativeSetScale(mNativeRef, scale[0], scale[1], scale[2]);
    }

    public void setOpacity(float opacity){
        nativeSetOpacity(mNativeRef, opacity);
    }

    public void setVisible(boolean visible){
        nativeSetVisible(mNativeRef, visible);
    }

    public void setGeometry(BaseGeometry geometry){
        if (geometry == null){
            throw new IllegalArgumentException("Missing Required geometry to be set on Node.");
        }
        nativeSetGeometry(mNativeRef, geometry.mNativeRef);
    }

    private native long nativeCreateNode();
    private native void nativeDestroyNode(long nodeReference);
    private native void nativeAddChildNode(long nodeReference, long childNodeReference);
    private native void nativeRemoveFromParent(long nodeReference);
    private native void nativeSetPosition(long nodeReference, float x, float y, float z);
    private native void nativeSetRotation(long nodeReference, float x, float y, float z);
    private native void nativeSetScale(long nodeReference, float x, float y, float z);
    private native void nativeSetOpacity(long nodeReference, float opacity);
    private native void nativeSetVisible(long nodeReference, boolean visible);
    private native void nativeSetGeometry(long nodeReference, long geometryReference);
}