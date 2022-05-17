#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

class EditAnimationsGui :
    public IGuiObject
{
public:
    EditAnimationsGui(Entity entityFor);

    virtual void CreateGui() override;

private:
    std::string selectedAnimation;
    unsigned int selectedFrameIndex;
    int inputFrameNo;
    char inputNewAnimationName[512];

    Entity entity;
};

