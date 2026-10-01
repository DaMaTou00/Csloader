# README

# csloader

## 免责声明

本项目仅用于学习用途，用于理解 Windows 平台下的底层 API 解析、内存分配、内存保护、缓冲区解密和回调调度等技术点。

请勿将本项目用于任何未经授权的系统、网络或设备。因使用本项目产生的任何后果，由使用者自行承担。
## 项目结构

```text
csloader/
├── TeachingPackerDemo.exe   # 打包器前端程序，用于生成和配置最终输出文件,C#编写。
├── src/                     # 核心代码目录
│   ├── main.cpp             # 主逻辑代码
│   ├── myapi.h              # 对外接口声明
│   ├── struct.h             # 结构体和配置定义
│   ├── config.h             # 配置相关头文件
│   ├── result.h             # 生成或嵌入的数据文件
│   └── libstaticlib.a       # 核心静态库
├── opt/                     # 辅助工具目录
│   ├── check.exe            # sigthief.py
│   ├── check.ini            # resourcehacker 生成文件
│   └── calc.py              # resourcehacker
├── out/                     # 输出目录，用于存放生成后的 exe 文件
└── README.md                # 项目说明文档
```
## 技术介绍

本项目主要用于加载无阶段的载荷，实现功能有动态获取API、内存保护、Module Stomping等功能。  
以上功能均在加载器上实现，所以使用时尽量不要用udrl并且按照以下配置修改profile部分内容。

```c
stage {
    set allocator      "VirtualAlloc";
    set magic_mz_x86   "BLOB";
    set magic_mz_x64   "BLOB";
    set magic_pe       "EA";
    set userwx         "true";
    set stomppe        "true";
    set obfuscate      "false";
    set cleanup        "true";
    set sleep_mask     "false";
    set smartinject    "false";
    set image_size_x86 "360448";
    set image_size_x64 "360448";

    transform-x86 {
        strrep "ReflectiveLoader" "DoLegitWork";
        strrep "beacon.dll"       "";
        strrep "This program cannot be run in DOS mode" "";
        strrep "%s (admin)"       "%s";
        strrep "%s as %s\x5c%s"   "%s as %s/%s";
    }

    transform-x64 {
        strrep "ReflectiveLoader" "DoLegitWork";
        strrep "beacon.x64.dll"   "";
        strrep "beacon.dll"       "";
        strrep "This program cannot be run in DOS mode" "";
        strrep "%s (admin)"       "%s";
        strrep "%s as %s\x5c%s"   "%s as %s/%s";
    }
}
```
生成BIN时按照如下配置即可，不要使用syscall

<img width="388" height="363" alt="图片" src="https://github.com/user-attachments/assets/52887bb8-606e-4741-a578-2a5f581b667b" />


## 主要模块

### API 解析

通过 PEB 和导出表解析 Kernel32、Ntdll 等模块中的函数地址，并支持加密名称匹配。也可以通过 syscall 方式获取 NT 函数。

### 环境检查

包含时间流逝、CPU/内存、系统语言和常见进程检查等逻辑。

延时检查：睡眠十秒并检测是否有加速，主要用于绕过杀软的本地沙箱检测

CPU/内存检查：CPU 逻辑核心数 > 4，物理内存 > 5 GB

系统语言：非中文直接退出

常见进程检查包括：

- `WXWorkWeb.exe`
- `WeChatAppEx.exe`
- `WXWork.exe`
- `Weixin.exe`

### 内存分配

通过以下方式分配缓冲区：

- `NtAllocateVirtualMemory`
- `NtCreateSection` / `NtMapViewOfSection`
- `HeapAlloc`

### 内存保护

beacon的sizeofImage如果小于一定字节（profile中配置image_size_x86/x64配置实际beacon需要的内存大小），就将Beacon展开到一个合法的dll的text节，用于伪装，同时在睡眠期间对Beacon做加密并设置内存不可访问，同时把sleep函数的栈进行修改，进一步实现伪装。

### 加密处理

支持以下加密方式处理BIN文件

- AES-CBC
- RC4
- XORSHIFT

### 执行方式

包含枚举回调、Windows Hook、Fiber 等回调/纤程。

## 免杀效果

### 火绒

<img width="891" height="802" alt="图片" src="https://github.com/user-attachments/assets/41c9fd6c-a259-41e1-9380-e1c4958b8ce0" />

### defender


<img width="709" height="634" alt="defender" src="https://github.com/user-attachments/assets/75e8ea81-5847-4039-a950-33c0f45ffeee" />

### 赛门EDR

<img width="1301" height="685" alt="赛门" src="https://github.com/user-attachments/assets/dd46e5bd-26ba-4109-8129-2b44a9ddb250" />


### 360

面对360不是很稳定，因此考虑用patch制作loader，不过这种方式在动态行为上没有对beacon进行保护，这里考虑用udrl技术或sleepmask进行保护。

<img width="1520" height="611" alt="图片" src="https://github.com/user-attachments/assets/459ddcb6-d845-42dc-89c4-388f07cebdf0" />

#### patch使用说明

在out目录下存在文件wechat.exe。这里做了两个地方的函数修改sub_7FF637DB43E0（用于跳转）和sub_7FF637E8BD00（用于执行），其他地方均未修改。
使用时先用python脚本（encrypt.py）加密bin文件，并将其保存到一张png图片里。
python xor_encrypt.py --pack <png_file> <shellcode.bin> <output.png>
然后利用resourcehacker，将png图片加载到wechat.exe中并将资源名命名为1


<img width="430" height="429" alt="图片" src="https://github.com/user-attachments/assets/d9ce767a-4c24-4ffe-975c-36592c69ef78" />

<img width="248" height="258" alt="图片" src="https://github.com/user-attachments/assets/d6299df1-34b0-4d76-889d-af7130c52ffd" />









