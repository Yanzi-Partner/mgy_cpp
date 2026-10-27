// main.cpp —— 完整版演示：四部分依次观察“构造 → 独立性 → 作用域 → 补全配置”。

#include <iostream>

#include "Assignment.h"
#include "Console.h"

int main()
{
    console::setupUtf8();
    std::cout << std::boolalpha; // 与课件一致：bool 显示 true / false

    std::cout << "begin: 开始执行 main\n";

    // ==================== 第一部分 ====================
    std::cout << "\n=== 第一部分：自动调用构造函数与构造函数重载 ===\n";
    {
        // 提供三个参数 → 选择三参数构造函数
        Assignment lab1("实验一", 1024, true);
        // 只提供两个参数 → 选择两参数构造函数，报告要求由缺省值 false 决定
        Assignment lab2("实验二", 2048);

        lab1.print(std::cout);
        lab2.print(std::cout);

        // ==================== 第二部分 ====================
        std::cout << "\n=== 第二部分：对象独立性与非法修改 ===\n";

        // 每个对象各有一份数据成员：改实验二的上限，实验一不受影响
        const bool widened = lab2.setMaxBytes(4096);
        std::cout << "把实验二上限改成 4096：" << widened
                  << " → 实验一上限=" << lab1.getMaxBytes()
                  << "，实验二上限=" << lab2.getMaxBytes() << '\n';

        // 非法修改：setter 先校验，失败就 return false，原值保持不变
        const bool zeroLimit = lab1.setMaxBytes(0);
        const bool emptyName = lab1.setTitle("");
        std::cout << "设置零上限是否成功：" << zeroLimit << '\n'
                  << "设置空名称是否成功：" << emptyName << '\n';
        lab1.print(std::cout);

        // ==================== 第三部分 ====================
        std::cout << "\n=== 第三部分：嵌套作用域与析构时机 ===\n";
        {
            Assignment temp("临时作业", 512, false);
            std::cout << "inside: 内部作用域尚未结束\n";
        } // 右花括号结束 temp 的作用域：析构发生在这之后、下一条输出之前
        std::cout << "outside: 临时作业已经析构\n";

        // ==================== 第四部分 ====================
        std::cout << "\n=== 第四部分：非法构造参数与补全配置 ===\n";
        {
            // 名称空、上限 0：构造照样执行，但只存入安全占位值，两个标志都是 false
            Assignment incomplete("", 0, true);
            std::cout << "创建后：\n";
            incomplete.printConfig(std::cout);

            // 先补名称：只有 titleConfigured_ 变 true，isReady 仍为 false
            const bool nameFixed = incomplete.setTitle("补交作业");
            std::cout << "补全名称：" << nameFixed
                      << " → 只补名称，可使用=" << incomplete.isReady() << '\n';

            // 再补上限：两个标志都 true，isReady 才变成 true
            const bool limitFixed = incomplete.setMaxBytes(512);
            std::cout << "补全上限：" << limitFixed
                      << " → 两项都合法，可使用=" << incomplete.isReady() << '\n';
            incomplete.printConfig(std::cout);
        } // incomplete 在这里析构，日志里显示的是补全后的名称

        std::cout << "\n即将离开外层作用域：注意实验二、实验一的析构顺序\n";
    } // 后创建的 lab2 先析构，先创建的 lab1 后析构

    std::cout << "\nend: 外层作用域内的对象均已析构\n";

    // 双击运行 exe 时停住控制台便于观察；脚本 / 管道运行时不会阻塞
    console::waitForEnterIfConsole();
    return 0;
}
