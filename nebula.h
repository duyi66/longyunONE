#ifndef __NEBULA_H__
#define __NEBULA_H__

#define CONFIG_SUPPORT_USB_4G  1
#define CONFIG_SUPPORT_USB_4G_SQLITE_APNS  1
#include <pthread.h>
typedef void                NB_VOID;
typedef signed char         NB_INT8;
typedef unsigned char       NB_UINT8;
typedef char                NB_CHAR;
typedef unsigned char       NB_UCHAR;
typedef signed short        NB_INT16;
typedef unsigned short      NB_UINT16;
typedef signed int          NB_INT;
typedef unsigned int        NB_UINT;
typedef signed long         NB_INT32;
typedef unsigned long       NB_UINT32;
typedef long long           NB_INT64;
typedef unsigned long long  NB_UINT64;

typedef struct _NB_DATETIME
{
    NB_UINT16 year;      /**< The number of years, larger than 1900 */
    NB_UINT16 mon;       /**< The number of months since January, in the range 1 to 12 */
    NB_UINT16 yday;      /**< The number of days since January 1, in the range 0 to 365 */
    NB_UINT16 mday;      /**< The day of the month, in the range 1 to 31 */
    NB_UINT16 wday;      /**< The number of days since Sunday, in the range 0 to 6 */
    NB_UINT16 hour;      /**< The number of hours past midnight, in the range 0 to 23 */
    NB_UINT16 min;       /**< The number of minutes after the hour, in the range 0 to 59 */
    NB_UINT16 sec;       /**< The number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds */
}NB_DATETIME;

typedef enum _NB_RET
{
    NB_R_SUCCESS = 0,
    NB_R_FAILURE = -1
}NB_RET;

typedef enum _NB_BOOL
{
    NB_B_FALSE,
    NB_B_TRUE
}NB_BOOL;

typedef pthread_mutex_t *NB_MUTEX;
#define NB_INVALID_MUTEX NULL
typedef pthread_t NB_THREAD;
#define NB_INVALID_THREAD (NB_THREAD)(-1)
typedef NB_RET (*NB_THREAD_PROC)(NB_UINT32, ...);
#endif