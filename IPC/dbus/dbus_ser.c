#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include <dbus/dbus.h>

#include "message.h"

DBusError dbus_error;

void print_dbus_error(char *str);

int main(int argc, char **argv)
{
    
    DBusConnection *conn;
    int ret;

    // 初始化错误信息
    dbus_error_init(&dbus_error);

    // 建立连接
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
    if(dbus_error_is_set(&dbus_error))
    {
        print_dbus_error("dbus_bus_get");
    }

    if(!conn)
    {
        exit(1);
    }

    // 为连接起名
    ret = dbus_bus_request_name(conn, SERVER_BUS_NAME,
    							DBUS_NAME_FLAG_DO_NOT_QUEUE, &dbus_error);
    if (dbus_error_is_set(&dbus_error))
    {
        print_dbus_error("dbus_bus_request_name");
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
        fprintf(stderr, "not primary owner, ret = %d\n", ret);
    }

    while (1)
    {
        if(!dbus_connection_read_write_dispatch(conn,-1))
        {
            fprintf(stderr, "Not connected now.\v");
            exit(1);
        }

        DBusMessage *msg;

        // 从总线上取出消息
        if((msg = dbus_connection_pop_message(conn)) == NULL)
        {
            fprintf(stderr, "Did not get message\n");
            continue;
        }

        // 对比消息中的方法接口名和方法名
        if(dbus_message_is_method_call(msg,INTERFACE_NAME,METHOD_NAME))
        {
            char *s;

            // 获取参数
            if(dbus_message_get_args(msg,&dbus_error,DBUS_TYPE_STRING,&s,DBUS_TYPE_INVALID))
            {
                printf("Received: %s\n", s);

                // 回复
                DBusMessage *reply;
                char answer[1024];

                assert(reply = dbus_message_new_method_return(msg));

                DBusMessageIter iter;
                // 把信号对应的相关参数压进去
                dbus_message_iter_init_append(reply, &iter);

                if (dbus_message_has_path(msg, OBJECT_PATH_NAME_YES))
                {
                    sprintf(answer, "Yes, %s", s);
                }
                else if (dbus_message_has_path(msg, OBJECT_PATH_NAME_NO))
                {
                    sprintf(answer, "No, %s", s);
                }
                else
                {
                    sprintf(answer, "No object found");
                }

                char *ptr = answer;
                assert(dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &ptr));

                assert(dbus_connection_send(conn, reply, NULL));
                dbus_connection_flush(conn);
                dbus_message_unref(reply);
            }
            else
            {
                print_dbus_error("Error getting message");
            }
        }
    }

    return 0;
}

void print_dbus_error(char *str)
{
    fprintf(stderr, "%s: %s\n", str, dbus_error.message);
    dbus_error_free(&dbus_error);
}

