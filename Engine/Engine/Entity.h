#pragma once

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Object.h"

class Transform;
class Component;
class Scene;

class Entity final : public Object
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Entity, Object)

private:
    Scene* ownerScene = nullptr;
    Transform transform;

    std::list<Component*> components;
    std::list<Component*> componentsToAdd;
    std::list<Component*> componentsToRemove;

protected:
    Entity() = default;
    Entity(std::string _guid) : Object(_guid) { }
    ~Entity() override = default;
    
    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON& node) override;

    void Update();
    void PreUpdate();
    void PostUpdate();

public:
    bool HasComponent(const std::string& componentName) const;
    void AddComponents(const std::vector<std::string>& componentList);
    Component* GetComponent(const std::string& componentName) const;
    Component* CreateComponent(const std::string& componentName);
    bool RemoveComponent(const Component* component);
    std::list<Component*> GetComponents() const { return components; }
    Component* GetComponentByUiD(STRCODE uid);

    Transform& GetTransform() { return transform; }

    Scene* GetParentScene() const;

    friend class Scene;
};

#endif // !_ENTITY_H_