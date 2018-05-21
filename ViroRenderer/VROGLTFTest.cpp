//
//  VROGLTFTest.cpp
//  ViroKit
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#include "VROGLTFTest.h"
#include "VROTestUtil.h"

VROGLTFTest::VROGLTFTest() :
    VRORendererTest(VRORendererTestType::GLTF) {
    _angle = 0;
}

VROGLTFTest::~VROGLTFTest() {
    
}

void VROGLTFTest::build(std::shared_ptr<VRORenderer> renderer,
                       std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                       std::shared_ptr<VRODriver> driver) {

    _driver = driver;

    VROGLTFModel duck("Duck", "gltf", { 0, -1.5, -5 }, { 1, 1, 1 }, 1, "");
    VROGLTFModel buggy("Buggy", "glb", { -0.75, -1.5, -5 }, { 0.03, 0.03, 0.03 }, 1, "");
    _models.push_back(duck);
    _models.push_back(buggy);

    _sceneController = std::make_shared<VROARSceneController>();
    std::shared_ptr<VROScene> scene = _sceneController->getScene();
    


    std::shared_ptr<VROTexture> environment = VROTestUtil::loadRadianceHDRTexture("ibl_mans_outside");
    //std::shared_ptr<VROTexture> environment = VROTestUtil::loadRadianceHDRTexture("ibl_ridgecrest_road");
    //std::shared_ptr<VROTexture> environment = VROTestUtil::loadRadianceHDRTexture("ibl_wooden_door");

    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->setPosition({0, 0, 0});

#if VRO_PLATFORM_ANDROID
    std::shared_ptr<VROLight> light = std::make_shared<VROLight>(VROLightType::Spot);
    light->setColor({ 1.0, 1.0, 1.0 });
    light->setPosition( { 0, 10, 10 });
    light->setDirection( { 0, -1.0, -1.0 });
    light->setAttenuationStartDistance(25);
    light->setAttenuationEndDistance(50);
    light->setSpotInnerAngle(35);
    light->setSpotOuterAngle(60);
    light->setCastsShadow(true);
    light->setIntensity(1000);

    std::shared_ptr<VROLight> ambient = std::make_shared<VROLight>(VROLightType::Ambient);
    ambient->setColor({ 1.0, 1.0, 1.0 });
    ambient->setIntensity(100);

    rootNode->addLight(light);
    rootNode->addLight(ambient);
#else
    rootNode->setLightingEnvironment(environment);
#endif
    rootNode->setBackgroundSphere(environment);

    _gltfContainerNode = std::make_shared<VRONode>();
    rootNode->addChildNode(_gltfContainerNode);

    _gltfIndex = 0;
    rotateModel();
    
    /*
     Shadow surface.
     */
    std::shared_ptr<VROSurface> surface = VROSurface::createSurface(80, 80);
    surface->setName("Surface");
    surface->getMaterials().front()->setLightingModel(VROLightingModel::Lambert);
    VROARShadow::apply(surface->getMaterials().front());
    
    std::shared_ptr<VRONode> surfaceNode = std::make_shared<VRONode>();
    surfaceNode->setGeometry(surface);
    surfaceNode->setRotationEuler({ -M_PI_2, 0, 0 });
    surfaceNode->setPosition({0, -6, -6});
    surfaceNode->setLightReceivingBitMask(1);
    rootNode->addChildNode(surfaceNode);
    
    std::shared_ptr<VRONodeCamera> camera = std::make_shared<VRONodeCamera>();
    camera->setRotationType(VROCameraRotationType::Orbit);
    camera->setOrbitFocalPoint({ 0, 0, -3});
    
    std::shared_ptr<VRONode> cameraNode = std::make_shared<VRONode>();
    cameraNode->setCamera(camera);
    rootNode->addChildNode(cameraNode);
    
    _pointOfView = cameraNode;
    
    _eventDelegate = std::make_shared<VROGLTFEventDelegate>(this);
    _eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnClick, true);
    rootNode->setEventDelegate(_eventDelegate);
}

void VROGLTFTest::rotateModel() {
    VROGLTFModel model = _models[_gltfIndex];
    std::shared_ptr<VRONode> gltfNode = VROTestUtil::loadGLTFModel(model.name, model.ext,
                                                                   model.position, model.scale,
                                                                   model.lightMask, model.animation, _driver);
    _gltfContainerNode->removeAllChildren();
    _gltfContainerNode->addChildNode(gltfNode);

    _gltfIndex = (_gltfIndex + 1) % _models.size();
}

void VROGLTFEventDelegate::onClick(int source, std::shared_ptr<VRONode> node, ClickState clickState,
                                  std::vector<float> position) {
    if (clickState == ClickState::Clicked) {
        _test->rotateModel();
    }
}