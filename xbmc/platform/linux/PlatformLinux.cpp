/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PlatformLinux.h"

#include "utils/StringUtils.h"

#include "platform/linux/powermanagement/LinuxPowerSyscall.h"

#include "windowing/WinSystemHeadless.h"

#include <cstdlib>

CPlatform* CPlatform::CreateInstance()
{
  return new CPlatformLinux();
}

bool CPlatformLinux::Init()
{
  if (!CPlatformPosix::Init())
    return false;

  setenv("OS", "Linux", true); // for python scripts that check the OS

  CWinSystemHeadless::RegisterMe();

  CLinuxPowerSyscall::Register();

  return true;

  std::string envSink;
  if (getenv("KODI_AE_SINK"))
    envSink = getenv("KODI_AE_SINK");

  if (StringUtils::EqualsNoCase(envSink, "ALSA"))
  {
    OPTIONALS::ALSARegister();
  }
  else if (StringUtils::EqualsNoCase(envSink, "PULSE"))
  {
    OPTIONALS::PulseAudioRegister();
  }
  else if (StringUtils::EqualsNoCase(envSink, "SNDIO"))
  {
    OPTIONALS::SndioRegister();
  }
  else if (StringUtils::EqualsNoCase(envSink, "ALSA+PULSE"))
  {
    OPTIONALS::ALSARegister();
    OPTIONALS::PulseAudioRegister();
  }
  else
  {
    if (!OPTIONALS::PulseAudioRegister())
    {
      if (!OPTIONALS::ALSARegister())
      {
        OPTIONALS::SndioRegister();
      }
    }
  }

  m_lirc.reset(OPTIONALS::LircRegister());

  return true;
}
