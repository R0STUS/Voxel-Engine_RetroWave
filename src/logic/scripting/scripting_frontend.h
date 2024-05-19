#ifndef LOGIC_SCRIPTING_SCRIPTING_FRONTEND_H_
#define LOGIC_SCRIPTING_SCRIPTING_FRONTEND_H_

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Hud;

namespace scripting {
    extern Hud* hud;

    void on_frontend_init(Hud* hud);
    void on_frontend_close();

    /** 
     * Load package-specific hud script 
     * @param env environment id
     * @param packid content-pack id
     * @param file script file path
     */
    void load_hud_script(int env, std::string packid, fs::path file);
}

#endif // LOGIC_SCRIPTING_SCRIPTING_FRONTEND_H_
