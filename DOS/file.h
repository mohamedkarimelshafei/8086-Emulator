#pragma once

class DOSFileSystem
{
public:
    bool Open(const char* path);
    void Close();
};
