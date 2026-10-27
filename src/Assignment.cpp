// Assignment.cpp —— 作业类完整版的实现

#include "Assignment.h"

#include <iostream>
#include <ostream>
#include <string>

// ==================== 构造函数 ====================

// 构造函数一：只给名称与大小上限，报告要求缺省为 false
Assignment::Assignment(const std::string& title, std::uint64_t maxBytes)
    : Assignment(title, maxBytes, false, "两参数")
{
}

// 构造函数二：三项信息都给全
Assignment::Assignment(const std::string& title, std::uint64_t maxBytes, bool requireReport)
    : Assignment(title, maxBytes, requireReport, "三参数")
{
}

// 私有构造函数：真正干活的版本，两个公开版本共用同一套初始化与校验
Assignment::Assignment(const std::string& title, std::uint64_t maxBytes,
                       bool requireReport, const char* tag)
    :
      // 初始化列表按声明顺序执行：title_ → maxBytes_ → requireReport_ → 两个标志
      // 非法输入不会抛错，而是换成安全占位值，同时把对应标志置 false
      title_(normalizeTitle(title)),
      maxBytes_(normalizeMaxBytes(maxBytes)),
      requireReport_(requireReport),
      titleConfigured_(hasValidTitle(title)),
      maxBytesConfigured_(hasValidMaxBytes(maxBytes))
{
    std::cout << "[构造·" << tag << "] " << title_ << '\n';
}

// ==================== 析构函数 ====================

Assignment::~Assignment()
{
    // 本类不持有需要手动释放的资源，这里只打印日志，方便观察析构时机与顺序
    std::cout << "[析构] " << title_ << '\n';
}

// ==================== 受控修改 ====================

bool Assignment::setTitle(const std::string& title)
{
    if (!hasValidTitle(title))
    {
        return false; // 非法修改：原值与 titleConfigured_ 都保持不变
    }
    title_ = title;
    titleConfigured_ = true; // 这次是“真正完成的业务配置”
    return true;
}

bool Assignment::setMaxBytes(std::uint64_t bytes)
{
    if (!hasValidMaxBytes(bytes))
    {
        return false; // 非法修改：原值与 maxBytesConfigured_ 都保持不变
    }
    maxBytes_ = bytes;
    maxBytesConfigured_ = true;
    return true;
}

void Assignment::setRequireReport(bool value)
{
    // 报告要求没有“占位值”，只有“需要 / 不需要”，因此不影响就绪标志
    requireReport_ = value;
}

// ==================== 查询 ====================

const std::string& Assignment::getTitle() const noexcept
{
    return title_;
}

std::uint64_t Assignment::getMaxBytes() const noexcept
{
    return maxBytes_;
}

bool Assignment::getRequireReport() const noexcept
{
    return requireReport_;
}

// ==================== 就绪判断 ====================

bool Assignment::isReady() const noexcept
{
    // 两个标志都来自合法输入，才认为可供业务使用；有占位值不算配置完成
    return titleConfigured_ && maxBytesConfigured_;
}

// ==================== 校验与规范化 ====================

bool Assignment::hasValidTitle(const std::string& title) noexcept
{
    // 名称不能为空，也不能只由空白字符组成（课件只判 empty，这里更严格）
    return title.find_first_not_of(" \t\r\n") != std::string::npos;
}

bool Assignment::hasValidMaxBytes(std::uint64_t bytes) noexcept
{
    // 0 字节没有意义，超过 1 GiB 也不符合课程要求
    return bytes > 0 && bytes <= kMaxAllowedBytes;
}

std::string Assignment::normalizeTitle(const std::string& title)
{
    return hasValidTitle(title) ? title : std::string(kPlaceholderTitle);
}

std::uint64_t Assignment::normalizeMaxBytes(std::uint64_t bytes) noexcept
{
    return hasValidMaxBytes(bytes) ? bytes : kPlaceholderMaxBytes;
}

// ==================== 输出 ====================

void Assignment::print(std::ostream& os) const
{
    os << "名称=" << title_
       << "，上限=" << maxBytes_ << " 字节"
       << "，需报告=" << (requireReport_ ? "true" : "false")
       << "，可使用=" << (isReady() ? "true" : "false") << '\n';
}

void Assignment::printConfig(std::ostream& os) const
{
    os << "  名称=" << title_
       << "（titleConfigured_=" << (titleConfigured_ ? "true" : "false") << "）\n"
       << "  上限=" << maxBytes_ << " 字节"
       << "（maxBytesConfigured_=" << (maxBytesConfigured_ ? "true" : "false") << "）\n"
       << "  需报告=" << (requireReport_ ? "true" : "false") << '\n'
       << "  可使用=" << (isReady() ? "true" : "false") << '\n';
}
