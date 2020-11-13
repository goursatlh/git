#!/usr/bin/env python3
import smtplib
from email.mime.text import MIMEText
from email.header import Header
from email import encoders
from email.utils import parseaddr, formataddr

server = "smtp.sina.com"
fromaddr= "goursatlh@sina.com"
toaddr = "doris.liu_1@philips.com"
password = "5f27958810ccac71" # not mail password, but the mail shouquanma

def _format_addr(s):
    name, addr = parseaddr(s)
    return formataddr((Header(name, 'utf-8').encode(), addr))

# create the mail
message = MIMEText('from:goursatlh@sina.com hello Doris, send by Python...', 'plain', 'utf-8')
message['From'] = _format_addr('Python Tesst  <%s>' % fromaddr)
message['To'] = _format_addr('hear <%s>' % toaddr)
message['Subject'] = Header('Hello ', 'utf-8').encode()

# send the mail
s = smtplib.SMTP_SSL(server, 465)
s.set_debuglevel(1)
s.login(fromaddr, password)
s.sendmail(fromaddr,toaddr,message.as_string())
s.quit()
