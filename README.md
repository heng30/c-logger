# C 语言实现的同步阻塞日志库：
## 功能：
- 调试日志框架，支持stdout输出和输出到日志文件中;
- 支持设置日志级别
- 支持日志文件重写
- 线程安全

## 例子：
```c
#include "logger.h"

int main(void) {
    const char *str = "hello";
    const char *str2 = "world";

    logger_set_filepath(filepath);
    logger_set_size(1024 * 1024);
    logger_set_level(LOGGER_LV_WARN | LOGGER_LV_DEBUG);

    _TRACE_LOG("%s %s", str, str2);
    _DEBUG_LOG("%s %s", str, str2);
    _INFO_LOG("%s %s", str, str2);
    _WARN_LOG("%s %s", str, str2);
    _ERROR_LOG("%s %s", str, str2);
    _FATAL_LOG("%s %s", str, str2);

```
