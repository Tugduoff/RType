/*
** EPITECH PROJECT, 2024
** ConfigEditor
** File description:
** Gui.hpp file
*/

#ifndef GUI_CONFIG_EDITOR_HPP
    #define GUI_CONFIG_EDITOR_HPP

    #include "raylib.h"
    #include "gui_window_file_dialog.h"

    #include <string>
    #include <fstream>
    #include <iostream>
    #include <cstring>
    #include <libconfig.h++>
    #include <vector>
    #include <memory>

namespace ConfigEditor {
    enum EditorState {
        CONFIG_NOT_LOADED,
        ENTITIES_DISPLAY,
        ENTITY_DISPLAY,
        ENTITY_EDIT,
        ENTITY_COMPONENT_EDIT,
        ENTITY_COMPONENT_DISPLAY,
        CREATE_NEW_CONFIG
    };

    class Gui {
        public:
            Gui();
            ~Gui() = default;

            void saveConfig();
            void drawEntities();
            void drawEntity();
            void drawEntityEdit();
            void drawComponentFromEntity();
            void drawComponentEditFromEntity();
            void drawCreateNewConfig();
            bool shouldExit() const { return __exitWindow; };
            void tick();
            void close();

        protected:
        private:

            std::string __fullPath;
            libconfig::Config __cfg;

            size_t __entityCount;

            Rectangle __panelRec;
            Rectangle __panelView;
            Vector2 __panelScroll;

            bool __exitWindow;

            GuiWindowFileDialogState __fileDialogState;

            EditorState __state;

            size_t __selectedEntity;
            size_t __selectedComponent;

            std::vector<bool> __selectedArgs;
            std::vector<bool> __selectedNames;
            std::vector<std::vector<char>> __argsBuffer;
            std::vector<std::vector<char>> __namesBuffer;

    };
};

#endif // GUI_CONFIG_EDITOR_HPP
