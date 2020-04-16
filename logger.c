#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "logger.h"

static struct {
    char m_filepath[PATH_MAX]; // 文件路径
    unsigned char m_level; // 日志级别
    size_t m_size; // 文件重写大小
    pthread_mutex_t m_mutex;
} g_lg = {
    .m_filepath = {0},
    .m_size = 1024 * 1024,
    .m_level = LOGGER_LV_ALL,
    .m_mutex = PTHREAD_MUTEX_INITIALIZER,
};

static bool _is_dir_exist(const char *dir) {
    if (!dir)
        return false;

    struct stat st = {0};
    if (lstat(dir, &st))
        return false;

    return S_ISDIR(st.st_mode);
}

static bool _create_dir(const char *dir, mode_t mode) {
    if (!dir)
        return false;

    char dir_buf[PATH_MAX] = {0};
    char *start = dir_buf;
    char ch = 0;

    if (!strchr(dir, '/'))
        return false;

    snprintf(dir_buf, sizeof(dir_buf), "%s/", dir);

    while ((start = strchr(start, '/'))) {
        ch = *(start + 1);
        *(start + 1) = '\0';
        if (_is_dir_exist(dir_buf))
            goto next;

        if (-1 == mkdir(dir_buf, mode)) {
            printf("mkdir %s error, errno: %d - %s\n", dir_buf, errno,
                   strerror(errno));
            return false;
        }

    next:
        *(start + 1) = ch;
        start++;
    }
    return true;
}

static char _level_flag_get(unsigned char level) {
    switch (level) {
    case LOGGER_LV_TRACE:
        return 'T';
        break;
    case LOGGER_LV_DEBUG:
        return 'D';
        break;
    case LOGGER_LV_INFO:
        return 'I';
        break;
    case LOGGER_LV_WARN:
        return 'W';
        break;
    case LOGGER_LV_ERROR:
        return 'E';
        break;
    case LOGGER_LV_FATAL:
        return 'F';
        break;
    }
    return 'U';
}

/* @func:
 *  设置日志输出文件
 */
void logger_set_filepath(const char *filepath) {
    if (!filepath)
        return;

    char path[PATH_MAX] = {0};
    char *ptr = NULL;
    size_t len = 0;

    pthread_mutex_lock(&g_lg.m_mutex);

    snprintf(g_lg.m_filepath, sizeof(g_lg.m_filepath), "%s", filepath);

    if (ptr = strrchr(g_lg.m_filepath, '/'), ptr) {
        len = (size_t)(ptr - g_lg.m_filepath);
        memcpy(path, g_lg.m_filepath, len);
    }

    if (!_is_dir_exist(path))
        _create_dir(path, 0755);

    pthread_mutex_unlock(&g_lg.m_mutex);
}

/* @func:
 *  设置日志级别
 */
void logger_set_level(unsigned char level) {
    pthread_mutex_lock(&g_lg.m_mutex);
    g_lg.m_level = level;
    pthread_mutex_unlock(&g_lg.m_mutex);
}

/* @func:
 *  设置日志文件大小
 */
void logger_set_size(size_t size) {
    pthread_mutex_lock(&g_lg.m_mutex);
    g_lg.m_size = size;
    pthread_mutex_unlock(&g_lg.m_mutex);
}

/* @func:
 *	写日志文件
 */
void logger_do(unsigned char level, const char *file, const char *func,
               int line, const char *format, ...) {

    if (!(level & g_lg.m_level))
        return;

    time_t nt = 0;
    struct tm ltm = {0};
    char buf[64] = {0};
    FILE *fp = NULL;
    struct stat st = {0};
    va_list vl;
    char level_c = 0;

    level_c = _level_flag_get(level);

    pthread_mutex_lock(&g_lg.m_mutex);
    nt = time((time_t *)NULL);
    localtime_r(&nt, &ltm);

    strftime(buf, sizeof(buf), "%b %d %T", &ltm);
    if (g_lg.m_filepath[0] == '\0') {
        fp = stdout;
    } else {
        if (lstat(g_lg.m_filepath, &st) == -1) {
            st.st_size = 0;
        }

        if (g_lg.m_size > (size_t)st.st_size) {
            fp = fopen(g_lg.m_filepath, "ab+");
        } else {
            fp = fopen(g_lg.m_filepath, "wb");
        }

        if (!fp) {
            printf("fopen %s error, errno: %d - %s\n", g_lg.m_filepath, errno,
                   strerror(errno));
            goto err;
        }
    }

    va_start(vl, format);
    fprintf(fp, "[%c] [%s] [%s] [%s:%d]: ", level_c, buf, file, func, line);
    vfprintf(fp, format, vl);
    fflush(fp);
    va_end(vl);

    if (g_lg.m_filepath[0] != '\0')
        fclose(fp);

err:
    pthread_mutex_unlock(&g_lg.m_mutex);
}

/* @func:
 *		打印调试日志信息
 */
void logger_dump(void) {
    _TRACE_LOG("==================");
    _TRACE_LOG("path: %s", g_lg.m_filepath);
    _TRACE_LOG("size: %lu", g_lg.m_size);
    _TRACE_LOG("level: %u", g_lg.m_level);
    _TRACE_LOG("==================");
}

#ifdef _TESTING

void logger_test(void) {
    size_t i = 0;
    size_t max_num = 1024;
    pthread_t pt[10] = {0};
    const char *filepath = "/tmp/log/logger_test.dat";

    logger_dump();
    logger_set_filepath(filepath);
    logger_set_size(1024 * 1024);
    logger_set_level(LOGGER_LV_WARN | LOGGER_LV_DEBUG);

    for (i = 0; i < sizeof(pt) / sizeof(pt[0]); i++) {
        pthread_create(&pt[i], NULL, ({
            void *_(void *arg) {
                char *str = "hello";
                size_t j = 0;
                for (j = 0; j < max_num; j++) {
                    _TRACE_LOG("%s:%d", str, j);
                    _DEBUG_LOG("%s:%d", str, j);
                    _INFO_LOG("%s:%d", str, j);
                    _WARN_LOG("%s:%d", str, j);
                    _ERROR_LOG("%s:%d", str, j);
                    _FATAL_LOG("%s:%d", str, j);
                }
                return arg;
            };
            _;
        }),
                       NULL);
    }

    for (i = 0; i < sizeof(pt) / sizeof(pt[0]); i++)
        pthread_join(pt[i], NULL);

    /* unlink(filepath); */
}

int main(void) {
    logger_test();
    return 0;
}

#endif
