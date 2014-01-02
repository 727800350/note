#!/usr/bin/env python 
## mple 4-1. Retrieving email using POP3 
import poplib 
username = 'buaa3924@126.com' 
password = 'ecpkn3924' 
mail_server = 'pop3.126.com' 
p = poplib.POP3(mail_server) 
p.user(username) 
p.pass_(password) 
## print p.list()
## print p.__dict__
for msg_id in  p.list()[1]:
		print msg_id     
		outf = open('%s.eml' % msg_id, 'w')     
		outf.write('\n'.join(p.retr(msg_id)[1]))     
		outf.close() 
p.quit()
