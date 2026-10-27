// ============================================================
// 课件原始代码 3：Assignment_Complete3.cpp（作业类完整版）
// 仅作对照参考，不参与编译。
// 新版实现：include/Assignment.h + src/Assignment.cpp + src/main.cpp
// ============================================================

// Assignment_Complete 完整版
#include <iostream> // 提供 cout、endl、boolalpha、cin
#include <string>   // 提供 string
using namespace std;

// 在第一课 Assignment 类的基础上增加构造函数、析构函数和状态检查。
// 【教师解释】下面不是创建作业，而是定义“作业类型”的规则；main 中的 lab1、lab2 才是实例。
class Assignment {
private:
    // 【教师解释】这五个字段存入对象状态。前3个承接上一课，后2个是本课新增的配置就绪标志。
    // 类中普通数据成员在不同对象中各自独立；下划线是命名习惯。
    string title_;                  // 作业名称：每个对象独立保存
    unsigned long long maxBytes_;   // 文件大小上限（字节）
    bool requireReport_;            // 是否需要实验报告

    // 教学补充：区分“安全占位值”和“真正完成的业务配置”。
    bool titleConfigured_;          // 是否提供了合法作业名称
    bool maxBytesConfigured_;       // 是否提供了合法大小上限

public:
    // 构造函数一：提供名称、大小上限，默认不要求实验报告。
    // 【教师解释】两参数构造函数：创建对象时只有“名称、大小上限”，缺省的报告要求由本构造函数设为false。
    // 请观察冒号后的初始化列表，而不是把它理解成普通赋值语句。
    Assignment(const string& title, unsigned long long maxBytes)
        // 【语法提示】a ? b : c 是条件表达式，a 真时取b，否则取c。
        : title_(title.empty() ? "待设置名称" : title),
          maxBytes_(maxBytes == 0 ? 1 : maxBytes),
          requireReport_(false),
          titleConfigured_(!title.empty()),
          // 【顺序提示】实际成员初始化顺序是本类成员的声明顺序，不取决于初始化列表书写顺序。
          maxBytesConfigured_(maxBytes != 0) {
        cout << "[构造·两参数] " << title_ << endl;
    }

    // 构造函数二：同时提供名称、大小上限和报告要求。
    // 【教师解释】三参数构造函数。main 创建lab1时因为提供三个参数而选择此版本。
    Assignment(const string& title, unsigned long long maxBytes,
               bool requireReport)
        : title_(title.empty() ? "待设置名称" : title),
          maxBytes_(maxBytes == 0 ? 1 : maxBytes),
          requireReport_(requireReport),
          titleConfigured_(!title.empty()),
          maxBytesConfigured_(maxBytes != 0) {
        cout << "[构造·三参数] " << title_ << endl;
    }

    // 析构函数：局部对象离开作用域时自动调用。
    // 本例只打印日志以便观察，类自身没有需要手动释放的资源。
    // 【教师解释】~类名()就是析构函数，本程序只打印日志以观察何时销毁。
    ~Assignment() {
        cout << "[析构] " << title_ << endl;
    }

    // 以下接口延续第一节课的 setter / getter。
    // 【教师解释】这里仍是上一节课的setter。先校验，非法就直接return false，原值保持。
    bool setTitle(const string& title) {
        if (title.empty()) {
            return false;               // 非法修改：原值不变
        }
        title_ = title;
        titleConfigured_ = true;
        return true;
    }

    bool setMaxBytes(unsigned long long bytes) {
        if (bytes == 0) {
            return false;               // 非法修改：原值不变
        }
        maxBytes_ = bytes;
        maxBytesConfigured_ = true;
        return true;
    }

    void setRequireReport(bool value) {
        requireReport_ = value;
    }

    string getTitle() const {
        return title_;
    }

    unsigned long long getMaxBytes() const {
        return maxBytes_;
    }

    bool getRequireReport() const {
        return requireReport_;
    }

    // 只有名称与大小上限都由合法输入明确配置，才认为可供业务使用。
    // 【教师解释】就绪由两个“合法配置标志”的逻辑与决定：有占位值不等于可以发布。
    bool isReady() const {
        return titleConfigured_ && maxBytesConfigured_;
    }
};

// 【教师解释】程序从main开始执行；类定义不是按顺序运行的代码。
int main() {
    cout << boolalpha;   // 将 bool 显示为 true / false
    cout << "begin: 开始执行 main" << endl;

    cout << "\n=== 第一部分：自动调用构造函数和构造函数重载 ===" << endl;
    {
        // 与上一课“先定义再逐项设置”不同：创建时直接提供必要信息。
        // 【教师解释】创建lab1时三参数构造函数自动运行，初始化列表先初始化成员，然后执行构造日志。
        Assignment lab1("实验一", 1024, true);  // 三参数构造
        // 【教师解释】创建lab2时两参数构造函数自动运行；其报告要求由requireReport_(false)确定。
        Assignment lab2("实验二", 2048);        // 两参数构造

        cout << "实验一：" << lab1.getTitle()
             << "，上限=" << lab1.getMaxBytes()
             << " 字节，需报告=" << lab1.getRequireReport()
             << "，可使用=" << lab1.isReady() << endl;
        cout << "实验二：" << lab2.getTitle()
             << "，上限=" << lab2.getMaxBytes()
             << " 字节，需报告=" << lab2.getRequireReport()
             << "，可使用=" << lab2.isReady() << endl;

        cout << "\n=== 第二部分：对象独立性与非法修改 ===" << endl;
        // 【教师解释】修改lab2不影响lab1：普通数据成员每个对象各有一份。
        lab2.setMaxBytes(4096);
        cout << "修改实验二后：实验一上限=" << lab1.getMaxBytes()
             << "，实验二上限=" << lab2.getMaxBytes() << endl;

        // 【教师解释】传入0触发return false，lab1原有1024不变。
        bool changed1 = lab1.setMaxBytes(0);
        bool changed2 = lab1.setTitle("");
        cout << "设置零上限是否成功：" << changed1 << endl;
        cout << "设置空名称是否成功：" << changed2 << endl;
        cout << "修改失败后：实验一名称=" << lab1.getTitle()
             << "，上限=" << lab1.getMaxBytes() << endl;

        cout << "\n=== 第三部分：嵌套作用域与析构时机 ===" << endl;
        // 【作用域实验】进入内层花括号，观察temp的生命周期。
        {
            Assignment temp("临时作业", 512, false);
            cout << "inside: 内部作用域尚未结束" << endl;
        }   // 【作用域实验】右花括号结束temp作用域；注意temp的析构发生在下一条outside输出之前。
            // temp 在此处析构
        cout << "outside: 临时作业已经析构" << endl;

        cout << "\n=== 第四部分：非法构造参数与补全配置 ===" << endl;
        {
            // 【边界实验】名称空、上限0；构造仍执行，但标记为未就绪，而不是错误地认为业务配置完成。
            Assignment incomplete("", 0, true);
            cout << "创建后：名称=" << incomplete.getTitle()
                 << "，安全占位上限=" << incomplete.getMaxBytes()
                 << "，可使用=" << incomplete.isReady() << endl;
            // 【状态跟踪】先补全名称，只把titleConfigured_置true，isReady仍为false。
            cout << "补全名称：" << incomplete.setTitle("补交作业") << endl;
            // 【状态跟踪】再补全合法上限，两个标志都true，isReady才变true。
            cout << "补全上限：" << incomplete.setMaxBytes(512) << endl;
            cout << "补全后：名称=" << incomplete.getTitle()
                 << "，上限=" << incomplete.getMaxBytes()
                 << "，可使用=" << incomplete.isReady() << endl;
            // 【作用域实验】这里析构incomplete；日志显示补全后的当前名称。
        }   // incomplete 在此处析构

        cout << "即将离开外层作用域：注意实验二、实验一的析构顺序" << endl;
        // 【顺序实验】退出外层作用域时先析构后创建的lab2，再析构先创建的lab1。
    }   // lab2 后创建先析构，lab1 先创建后析构

    cout << "end: 外层作用域内的对象均已析构" << endl;

    // 适配 Dev-C++：运行后按回车退出，便于观察析构输出。
    // 【运行提醒】这是为了让Dev-C++控制台停下来供观察，不会影响前面已经发生的析构。
    cout << "\n按回车键退出程序..." << endl;
    cin.get();
    return 0;
}
