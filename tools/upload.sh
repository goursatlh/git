#! /usr/bin/expect -f
set timeout 300
if {$argc != 2} {
        send_user "Wrong input, Useage: ./upload.sh load_name server_name\n"
        exit 0
}

send_user "Download load from server......\n"
spawn scp ts@10.89.255.148:/tftpboot/[lindex $argv 0] .
expect ":"
send "ts\n"
interact

if {[lindex $argv 1] == 1} {
    send_user "Upload to sercomm......\n"
    spawn scp [lindex $argv 0] wwang@216.234.147.51:/home/wwang/image
    expect ":"
    send "ExtractionThird41\n"
    interact
} elseif {[lindex $argv 1] == 2} { 
    send_user "Upload to unihan......\n"
    spawn scp [lindex $argv 0] wwang@216.234.147.108:/home/wwang/image
    expect ":"
    send "ExtractionThird41\n"
    interact
} else {
    send_user "Bad server name\n"
}

