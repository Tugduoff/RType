/*
** EPITECH PROJECT, 2024
** ConfigEditor
** File description:
** Gui.cpp file
*/

#include "Gui.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"
#undef GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION

#include <vector>

ConfigEditor::Gui::Gui() :
    __fullPath(""),
    __cfg(),
    __entityCount(0),
    __panelRec({ 45, 110, 1830, 880 }),
    __panelView({ 0 }),
    __panelScroll({ 99, -20 }),
    __exitWindow(false),
    __state(ConfigEditor::EditorState::CONFIG_NOT_LOADED),
    __selectedEntity(0),
    __selectedComponent(0),
    __selectedArgs(),
    __argsBuffer()
{
    InitWindow(1920, 1080, "Config Editor");

    ToggleFullscreen();
    SetExitKey(0);
    SetTargetFPS(60);

    std::string levelsPath = std::string(GetWorkingDirectory()) + "/config/levels";

    __fileDialogState = InitGuiWindowFileDialog(levelsPath.c_str());

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 200);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 150);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 250);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 200);
}

void ConfigEditor::Gui::close()
{
    CloseWindow();
}

void ConfigEditor::Gui::saveConfig() {
    if (__fullPath.empty()) {
        std::cerr << "No file selected to save!" << std::endl;
        return;
    }
    try {
        __cfg.setOption(libconfig::Config::OptionColonAssignmentForGroups, false);
        __cfg.setOption(libconfig::Config::OptionSemicolonSeparators, false);
        __cfg.writeFile(__fullPath.c_str());
        std::cerr << "Config file saved: " << __fullPath << std::endl;
    } catch (libconfig::FileIOException &e) {
        std::cerr << "Error while writing file: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error while writing file: " << e.what() << std::endl;
    }
}

void ConfigEditor::Gui::drawEntities()
{
    GuiScrollPanel(__panelRec, TextFormat("Entities: %d", __entityCount), (Rectangle){0, 0, 1810, 140 + 140 * (((__entityCount - 1) / 8)) }, &__panelScroll, &__panelView);

    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    if (GuiButton((Rectangle){ 1772.5, 112.5, 100, 20 }, "Add Entity")) {
        libconfig::Setting &entities = __cfg.getRoot()["entities"];
        libconfig::Setting &newEntity = entities.add(libconfig::Setting::TypeGroup);
        newEntity.add("name", libconfig::Setting::TypeString) = "New Entity";
        newEntity.add("spawnTime", libconfig::Setting::TypeInt) = 0;
        newEntity.add("type", libconfig::Setting::TypeString) = "Unknown";
        newEntity.add("components", libconfig::Setting::TypeList);
        __entityCount++;
    }

    if (__entityCount > 0) {
        BeginScissorMode(__panelView.x, __panelView.y, __panelView.width, __panelView.height);
            std::vector<size_t> indicesToDelete;
            size_t idx = 0;
            for (const auto &entity : __cfg.getRoot()["entities"]) {
                libconfig::Setting &components = entity["components"];
                int spawnTime = -1;
                std::string type = "Unknown";

                entity.lookupValue("spawnTime", spawnTime);
                entity.lookupValue("type", type);

                if (GuiButton((Rectangle){ 60 + (idx % 8) * 227.5 + __panelScroll.x, 170 + (140 * (idx / 8)) + __panelScroll.y, 200, 65 }, "")) {
                    __selectedEntity = idx;
                    __state = ConfigEditor::EditorState::ENTITY_DISPLAY;
                };
                GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
                GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
                GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
                GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
                GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);
                GuiPanel((Rectangle){ 60 + (idx % 8) * 227.5 + __panelScroll.x, 145 + (140 * (idx / 8)) + __panelScroll.y, 200, 91 }, entity["name"]);
                GuiLabel((Rectangle){ 68 + (idx % 8) * 227.5 + __panelScroll.x, 165 + (140 * (idx / 8)) + __panelScroll.y, 180, 70 }, TextFormat("Components: %d\nSpawn Time: %d\nTemplate: %s", components.getLength(), spawnTime, type.c_str()));
                if (GuiButton((Rectangle){ 60 + (idx % 8) * 227.5 + __panelScroll.x, 235 + (140 * (idx / 8)) + __panelScroll.y, 200, 30 }, "Edit")) {
                    __selectedEntity = idx;
                    __state = ConfigEditor::EditorState::ENTITY_EDIT;
                };
                GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
                GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0);
                GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0);
                if (GuiButton((Rectangle){ 60 + (idx % 8) * 227.5 + __panelScroll.x + 177.5, 145 + (140 * (idx / 8)) + __panelScroll.y + 2.5, 20, 20 }, "x")) {
                    indicesToDelete.push_back(idx);
                };
                idx++;
            }
            for (auto &idx : indicesToDelete) {
                __cfg.getRoot()["entities"].remove(idx);
                __entityCount--;
            }
        EndScissorMode();
    }
}

void ConfigEditor::Gui::drawComponentFromEntity()
{
    if (__selectedEntity >= __entityCount) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        return;
    }
    if (__selectedComponent >= __cfg.getRoot()["entities"][__selectedEntity]["components"].getLength()) {
        __state = ConfigEditor::EditorState::ENTITY_DISPLAY;
        return;
    }
    libconfig::Setting &component = __cfg.getRoot()["entities"][__selectedEntity]["components"][__selectedComponent];
    size_t argsCount = component["args"].getLength();
    std::string name = "Unknown";

    component.lookupValue("name", name);

    GuiPanel((Rectangle){ 45, 110, 1830, 880 }, TextFormat("Viewing Component: %s", name.c_str()));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    if (GuiButton((Rectangle){ 1772.5, 112.5, 100, 20 }, "Back")) {
        __state = ConfigEditor::EditorState::ENTITY_DISPLAY;
        __selectedComponent = 0;
    }

    for (size_t argIdx = 0; argIdx < argsCount; argIdx++) {
        std::string argName = component["args"][argIdx].getName();
        libconfig::Setting &argValue = component["args"][argIdx];

        std::string argValueStr;
        switch (argValue.getType()) {
            case libconfig::Setting::TypeInt:
                argValueStr = std::to_string(static_cast<int>(argValue));
                break;
            case libconfig::Setting::TypeBoolean:
                argValueStr = argValue ? "true" : "false";
                break;
            case libconfig::Setting::TypeString:
                argValueStr = argValue.c_str();
                break;
            default:
                argValueStr = "Unknown Type";
        }
        GuiLabel((Rectangle){ 53, 143 + argIdx * 20, 782, 20 }, TextFormat("%s: %s", argName.c_str(), argValueStr.c_str()));
    }
}

void ConfigEditor::Gui::drawComponentEditFromEntity()
{
    if (__selectedEntity >= __entityCount) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        return;
    }
    if (__selectedComponent >= __cfg.getRoot()["entities"][__selectedEntity]["components"].getLength()) {
        __state = ConfigEditor::EditorState::ENTITY_EDIT;
        return;
    }
    libconfig::Setting &component = __cfg.getRoot()["entities"][__selectedEntity]["components"][__selectedComponent];
    size_t argsCount = component["args"].getLength();
    std::string name = "Unknown";

    component.lookupValue("name", name);

    GuiPanel((Rectangle){ 45, 110, 1830, 880 }, TextFormat("Viewing Component: %s", name.c_str()));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    if (GuiButton((Rectangle){ 1772.5, 112.5, 100, 20 }, "Back")) {
        __state = ConfigEditor::EditorState::ENTITY_EDIT;
        __selectedComponent = 0;
    }

    __selectedArgs.resize(argsCount + 1, false);
    __selectedNames.resize(argsCount, false);
    __argsBuffer.resize(argsCount + 1, std::vector<char>(256, '\0'));
    __namesBuffer.resize(argsCount, std::vector<char>(256, '\0'));
    std::vector<size_t> indicesToDelete;

    GuiLabel((Rectangle){ 53, 150, 782, 20 }, TextFormat("Name: %s", name.c_str()));

    if (__selectedArgs[argsCount]) {
        if (GuiTextBox((Rectangle){ 1550, 150, 300, 25 }, __argsBuffer[argsCount].data(), 256, true)) {
            name = std::string(__argsBuffer[argsCount].data());
            component["name"] = name;
            __selectedArgs[argsCount] = false;
            return;
        }
        if (GuiButton((Rectangle){ 1440, 150, 100, 25 }, "Validate")) {
            name = std::string(__argsBuffer[argsCount].data());
            component["name"] = name;
            __selectedArgs[argsCount] = false;
            return;
        }
    } else {
        if (GuiButton((Rectangle){ 1750, 150, 100, 25 }, "Edit")) {
            __selectedArgs[argsCount] = true;
            for (size_t i = 0; i < argsCount; i++) {
                __selectedArgs[i] = false;
            }
            strcpy(__argsBuffer[argsCount].data(), name.c_str());
            return;
        }
    }

    size_t argIdx = 0;
    for (; argIdx < argsCount; argIdx++) {
        std::string argName = component["args"][argIdx].getName();
        libconfig::Setting &argValue = component["args"][argIdx];

        std::string argValueStr;
        switch (argValue.getType()) {
            case libconfig::Setting::TypeInt:
                argValueStr = std::to_string(static_cast<int>(argValue));
                break;
            case libconfig::Setting::TypeBoolean:
                argValueStr = argValue ? "true" : "false";
                break;
            case libconfig::Setting::TypeString:
                argValueStr = argValue.c_str();
                break;
            default:
                argValueStr = "Unknown Type";
        }
        GuiLabel((Rectangle){ 53, 180 + argIdx * 30, 782, 20 }, TextFormat("%s: %s", argName.c_str(), argValueStr.c_str()));
        if (GuiButton((Rectangle){ 1750, 180 + argIdx * 30, 100, 25 }, "Delete")) {
            indicesToDelete.push_back(argIdx);
        }

        if (__selectedArgs[argIdx]) {
            if (GuiTextBox((Rectangle){ 1440, 180 + argIdx * 30, 300, 25 }, __argsBuffer[argIdx].data(), 256, true)) {
                switch (argValue.getType()) {
                    case libconfig::Setting::TypeInt:
                        argValue = atoi(__argsBuffer[argIdx].data());
                        break;
                    case libconfig::Setting::TypeBoolean:
                        argValue = strcmp(__argsBuffer[argIdx].data(), "true") == 0;
                        break;
                    case libconfig::Setting::TypeString:
                        argValue = std::string(__argsBuffer[argIdx].data());
                        break;
                    default:
                        break;
                }
                __selectedArgs[argIdx] = false;
                return;
            }
            if (GuiButton((Rectangle){ 1330, 180 + argIdx * 30, 100, 25 }, "Validate")) {
                switch (argValue.getType()) {
                    case libconfig::Setting::TypeInt:
                        argValue = atoi(__argsBuffer[argIdx].data());
                        break;
                    case libconfig::Setting::TypeBoolean:
                        argValue = strcmp(__argsBuffer[argIdx].data(), "true") == 0;
                        break;
                    case libconfig::Setting::TypeString:
                        argValue = std::string(__argsBuffer[argIdx].data());
                        break;
                    default:
                        break;
                }
                __selectedArgs[argIdx] = false;
                return;
            }
        } else if (__selectedNames[argIdx]) {
            if (GuiTextBox((Rectangle){ 1440, 180 + argIdx * 30, 300, 25 }, __namesBuffer[argIdx].data(), 256, true)) {
                if (!component["args"].exists(__namesBuffer[argIdx].data())) {
                    libconfig::Setting::Type argType = component["args"][argIdx].getType();
                    __cfg.getRoot()["entities"][__selectedEntity]["components"][__selectedComponent]["args"].remove(argIdx);
                    libconfig::Setting* newArg = nullptr;

                    switch (argType) {
                        case libconfig::Setting::TypeInt:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeInt);
                            *newArg = std::stoi(argValueStr);
                            break;
                        case libconfig::Setting::TypeBoolean:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeBoolean);
                            *newArg = (argValueStr == "true");
                            break;
                        case libconfig::Setting::TypeString:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeString);
                            *newArg = argValueStr;
                            break;
                        default:
                            std::cerr << "Error: Unsupported type!" << std::endl;
                            break;
                    }
                }
                __selectedNames[argIdx] = false;
                return;
            }
            if (GuiButton((Rectangle){ 1330, 180 + argIdx * 30, 100, 25 }, "Validate")) {
                if (!component["args"].exists(__namesBuffer[argIdx].data())) {
                    libconfig::Setting::Type argType = component["args"][argIdx].getType();
                    __cfg.getRoot()["entities"][__selectedEntity]["components"][__selectedComponent]["args"].remove(argIdx);
                    libconfig::Setting* newArg = nullptr;

                    switch (argType) {
                        case libconfig::Setting::TypeInt:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeInt);
                            *newArg = std::stoi(argValueStr);
                            break;
                        case libconfig::Setting::TypeBoolean:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeBoolean);
                            *newArg = (argValueStr == "true");
                            break;
                        case libconfig::Setting::TypeString:
                            newArg = &component["args"].add(__namesBuffer[argIdx].data(), libconfig::Setting::TypeString);
                            *newArg = argValueStr;
                            break;
                        default:
                            std::cerr << "Error: Unsupported type!" << std::endl;
                            break;
                    }
                }
                __selectedNames[argIdx] = false;
                return;
            }
        } else {
            if (GuiButton((Rectangle){ 1530, 180 + argIdx * 30, 100, 25 }, "Edit name")) {
                __selectedNames[argIdx] = true;
                __selectedArgs[argsCount] = false;
                for (size_t i = 0; i < argsCount; i++) {
                    __selectedArgs[i] = false;
                }
                for (size_t i = 0; i < argsCount; i++) {
                    if (i != argIdx) {
                        __selectedNames[i] = false;
                    }
                }
                strcpy(__namesBuffer[argIdx].data(), argName.c_str());
                return;
            }
            if (GuiButton((Rectangle){ 1640, 180 + argIdx * 30, 100, 25 }, "Edit value")) {
                __selectedArgs[argIdx] = true;
                __selectedArgs[argsCount] = false;
                for (size_t i = 0; i < argsCount; i++) {
                    if (i != argIdx) {
                        __selectedArgs[i] = false;
                    }
                }
                for (size_t i = 0; i < argsCount; i++) {
                    __selectedNames[i] = false;
                }
                strcpy(__argsBuffer[argIdx].data(), argValueStr.c_str());
                return;
            }
        }
    }
    if (GuiButton((Rectangle){ 53, 180 + argIdx * 30, 150, 25 }, "Add new integer key")) {
        libconfig::Setting &newArg = component["args"].add(std::string("newKey" + std::to_string(argIdx)), libconfig::Setting::TypeInt);
        newArg = 0;
    };
    if (GuiButton((Rectangle){ 213, 180 + argIdx * 30, 150, 25 }, "Add new boolean key")) {
        libconfig::Setting &newArg = component["args"].add(std::string("newKey" + std::to_string(argIdx)), libconfig::Setting::TypeBoolean);
        newArg = false;
    };
    if (GuiButton((Rectangle){ 373, 180 + argIdx * 30, 150, 25 }, "Add new string key")) {
        libconfig::Setting &newArg = component["args"].add(std::string("newKey" + std::to_string(argIdx)), libconfig::Setting::TypeString);
        newArg = "value";
    };
    for (auto &idx : indicesToDelete) {
        __cfg.getRoot()["entities"][__selectedEntity]["components"][__selectedComponent]["args"].remove(idx);
    }
}

void ConfigEditor::Gui::drawEntity()
{
    if (__selectedEntity >= __entityCount) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        return;
    }
    libconfig::Setting &entity = __cfg.getRoot()["entities"][__selectedEntity];
    libconfig::Setting &components = entity["components"];

    std::vector<size_t> indicesToDelete;
    std::string name = "Entity";
    int spawnTime = 0;
    int componentsCount = components.getLength();
    std::string type = "Unknown";

    entity.lookupValue("name", name);
    entity.lookupValue("spawnTime", spawnTime);
    entity.lookupValue("type", type);

    GuiPanel((Rectangle){ 45, 110, 1830, 880 }, TextFormat("Viewing Entity: %s", name.c_str()));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    if (GuiButton((Rectangle){ 1772.5, 112.5, 100, 20 }, "Back")) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        __selectedEntity = 0;
    }
    GuiLabel((Rectangle){ 60, 130, 200, 70 }, TextFormat("Spawn Time: %d\nTemplate: %s\nComponents: %d", spawnTime, type.c_str(), componentsCount));

    for (size_t idx = 0; idx < componentsCount; idx++) {
        libconfig::Setting &component = components[idx];
        std::string name = "Unknown";
        int argsCount = component["args"].getLength();

        component.lookupValue("name", name);

        if (GuiButton((Rectangle){ 80 + 220 * (idx % 8), 200 + 75 * (idx / 8), 200, 50 }, "")) {
            __selectedComponent = idx;
            __state = ConfigEditor::EditorState::ENTITY_COMPONENT_DISPLAY;
        }
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);
        GuiPanel((Rectangle){ 80 + 220 * (idx % 8), 200 + 75 * (idx / 8), 200, 51 }, name.c_str());
        GuiLabel((Rectangle){ 88 + 220 * (idx % 8), 212 + 75 * (idx / 8), 180, 50 }, TextFormat("Args: %d", argsCount));
    }
}

void ConfigEditor::Gui::drawEntityEdit()
{
    if (__selectedEntity >= __entityCount) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        return;
    }
    libconfig::Setting &entity = __cfg.getRoot()["entities"][__selectedEntity];
    libconfig::Setting &components = entity["components"];

    std::vector<size_t> indicesToDelete;
    std::string name = "Entity";
    int spawnTime = 0;
    int componentsCount = components.getLength();
    std::string type = "Unknown";

    entity.lookupValue("name", name);
    entity.lookupValue("spawnTime", spawnTime);
    entity.lookupValue("type", type);

    GuiPanel((Rectangle){ 45, 110, 1830, 880 }, TextFormat("Editing Entity: %s", name.c_str()));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    if (GuiButton((Rectangle){ 1772.5, 112.5, 100, 20 }, "Back")) {
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
    }

    static bool nameEditMode = false;
    static bool templateEditMode = false;
    static bool spawnTimeEditMode = false;

    static char nameBuffer[128];
    static char templateBuffer[128];
    static char spawnTimeBuffer[32];

    GuiLabel((Rectangle){ 60, 150, 782, 25 }, TextFormat("Name: %s", name.c_str()));
    GuiLabel((Rectangle){ 60, 180, 782, 25 }, TextFormat("Template: %s", type.c_str()));
    GuiLabel((Rectangle){ 60, 210, 782, 25 }, TextFormat("SpawnTime: %s", std::to_string(spawnTime).c_str()));

    if (nameEditMode) {
        if (GuiTextBox((Rectangle){ 1550, 150, 300, 25 }, nameBuffer, sizeof(nameBuffer), true)) {
            name = std::string(nameBuffer);
            entity["name"] = name;
            nameEditMode = false;
            return;
        }
        if (GuiButton((Rectangle){ 1440, 150, 100, 25 }, "Validate")) {
            name = std::string(nameBuffer);
            entity["name"] = name;
            nameEditMode = false;
            return;
        }
    } else {
        if (GuiButton((Rectangle){ 1750, 150, 100, 25 }, "Edit")) {
            nameEditMode = true;
            templateEditMode = false;
            spawnTimeEditMode = false;
            strcpy(nameBuffer, name.c_str());
            return;
        }
    }

    if (templateEditMode) {
        if (GuiTextBox((Rectangle){ 1550, 180, 300, 25 }, templateBuffer, sizeof(templateBuffer), true)) {
            type = std::string(templateBuffer);
            templateEditMode = false;
            if (!entity.exists("type"))
                entity.add("type", libconfig::Setting::TypeString) = type;
            entity["type"] = type;
            return;
        }
        if (GuiButton((Rectangle){ 1440, 180, 100, 25 }, "Validate")) {
            type = std::string(templateBuffer);
            if (!entity.exists("type"))
                entity.add("type", libconfig::Setting::TypeString) = type;
            templateEditMode = false;
            return;
        }
    } else {
        if (GuiButton((Rectangle){ 1750, 180, 100, 25 }, "Edit")) {
            templateEditMode = true;
            nameEditMode = false;
            spawnTimeEditMode = false;
            strcpy(templateBuffer, type.c_str());
            return;
        }
    }

    if (spawnTimeEditMode) {
        if (GuiTextBox((Rectangle){ 1550, 210, 300, 25 }, spawnTimeBuffer, sizeof(spawnTimeBuffer), true)) {
            spawnTime = atoi(spawnTimeBuffer);
            entity["spawnTime"] = spawnTime;
            spawnTimeEditMode = false;
            return;
        }
        if (GuiButton((Rectangle){ 1440, 210, 100, 25 }, "Validate")) {
            spawnTime = atoi(spawnTimeBuffer);
            entity["spawnTime"] = spawnTime;
            spawnTimeEditMode = false;
            return;
        }
    } else {
        if (GuiButton((Rectangle){ 1750, 210, 100, 25 }, "Edit")) {
            spawnTimeEditMode = true;
            templateEditMode = false;
            nameEditMode = false;
            strcpy(spawnTimeBuffer, std::to_string(spawnTime).c_str());
            return;
        }
    }
    size_t idx = 0;
    for (; idx < componentsCount; idx++) {
        libconfig::Setting &component = components[idx];
        std::string name = "Unknown";
        int argsCount = component["args"].getLength();

        component.lookupValue("name", name);

        if (GuiButton((Rectangle){ 80 + 220 * (idx % 8), 275 + 100 * (idx / 8), 200, 25 }, "")) {
            __selectedComponent = idx;
            __state = ConfigEditor::EditorState::ENTITY_COMPONENT_DISPLAY;
        }
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);
        GuiPanel((Rectangle){ 80 + 220 * (idx % 8), 250 + 100 * (idx / 8), 200, 51 }, name.c_str());
        GuiLabel((Rectangle){ 88 + 220 * (idx % 8), 262 + 100 * (idx / 8), 180, 50 }, TextFormat("Args: %d", argsCount));
        if (GuiButton((Rectangle){ 80 + 220 * (idx % 8), 300 + 100 * (idx / 8), 200, 30 }, "Edit")) {
            __selectedComponent = idx;
            __state = ConfigEditor::EditorState::ENTITY_COMPONENT_EDIT;
        }
        GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0);
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0);
        if (GuiButton((Rectangle){ 80 + 220 * (idx % 8) + 177.5, 250 + 100 * (idx / 8) + 2.5, 20, 20 }, "x")) {
            indicesToDelete.push_back(idx);
        };
    }
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);
    if (GuiButton((Rectangle){ 80 + 220 * (idx % 8), 275 + 100 * (idx / 8), 200, 25 }, "Add new component")) {
        libconfig::Setting &newComponent = components.add(libconfig::Setting::TypeGroup);
        newComponent.add("name", libconfig::Setting::TypeString) = "New Component";
        newComponent.add("args", libconfig::Setting::TypeGroup);
    }
    for (auto &idx2 : indicesToDelete) {
        __cfg.getRoot()["entities"][__selectedEntity]["components"].remove(idx2);
    }
}

void ConfigEditor::Gui::drawCreateNewConfig()
{
    if (GuiTextBox((Rectangle){ 45, 110, 1830, 25 }, __fileDialogState.fileNameText, 256, true)) {
        __fileDialogState.fileNameText[strlen(__fileDialogState.fileNameText)] = '\0';
    }
    if (GuiButton((Rectangle){ 45, 140, 1830, 25 }, "Create")) {
        __fullPath = std::string(__fileDialogState.dirPathText) + "/" + std::string(__fileDialogState.fileNameText) + ".cfg";
        __cfg.getRoot().add("entities", libconfig::Setting::TypeList);
        __cfg.getRoot().add("templates", libconfig::Setting::TypeList);
        __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
        __entityCount = 0;
        saveConfig();
    }
}

void ConfigEditor::Gui::tick()
{
    __exitWindow = WindowShouldClose();

    if (__fileDialogState.SelectFilePressed) {
        if (IsFileExtension(__fileDialogState.fileNameText, ".cfg")) {
            __fullPath = std::string(__fileDialogState.dirPathText) + "/" + std::string(__fileDialogState.fileNameText);

            try {
                __cfg.readFile(__fullPath);
                std::cerr << "\nConfig file loaded: " << __fullPath << std::endl;
                __state = ConfigEditor::EditorState::ENTITIES_DISPLAY;
                __entityCount = __cfg.getRoot()["entities"].getLength();
            } catch (libconfig::ParseException &e) {
                std::string err = "Error while parsing file: " +
                std::string(e.getFile()) + " in line: " +
                std::to_string(e.getLine()) + " : " + std::string(e.getError());
                throw std::runtime_error(err);
            } catch (libconfig::FileIOException &e) {
                std::string err = "Error while reading file: " +
                std::string(e.what()) + "\n" + "File path was: " + __fullPath;
                throw std::runtime_error(err);
            }
        }

        __fileDialogState.SelectFilePressed = false;
    }

    BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, 19)));

        DrawRectangle(0, 0, GetScreenWidth(), 70, LIGHTGRAY);

        DrawText(TextFormat("Current file: %s", __fileDialogState.fileNameText), 320, 30, 10, DARKGRAY);

        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 80);
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 200);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 100);
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 250);

        if (GuiButton((Rectangle){ 20, 20, 140, 30 }, GuiIconText(ICON_FILE_OPEN, "Select level")))
            __fileDialogState.windowActive = true;
        if (GuiButton((Rectangle){ 170, 20, 140, 30 }, GuiIconText(ICON_FILE_ADD, "New level")))
            __state = ConfigEditor::EditorState::CREATE_NEW_CONFIG;
        if (GuiButton((Rectangle){ 1800, 20, 100, 30 }, "Close"))
            __exitWindow = true;
        if (GuiButton((Rectangle){ 1680, 20, 100, 30 }, "Save"))
            saveConfig();

        switch (__state) {
            case ConfigEditor::EditorState::CONFIG_NOT_LOADED:
                break;
            case ConfigEditor::EditorState::ENTITIES_DISPLAY:
                drawEntities();
                break;
            case ConfigEditor::EditorState::ENTITY_DISPLAY:
                drawEntity();
                break;
            case ConfigEditor::EditorState::ENTITY_EDIT:
                drawEntityEdit();
                break;
            case ConfigEditor::EditorState::ENTITY_COMPONENT_DISPLAY:
                drawComponentFromEntity();
                break;
            case ConfigEditor::EditorState::ENTITY_COMPONENT_EDIT:
                drawComponentEditFromEntity();
                break;
            case ConfigEditor::EditorState::CREATE_NEW_CONFIG:
                drawCreateNewConfig();
                break;
        }

        if (__fileDialogState.windowActive) GuiLock();

        GuiUnlock();

        GuiWindowFileDialog(&__fileDialogState);

    EndDrawing();
}
