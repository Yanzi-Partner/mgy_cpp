# assignment_complete —— 作业类完整版

对应课件 `Assignment_Complete3.cpp`：构造函数重载、析构函数、以及本课的核心
**「安全占位值」≠「真正完成的业务配置」**。

```
构造成功（对象已存在）
        ≠
业务配置合格（isReady() 为 true）
```

## 一、工程结构

```
assignment_complete/
├─ CMakeLists.txt                   工程文件（CMake，可生成 VS 解决方案）
├─ build.ps1                        一键构建（g++，零告警）
├─ include/
│  ├─ Assignment.h                  5 个成员 + 构造重载 + isReady()
│  └─ Console.h                     控制台辅助（UTF-8、按需等待回车）
├─ src/
│  ├─ Assignment.cpp
│  ├─ Console.cpp
│  └─ main.cpp                      四部分演示
├─ docs/课件代码3_Assignment_Complete.cpp   课件原代码，仅对照，不编译
└─ README.md
```

## 二、编译与运行

```powershell
cd E:\C++\assignment_complete
.\build.ps1
.\assignment_complete.exe
```

CMake 方式：`cmake -S . -B build && cmake --build build`（本机只有 g++，已实测后者）。

## 三、五个成员各自管什么

| 成员 | 作用 | 谁写 |
|---|---|---|
| `title_` | 作业名称 | 构造函数 / `setTitle` |
| `maxBytes_` | 文件大小上限 | 构造函数 / `setMaxBytes` |
| `requireReport_` | 是否需要实验报告 | 构造函数 / `setRequireReport` |
| `titleConfigured_` | 名称是否来自**合法输入** | 构造函数 / `setTitle` 成功后置真 |
| `maxBytesConfigured_` | 上限是否来自**合法输入** | 构造函数 / `setMaxBytes` 成功后置真 |

`isReady()` 只看后两个标志：

```cpp
bool Assignment::isReady() const noexcept
{
    return titleConfigured_ && maxBytesConfigured_;
}
```

所以「成员里都有值」和「配置完成」是两件事——占位值只是让对象保持可读、不崩溃。

### 占位值的处理规则

| 传入值 | 存进成员的值 | 对应标志 | `isReady()` |
|---|---|---|---|
| `""` 或 `"   "`（纯空白） | `待设置名称` | `false` | `false` |
| `0` 字节 | `1` 字节 | `false` | `false` |
| `> 1 GiB` | `1` 字节 | `false` | `false` |
| 正常名称、合法上限 | 原值 | `true` | `true` |

## 四、main 的四部分

| 部分 | 观察什么 |
|---|---|
| 第一部分 | 三参数 / 两参数构造函数按实参个数自动选择，构造日志随即打印 |
| 第二部分 | 改 `lab2` 不影响 `lab1`；`setMaxBytes(0)`、`setTitle("")` 被拒绝，原值不变 |
| 第三部分 | `temp` 在内层大括号结束处析构，早于下一条 `outside` 输出 |
| 第四部分 | `incomplete("", 0, true)` 构造成功但未就绪；补名称仍不够，补上限才变就绪 |
| 收尾 | 外层大括号结束：`lab2` 先析构、`lab1` 后析构 |

## 五、运行结果

```
begin: 开始执行 main

=== 第一部分：自动调用构造函数与构造函数重载 ===
[构造·三参数] 实验一
[构造·两参数] 实验二
名称=实验一，上限=1024 字节，需报告=true，可使用=true
名称=实验二，上限=2048 字节，需报告=false，可使用=true

=== 第二部分：对象独立性与非法修改 ===
把实验二上限改成 4096：true → 实验一上限=1024，实验二上限=4096
设置零上限是否成功：false
设置空名称是否成功：false
名称=实验一，上限=1024 字节，需报告=true，可使用=true

=== 第三部分：嵌套作用域与析构时机 ===
[构造·三参数] 临时作业
inside: 内部作用域尚未结束
[析构] 临时作业
outside: 临时作业已经析构

=== 第四部分：非法构造参数与补全配置 ===
[构造·三参数] 待设置名称
创建后：
  名称=待设置名称（titleConfigured_=false）
  上限=1 字节（maxBytesConfigured_=false）
  需报告=true
  可使用=false
补全名称：true → 只补名称，可使用=false
补全上限：true → 两项都合法，可使用=true
  名称=补交作业（titleConfigured_=true）
  上限=512 字节（maxBytesConfigured_=true）
  需报告=true
  可使用=true
[析构] 补交作业

即将离开外层作用域：注意实验二、实验一的析构顺序
[析构] 实验二
[析构] 实验一

end: 外层作用域内的对象均已析构
```

第四部分两个 `[析构]` 的差别值得注意：`incomplete` 构造时日志是「待设置名称」，
析构时打印的却是「补交作业」——析构函数读的是**当时的成员值**，不是构造时的快照。

## 六、相对课件代码的改动

| # | 课件写法 | 本工程 |
|---|---|---|
| 1 | 类、实现、`main` 都在一个 `.cpp` | 拆成 `.h` + `.cpp` + `main.cpp` |
| 2 | 全局 `using namespace std;` | 一律 `std::` |
| 3 | 两个构造函数各抄一遍 5 行初始化列表 | 都委托给私有构造函数，初始化与校验只写一份 |
| 4 | 占位值与标志各写一遍三目表达式 | 抽成 `hasValidTitle / normalizeTitle / hasValidMaxBytes / normalizeMaxBytes` |
| 5 | `title.empty()` 判空 | 纯空白也算未配置（`find_first_not_of`） |
| 6 | 上限只把 `0` 换掉 | 增加 1 GiB 上限，超限同样记为未配置 |
| 7 | getter 返回值拷贝 | `getTitle()` 返回 `const std::string&` |
| 8 | 每行 `endl` 刷新缓冲 | 统一 `'\n'` |
| 9 | 输出语句在 main 里反复拼装 | `print()` 单行摘要、`printConfig()` 带两个标志 |
| 10 | bool 显示依赖 `boolalpha` 全局状态 | 打印函数内部显式转换，换到别的流也不会变成 1/0 |
| 11 | `cin.get()` 无条件阻塞 | 仅当输入来自控制台才等待，脚本运行不卡住 |
| 12 | 中文可能乱码 | `SetConsoleOutputCP(CP_UTF8)` + 编译期 `-finput-charset/-fexec-charset` |
| 13 | 注释混杂教师口吻 | 注释改为「这段代码为什么这样写」的说明，概念单独写在 README |

关于第 3 条，课件注释提醒「实际初始化顺序是成员声明顺序，不取决于初始化列表书写顺序」，
本工程把这条写进了头文件的数据成员注释，并用 `printConfig()` 把两个标志暴露出来便于验证。

## 七、可继续做的小练习

1. 把 `bool` 标志换成 `enum class ConfigState`（见同目录 `../assignment_lifecycle` 的做法），比较两种表达。
2. 让 `setRequireReport()` 也返回 `bool`，并在「要求报告但上限太小」时拒绝——标志之外再加一条业务规则。
3. 加拷贝构造函数并打印日志，观察 `Assignment b = a;` 时构造/析构各发生几次。
4. 故意把初始化列表的顺序写乱，验证编译器按声明顺序执行（配合 `-Wreorder` 看告警）。
5. 用 `std::vector<Assignment>` 装多份作业，看 `push_back` 的拷贝构造与元素析构。
