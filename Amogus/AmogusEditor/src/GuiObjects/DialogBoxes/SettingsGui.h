#pragma once

#include "../IGuiObject.h"

class SettingsGui :
    public IGuiObject
{
public:
    SettingsGui(ImGuiLayer* layer);

    virtual void CreateGui() override;

private:
    void FontSelect();
    void FontSizeSelect();
    void ConsoleTextColourSelect();

    void ExportToFile();
};