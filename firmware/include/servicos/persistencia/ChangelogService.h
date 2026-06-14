#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

class ChangelogService {
public:
    static void init();
    static String getChangelog();
    static void addEntry(const String& versao, const String& data, const String& mudanca);
};
