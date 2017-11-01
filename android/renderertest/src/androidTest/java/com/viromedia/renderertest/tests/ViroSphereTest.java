/*
 * Copyright (c) 2017-present, ViroMedia, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the LICENSE file in the
 * root directory of this source tree. An additional grant of patent rights can be found in
 * the PATENTS file in the same directory.
 */

package com.viromedia.renderertest.tests;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.util.Log;

import com.viro.renderer.jni.AmbientLight;
import com.viro.renderer.jni.Material;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.Sphere;
import com.viro.renderer.jni.Texture;
import com.viro.renderer.jni.Vector;

import org.junit.Test;

import java.util.Arrays;

/**
 * Created by vadvani on 10/29/17.
 */

public class ViroSphereTest extends ViroBaseTest {
    private Sphere mSphereOne;
    private Sphere mSphereTwo;
    public ViroSphereTest() {
        super();
        mSphereOne = null;
        mSphereTwo = null;
    }

    @Override
    void configureTestScene() {
        AmbientLight ambientLightJni = new AmbientLight(Color.WHITE, 1000.0f);
        mScene.getRootNode().addLight(ambientLightJni);

        // Creation of first sphere using radius constructor.
        Material material = new Material();
        Bitmap bobaBitmap = this.getBitmapFromAssets(mActivity, "boba.png");
        Texture bobaTexture = new Texture(bobaBitmap, Texture.TextureFormat.RGBA8, true, true);
        material.setDiffuseTexture(bobaTexture);
        material.setDiffuseColor(Color.RED);
        material.setLightingModel(Material.LightingModel.BLINN);

        Node node = new Node();
        mSphereOne = new Sphere(5);
        node.setGeometry(mSphereOne);
        float[] spherePosition = {-6.5f,0,-15};
        node.setPosition(new Vector(spherePosition));
        mSphereOne.setMaterials(Arrays.asList(material));
        mScene.getRootNode().addChildNode(node);

        //Create of second sphere using other constructor.
        Material materialBlue = new Material();
        materialBlue.setDiffuseColor(Color.BLUE);
        materialBlue.setLightingModel(Material.LightingModel.BLINN);

        Node nodeTwo = new Node();
        mSphereTwo =  new Sphere(6, 10, 10,true);
        nodeTwo.setGeometry(mSphereTwo);
        float[] sphereTwoPosition = {6.5f,0,-15};
        nodeTwo.setPosition(new Vector(sphereTwoPosition));
        mSphereTwo.setMaterials(Arrays.asList(materialBlue));
        mScene.getRootNode().addChildNode(nodeTwo);
    }

    @Test
    public void sphereTest() {
        Log.i("ViroSphereTest", "sphereTest beginning");
        sphereHeightSegmentCount();
        sphereWidthSegmentCount();
        sphereRadiusCount();
        Log.i("ViroSphereTest", "sphereTest middle");
        sphereFaceOutward();
        Log.i("ViroSphereTest", "sphereTest end");
    }


    public void sphereHeightSegmentCount() {
        mMutableTestMethod = () -> {
            if(mSphereTwo != null  && mSphereTwo.getHeightSegmentCount() < 30) {
                mSphereTwo.setHeightSegmentCount(mSphereTwo.getHeightSegmentCount() +1);
            }
        };
        Log.i("ViroSphereTest", "sphereHeightSegmentCount");
        assertPass("Blue sphere changed in height segment from 10 to 30");
    }

    public void sphereWidthSegmentCount() {
        mMutableTestMethod = () -> {
            if(mSphereTwo != null  && mSphereTwo.getWidthSegmentCount() < 30) {
                mSphereTwo.setWidthSegmentCount(mSphereTwo.getWidthSegmentCount() +1);
            }
        };
        Log.i("ViroSphereTest", "sphereWidthSegmentCount");
        assertPass("Blue sphere changed in width segment from 10 to 30");
    }

    public void sphereRadiusCount() {
        mMutableTestMethod = () -> {
            if(mSphereOne != null  && mSphereOne.getRadius() < 5) {
                mSphereOne.setRadius(mSphereOne.getRadius() +1);
            }
        };
        assertPass("Boba sphere changed in radius segment from 5 to 10");
    }

    public void sphereFaceOutward() {
        mSphereOne.setFacesOutward(false);
        assertPass("Boba sphere flipped face outward to false.");
    }
}