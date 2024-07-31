#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "xorstr.h"

const size_t buffer_size = 64 * 1024 * 1024;

bool write_random_data()
{
    std::ofstream file(xorstr_("temp_file.dat"), std::ios::binary | std::ios::out);
    if (!file.is_open())
    {
        std::puts(xorstr_("[disk] could not open file for writing"));
        return false;
    }

    char* buffer = new(std::nothrow) char[buffer_size];
    if (!buffer)
    {
        std::puts(xorstr_("[disk] allocation failed"));
        return false;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < buffer_size; ++i)
    {
        buffer[i] = static_cast<char>(std::rand() % 256);
    }

    size_t written = 0;
    while (true)
    {
        file.write(buffer, buffer_size);
        if (!file)
        {
            std::puts(xorstr_("[disk] writing failed disk may be full"));
            break;
        }

        written += buffer_size;
        if (written % (1024 * 1024 * 1024) == 0)
        {
            std::cout << xorstr_("\r[disk] overwritten: ") << (written / (1024 * 1024 * 1024)) << xorstr_(" gb") << std::flush;
        }
    }

    delete[] buffer;
    file.close();
    std::remove(xorstr_("temp_file.dat"));

    return true;
}

int main()
{ 
    SetConsoleTitle(xorstr_("wipe_disk - github.com/sharkOG/"));

    std::puts(xorstr_("[disk] starting cleaning .."));

    if (write_random_data())
    {
        std::puts(xorstr_("[disk] cleaning completed .."));
    }
    else {
        std::puts(xorstr_("[disk] cleaning failed .."));
    }

    std::puts(xorstr_("[disk] for exit press enter .."));

    std::cin.get();

    return 0;
}