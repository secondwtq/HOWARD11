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

#include "AtTheVeryBeginning.hxx"

namespace Howard {

namespace AtTheVeryBeginning {

constexpr const char Setting<ATVBSettingType::WindowSetting>::section_name[];
constexpr cfg_opt_t Setting<ATVBSettingType::WindowSetting>::section_opts[];
Setting<ATVBSettingType::WindowSetting> Setting<ATVBSettingType::WindowSetting>::m_instance;

constexpr const char Setting<ATVBSettingType::SRendering>::section_name[];
constexpr cfg_opt_t Setting<ATVBSettingType::SRendering>::section_opts[];
Setting<ATVBSettingType::SRendering> Setting<ATVBSettingType::SRendering>::m_instance;

SettingGlobal SettingGlobal::m_instance;
constexpr cfg_opt_t SettingGlobal::opts[];

}

}