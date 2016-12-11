# encoding: utf-8
#!/usr/bin/env python
import smtplib
from email.mime.text import MIMEText
## send text email

## mail is the smtp sender
def sendMail(from_addr, to_addr, subject, message, mail):
	msg = MIMEText(message)
	msg["Subject"] = subject
	msg["From"] = from_addr
	msg["To"] = to_addr
	mail.sendmail(from_addr, to_addr, msg.as_string())
	return 0

def main():
	mail_server = 'smtp.126.com'
	mail_server_port = 25
	mail = smtplib.SMTP(mail_server, mail_server_port)
	mail.set_debuglevel(1)
	mail.starttls()
## 	use the profile below to send email
	mail.login("from@126.com", "password")

	msg = "mail.txt"
	from_addr = 'from@126.com'
	to_addr = 'to@126.com'
	subject = "the subject"
	sendMail(from_addr, to_addr, subject, msg, mail)
	mail.quit()

	return 0

if __name__ == "__main__":
	main()
