#!/usr/bin/env python3.5
import mysql.connector

conn = mysql.connector.connect(user='root', password='wt', database='test')
cursor = conn.cursor()

cursor.execute('drop table user')
cursor.execute('create table user(id varchar(20) primary key, name varchar(20))')

#add
cursor.execute('insert into user (id, name) values (%s, %s)', ['1', 'Michael'])
cursor.execute('insert into user (id, name) values (%s, %s)', ['2', 'Walter'])
print(cursor.rowcount)

conn.commit()
cursor.close()

cursor = conn.cursor()
cursor.execute('select * from user where id = %s', ('1',))
vaules = cursor.fetchall()
print(vaules)

#update
cursor.execute('update user set name="Michael2" where id=1')
cursor.execute('select * from user')
vaules = cursor.fetchall()
print(vaules)
conn.commit()
cursor.execute('select * from user')
vaules = cursor.fetchall()
print(vaules)

#delete
cursor.execute('delete from user where id=1')
cursor.execute('select * from user')
vaules = cursor.fetchall()
print(vaules)

#cursor.execute('drop table user')
cursor.close()
conn.close()

