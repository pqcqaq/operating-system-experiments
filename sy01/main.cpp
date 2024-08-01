#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH 260

int copy_file(const wchar_t* source, const wchar_t* destination) {
    // 检查源文件是否存在
    if (GetFileAttributesW(source) == INVALID_FILE_ATTRIBUTES) {
        wprintf(L"错误: 源文件 '%s' 不存在\n", source);
        return 0;
    }

    // 检查目标文件是否已存在
    if (GetFileAttributesW(destination) != INVALID_FILE_ATTRIBUTES) {
        wchar_t response;
        wprintf(L"目标文件 '%s' 已存在。是否覆盖? (y/n): ", destination);
        wscanf(L" %lc", &response);
        if (response != L'y' && response != L'Y') {
            wprintf(L"拷贝操作已取消\n");
            return 0;
        }
    }

    // 执行文件拷贝
    if (CopyFileW(source, destination, FALSE)) {
        wprintf(L"文件成功拷贝到 %s\n", destination);
        return 1;
    } else {
        wprintf(L"拷贝失败: %d\n", GetLastError());
        return 0;
    }
}

// gcc .\main.cpp -o Copy.exe -municode    
int wmain(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        wprintf(L"用法: Copy <源文件> <目标文件1> [<目标文件2> ...]\n");
        return 1;
    }
    const wchar_t* source = argv[1];
    for (int i = 2; i < argc; i++) {
        copy_file(source, argv[i]);
    }
    return 0;
}
