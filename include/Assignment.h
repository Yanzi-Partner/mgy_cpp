// Assignment.h —— 作业类完整版：构造函数重载 + 析构 + “占位值 / 已配置”两套信息。
//
// 本课要点：
//   1. 构造参数不合法时，对象照样能构造成功，只是往成员里放“安全占位值”，
//      并把对应的“已配置”标志置为 false；
//   2. isReady() 只认这两个标志：有占位值 ≠ 业务配置完成，也就不能发布；
//   3. 只有 setter 收到合法输入，才会把对应标志置真，对象才真正“就绪”。

#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

class Assignment
{
public:
    // ---------------- 类内常量：占位值与业务边界 ----------------
    static constexpr const char* kPlaceholderTitle = "待设置名称";    // 名称缺失时的安全占位值
    static constexpr std::uint64_t kPlaceholderMaxBytes = 1;          // 上限缺失时的安全占位值
    static constexpr std::uint64_t kMaxAllowedBytes = 1024ULL * 1024 * 1024; // 上限的硬上限：1 GiB

    // ---------------- 构造函数重载 ----------------
    // 两个公开构造函数只负责“选择模板”，真正的初始化与校验集中在私有版本里。
    Assignment(const std::string& title, std::uint64_t maxBytes);              // 报告要求缺省为 false
    Assignment(const std::string& title, std::uint64_t maxBytes,
               bool requireReport);                                            // 三项信息都给全

    // 析构函数：局部对象离开作用域时自动调用（本例只打印日志，没有需要释放的资源）
    ~Assignment();

    // ---------------- 受控修改 ----------------
    // 非法输入直接返回 false，原值与对应标志都不变
    [[nodiscard]] bool setTitle(const std::string& title);
    [[nodiscard]] bool setMaxBytes(std::uint64_t bytes);
    void setRequireReport(bool value);

    // ---------------- 查询 ----------------
    [[nodiscard]] const std::string& getTitle() const noexcept;
    [[nodiscard]] std::uint64_t getMaxBytes() const noexcept;
    [[nodiscard]] bool getRequireReport() const noexcept;

    // ---------------- 就绪判断 ----------------
    // 只有“名称和大小上限都由合法输入明确配置过”才算可供业务使用
    [[nodiscard]] bool isReady() const noexcept;

    // ---------------- 输出 ----------------
    void print(std::ostream& os) const;       // 单行摘要：值 + 是否可用
    void printConfig(std::ostream& os) const; // 详细：值 + 两个“已配置”标志 + 结论

private:
    // 私有构造函数：公开的两个版本都委托给它，初始化与校验规则只写一份
    Assignment(const std::string& title, std::uint64_t maxBytes,
               bool requireReport, const char* tag);

    // 合法性判断（供构造函数与 setter 共用）
    [[nodiscard]] static bool hasValidTitle(const std::string& title) noexcept;
    [[nodiscard]] static bool hasValidMaxBytes(std::uint64_t bytes) noexcept;

    // 规范化：非法输入换成安全占位值，保证成员里永远是可读的合法状态
    [[nodiscard]] static std::string normalizeTitle(const std::string& title);
    [[nodiscard]] static std::uint64_t normalizeMaxBytes(std::uint64_t bytes) noexcept;

    // ---------------- 数据成员（声明顺序 = 初始化顺序）----------------
    std::string title_;        // 作业名称：每个对象独立保存
    std::uint64_t maxBytes_;   // 文件大小上限（字节）
    bool requireReport_;       // 是否需要实验报告
    bool titleConfigured_;     // 是否提供了合法作业名称
    bool maxBytesConfigured_;  // 是否提供了合法大小上限
};
