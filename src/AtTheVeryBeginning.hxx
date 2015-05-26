//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-24
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_ATTHEVERYBEGINNING_HXX
#define HOWARD11_ATTHEVERYBEGINNING_HXX

#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <confuse.h>

#include <string>

#include "FSM/FSM.hxx"
#include "FSM/FSMHelper.hxx"

#include "Debug.hxx"

namespace Howard {

namespace AtTheVeryBeginning {

using namespace FSMHelper;

enum ATVBSettingType {

    WindowSetting,
    SRendering

};

template <ATVBSettingType SettingT>
struct Setting {

};

template <>
struct Setting<WindowSetting> {

    static constexpr const char section_name[] = "window";
    static constexpr cfg_opt_t section_opts[] = {
        CFG_INT("width", 800, CFGF_NONE),
        CFG_INT("height", 600, CFGF_NONE),
        CFG_STR("title", "Howard 11 Demo", CFGF_NONE),
        CFG_END()
    };

    static void fetch(cfg_t *cfg) {
        cfg_t *section = cfg_getsec(cfg, section_name);
        instance()->width = instance()->actual_width = cfg_getint(section, "width");
        instance()->height = instance()->actual_height = cfg_getint(section, "height");
        instance()->title = cfg_getstr(section, "title");
    }

    size_t width, height;
    size_t actual_width, actual_height;
    std::string title;

    static Setting<WindowSetting> *instance() {
        return &m_instance; }

    private:
        static Setting<WindowSetting> m_instance;
};

template <>
struct Setting<SRendering> {

    static constexpr const char section_name[] = "rendering";
    static constexpr cfg_opt_t section_opts[] = {
        CFG_INT("gl_version_major", 4, CFGF_NONE),
        CFG_INT("gl_version_minor", 1, CFGF_NONE),
        CFG_END()
    };

    static void fetch(cfg_t *cfg) {
        cfg_t *section = cfg_getsec(cfg, section_name);
        instance()->glv_major = cfg_getint(section, "gl_version_major");
        instance()->glv_minor = cfg_getint(section, "gl_version_minor");
    }

    char glv_major, glv_minor;

    static Setting<SRendering> *instance() {
        return &m_instance; }
    private: static Setting<SRendering> m_instance;
};

class SettingGlobal {

    static constexpr cfg_opt_t opts[] = {
        CFG_SEC(Setting<WindowSetting>::section_name,
                const_cast<cfg_opt_t *>(Setting<WindowSetting>::section_opts),
                CFGF_NONE),
        CFG_SEC(Setting<SRendering>::section_name,
                const_cast<cfg_opt_t *>(Setting<SRendering>::section_opts),
                CFGF_NONE),
        CFG_END()
    };

    public:

    void load(const char *filename) {
        log("ATVB", Debug) << "AtTheVeryBeginning: Loading " << filename << rn;

        cfg_t *cfg = cfg_init(const_cast<cfg_opt_t *>(opts), CFGF_NONE);
        switch(cfg_parse(cfg, filename)) {
            case CFG_FILE_ERROR:
                printf("warning: configuration file '%s' could not be read: %s\n"
                               "continuing with default values...\n\n", filename, strerror(errno));
            case CFG_SUCCESS:
                break;
            case CFG_PARSE_ERROR:
                printf("warning: configuration file '%s' error: %s\n"
                               "exiting...\n\n", filename, strerror(errno));
                ASSERT_FOUNDATION();
        }

        Setting<WindowSetting>::fetch(cfg);
        Setting<SRendering>::fetch(cfg);
        cfg_free(cfg);
    }

    static SettingGlobal *instance() { return &m_instance; }

    private:

    static SettingGlobal m_instance;

};

}

}

#endif // HOWARD11_ATTHEVERYBEGINNING_HXX
