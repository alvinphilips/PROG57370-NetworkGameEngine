#pragma once

#include "Component.h"
#include "EngineMath.h"  

/**
 * @class Transform
 *
 * The Transform class represents the position, rotation, and scale of an entity in the game world.
 * It is a Component that can be attached to an Entity to define its spatial properties.
 */

class Transform : public Component {
    DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component);
public:

     /**
     * @brief Default constructor for the Transform class.
     */
    Transform();

    /**
     * @brief Initializes the Transform component.
     * This function is called during the initialization of the component.
     */
    void Initialize() override;

    /**
     * @brief Updates the Transform component's state.
     * This function is called every frame to update the component's state.
     */
    void Update() override;

    /**
     * @brief Loads Transform data from JSON.
     *
     * @param node The JSON object containing the Transform data.
     */
    void Load(json::JSON& node) override;

    //xyz properties, angle in degrees or radians
    Vec2 position; ///< The position of the entity in the game world.
    float rotation; ///< The rotation of the entity, in degrees or radians.
    Vec2 scale; ///< The scale of the entity.

    
    /**
     * @brief Translates the entity by a given vector.
     *
     * @param delta The vector by which to translate the entity.
     */
    void Translate(const Vec2& delta);

    /**
     * @brief Rotates the entity by a given amount.
     *
     * @param delta The amount by which to rotate the entity.
     */
    void Rotate(float delta);

    /**
    * @brief Scales the entity by a given vector.
    *
    * @param delta The vector by which to scale the entity.
    */
    void Scale(const Vec2& delta);
};