//
//  VRONode.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRONode_h
#define VRONode_h

#include <memory>
#include <stack>
#include <vector>
#include <string>
#include "VROMatrix4f.h"
#include "VROQuaternion.h"
#include "VRORenderContext.h"
#include "VRORenderParameters.h"
#include "VROAnimatable.h"

class VROGeometry;
class VROLight;
class VROAction;

class VRONode : public VROAnimatable {
    
public:
    
    /*
     Designated initializer for nodes in the model tree.
     */
    VRONode(const VRORenderContext &context);
    
    /*
     Copy constructor. This copies the node but *not* the underlying
     geometries or lights. Instead, these are shared by reference with the
     copied node. Additionally, this constructor will not copy child nodes.
     
     To copy child nodes recursively, invoke the clone() function.
     */
    VRONode(const VRONode &node);
    
    /*
     Designated initializer for nodes in the presentation tree.
     */
    VRONode();
    virtual ~VRONode();
    
    /*
     Copy constructor that recursively copies all child nodes. This copies
     the node but *not* the underlying geometries or lights. Instead, these
     are shared by reference with the copied node.
     */
    std::shared_ptr<VRONode> clone();
    
    void render(const VRORenderContext &context,
                VRORenderParameters &params);
    
    void setGeometry(std::shared_ptr<VROGeometry> geometry) {
        _geometry = geometry;
    }
    std::shared_ptr<VROGeometry> getGeometry() const {
        return _geometry;
    }
    
    VROMatrix4f getTransform() const;
    
    void setRotation(VROQuaternion rotation);
    void setPosition(VROVector3f position);
    void setScale(VROVector3f scale);
    
    void setLight(std::shared_ptr<VROLight> light) {
        _light = light;
    }
    std::shared_ptr<VROLight> getLight() {
        return _light;
    }
    
    void addChildNode(std::shared_ptr<VRONode> node) {
        _subnodes.push_back(node);
        node->_supernode = std::static_pointer_cast<VRONode>(shared_from_this());
    }
    void removeFromParentNode() {
        std::vector<std::shared_ptr<VRONode>> parentSubnodes = _supernode->_subnodes;
        parentSubnodes.erase(
                             std::remove_if(parentSubnodes.begin(), parentSubnodes.end(),
                                            [this](std::shared_ptr<VRONode> layer) {
                                                return layer.get() == this;
                                            }), parentSubnodes.end());
        _supernode.reset();
    }
    
    void runAction(std::shared_ptr<VROAction> action);
    void removeAction(std::shared_ptr<VROAction> action);
    void removeAllActions();
    
protected:
    
    /*
     The node's parent and children.
     */
    std::vector<std::shared_ptr<VRONode>> _subnodes;
    std::shared_ptr<VRONode> _supernode;
    
private:
    
    std::shared_ptr<VROGeometry> _geometry;
    std::shared_ptr<VROLight> _light;
    
    VROVector3f _scale;
    VROVector3f _position;
    VROQuaternion _rotation;
    
    /*
     The 'presentation' counterpart of this node. The presentation node
     reflects what's actually on the screen during an animation, as opposed
     to the model.
     */
    std::shared_ptr<VRONode> _presentationNode;
    
    /*
     Active actions on this node.
     */
    std::vector<std::shared_ptr<VROAction>> _actions;
    
    /*
     Action processing: execute all current actions and remove those that are
     expired.
     */
    void processActions();
    
    /*
     Render functions.
     */
    void pushTransforms(VRORenderParameters &params);
    void renderNode(const VRORenderContext &context, VRORenderParameters &params);
    void popTransforms(VRORenderParameters &params);

};

#endif /* VRONode_h */
