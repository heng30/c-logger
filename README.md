# C 语言实现的同步阻塞日志库：
## 功能：
- 支持输出到标准输出和文件中
- 支持设置日志级别
- 支持日志文件重写
- 线程安全

## 例子：
```c
#include "logger.h"

int main(void) {
    const char *str = "hello";
    const char *str2 = "world";
    const char *filepath = "/tmp/logger.log";

    logger_set_filepath(filepath);
    logger_set_size(1024 * 1024);
    logger_set_level(LOGGER_LV_ALL);

    _TRACE_LOG("%s %s", str, str2);
    _DEBUG_LOG("%s %s", str, str2);
    _INFO_LOG("%s %s", str, str2);
    _WARN_LOG("%s %s", str, str2);
    _ERROR_LOG("%s %s", str, str2);
    _FATAL_LOG("%s %s", str, str2);
}
```


``` text
[T] [Apr 16 16:59:34] [m.c] [main:12]: hello world
[D] [Apr 16 16:59:34] [m.c] [main:13]: hello world
[I] [Apr 16 16:59:34] [m.c] [main:14]: hello world
[W] [Apr 16 16:59:34] [m.c] [main:15]: hello world
[E] [Apr 16 16:59:34] [m.c] [main:16]: hello world
[F] [Apr 16 16:59:34] [m.c] [main:17]: hello world
```
