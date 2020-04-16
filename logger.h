/* @desc:
 *  1. 调试日志框架，支持stdout输出和输出到日志文件中;
 *  2. 支持设置日志级别
 *  3. 支持日志文件重写
 *  4. 线程安全
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#define LOGGER_LV_NONE 0
#define LOGGER_LV_TRACE 1
#define LOGGER_LV_DEBUG 2
#define LOGGER_LV_INFO  4
#define LOGGER_LV_WARN  8
#define LOGGER_LV_ERROR 16
#define LOGGER_LV_FATAL 32
#define LOGGER_LV_ALL 63


/* @func:
 *	打印调试日志信息
 */
void logger_dump(void);

/* @func:
 *	功能性测试
 */
void logger_test(void);

/* @func:
 *	写日志文件
 */
void logger_do(unsigned char level, const char *file, const char *func, int line, const char *format, ...);

#define _TRACE_LOG(format, ...) logger_do(LOGGER_LV_TRACE, __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)
#define _DEBUG_LOG(format, ...) logger_do(LOGGER_LV_DEBUG, __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)
#define _INFO_LOG(format, ...)  logger_do(LOGGER_LV_INFO , __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)
#define _WARN_LOG(format, ...)  logger_do(LOGGER_LV_WARN , __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)
#define _ERROR_LOG(format, ...) logger_do(LOGGER_LV_ERROR, __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)
#define _FATAL_LOG(format, ...) logger_do(LOGGER_LV_FATAL, __FILE__, __FUNCTION__, __LINE__, format "\n", ##__VA_ARGS__)

#endif
