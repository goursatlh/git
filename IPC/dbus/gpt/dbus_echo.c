#include<stdio.h> 
#include<stdlib.h> 
#include<dbus/dbus.h> 
 
 
//��Ϣѭ�����������ṩ������������Add��Quit;
DBusHandlerResult dbus_message_handler(DBusConnection* conn, DBusMessage* msg, void* user_data) 
{ 
	if (dbus_message_is_method_call(msg, "com.example.Calculator", "Add")) 
	{
		// ���� Add ����
		DBusMessageIter iter; 
		int a, b, sum;
		dbus_message_iter_init(msg, &iter); 
		dbus_message_iter_get_basic(&iter, &a); 
		dbus_message_iter_next(&iter);
		dbus_message_iter_get_basic(&iter, &b); 
		sum = a + b; 
		DBusMessage* reply = dbus_message_new_method_return(msg); 
		DBusMessageIter reply_iter; 
		dbus_message_iter_init_append(reply, &reply_iter); 
		dbus_message_iter_append_basic(&reply_iter, DBUS_TYPE_INT32, &sum);
		dbus_connection_send(conn, reply,NULL); 
		dbus_message_unref(reply); 
		return DBUS_HANDLER_RESULT_HANDLED; 
	}
	else if (dbus_message_is_method_call(msg, "com.example.Calculator", "Quit"))
	{ 
		// ���� Quit ���� 
		dbus_connection_close(conn); 
		return DBUS_HANDLER_RESULT_HANDLED;
	} 
	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED; 
} 
 
//�յ���Ӧ����Ϣ����
void dbus_reply_handler(DBusPendingCall* pending, void* user_data) 
{
	int* sum = (int*)user_data;
	DBusMessage* reply =dbus_pending_call_steal_reply(pending);
	DBusMessageIter iter;
	if (reply)
	{
		if (dbus_message_iter_init(reply, &iter))
		{ 
			dbus_message_iter_get_basic(&iter, sum); 
			printf("The sum is: %d\n", *sum);
		} 
		dbus_message_unref(reply);
	} 
	dbus_pending_call_unref(pending);
}
 
int main() 
{ 
    DBusError error; 
    DBusConnection* conn; 
    DBusMessage* msg; 
    DBusPendingCall* pending;
    int sum = 0;
    DBusObjectPathVTable dbus_interface = { .message_function = &dbus_message_handler }; 
    dbus_error_init(&error);
 
    // �������Ự���ߵ�����
    conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) 
    {
      fprintf(stderr, "Error connecting to system bus: %s\n", error.message);
      dbus_error_free(&error);
      return EXIT_FAILURE;
    }
 
    // ע�����·������Ϣ������
    dbus_bus_request_name(conn, "com.example.Calculator", 0, &error);       //��dbus���빫����
    if (dbus_error_is_set(&error))
    {
      fprintf(stderr, "Error requesting name: %s\n", error.message);
      dbus_error_free(&error);
      return EXIT_FAILURE;
    }
	dbus_connection_register_object_path(conn, "/com/example/Calculator", &dbus_interface, NULL); 
 
    // ����DBus��Ϣ������������ӵ���Ϣ��
	int a = 2, b = 3;
    msg = dbus_message_new_method_call("com.example.Calculator", "/com/example/Calculator", "com.example.Calculator", "Add");
    DBusMessageIter iter;
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT32, &a);     
    dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT32, &b);
 
    // ����DBus��Ϣ���ȴ�Ӧ��
    dbus_connection_send_with_reply(conn, msg, &pending, -1);
    dbus_message_unref(msg);
    dbus_pending_call_set_notify(pending, dbus_reply_handler, &sum, NULL);     //��Ӧ����Ϣ�ŵ�dbus_reply_handlerȥ����
 
    while (dbus_connection_read_write_dispatch(conn, -1)) {
 
    }
	while(1);
    dbus_connection_unref(conn);    //��������
 
    return EXIT_SUCCESS;
}
