[配置mutt做邮件客服端](http://home.ustc.edu.cn/~lixuebai/GNU/MuttConfig.html)

	msmtp --version

先单独使用msmtp发送测试: `msmtp -S`, 检测配置文件
查看当前的配置文件路径: `msmtp -P`

如果不能肯定SMTP 服务器是否支持TLS 加密或者是否有其它验证方式,用如下命令看看

	msmtp --host=smtp.gmail.com --serverinfo


## Simple demo
	echo "test" |mutt -a dbms_stats.txt -s my_first_test fyb@gmail.com

	echo -e "文字描述,可以带参数$allname" | mutt -a "附件(写好路径)" -a "附件2(写好路径)" aa@163.com,bb@yahoo.com.cn(收信人) -c cc@163.com,dd@163.com(抄送)

文件 testmail.txt

	To: wangchaogo1990@126.com
	From: wangchao1990@hotmail.com
	Subject: A test
	
	body this is test

Use **msmtp** to send

	cat testmail.txt | msmtp xxx@126.com

# Commands
## Moving Around in Menus
       j or Down       next-entry      move to the next entry
       k or Up	       previous-entry  move to the previous entry
       z or PageDn     page-down       go to the next page
       Z or PageUp     page-up	       go to the previous page
       = or Home       first-entry     jump to the first entry
       * or End	       last-entry      jump to the last entry
       q	       quit	       exit the current menu
       ?	       help	       list all keybindings for the current menu

## Editing Input Fields
  Mutt has a builtin line editor which is used as the primary way to
  input textual data such as email addresses or filenames.  The keys
  used to move around while editing are very similar to those of Emacs.

       <Tab>	       complete	       complete filename or alias
       ^T	       complete-query  complete address with query
       <Return>	       n/a	       finish editing

## Reading Mail - The Index and Pager
  Similar to many other mail clients, there are two modes in which mail
  is read in Mutt.  
  
- The first is the index of messages in the mailbox,
  which is called the **index** in Mutt.  
- The second mode is the display of the message contents. 
This is called the **pager**.

### The Message Index
	  c		  change to a different mailbox
	  D		  delete messages matching a pattern
	  d		  delete the current message
	  l		  show messages matching a pattern
	  o		  change the current sort method
	  O		  reverse sort the mailbox
	  q		  save changes and exit
	  T		  tag messages matching a pattern
	  t		  toggle the tag on a message
	  U		  undelete messages matching a pattern
	  u		  undelete-message
	  v		  view-attachments
	  x		  abort changes and exit
	  <Return>	  display-message
	  <Tab>		  jump to the next new message
	  @		  show the author's full e-mail address
	  $		  save changes to mailbox
	  /		  search
	  ESC /		  search-reverse
	  ^T		  untag messages matching a pattern

#### Status Flags
     D	message is deleted (is marked for deletion)
     d	message have attachments marked for deletion
     K	contains a PGP public key
     N	message is new
     O	message is old
     P	message is PGP encrypted
     r	message has been replied to
     S	message is PGP signed, and the signature is succesfully verified
     s	message is PGP signed
     !	message is flagged
     *	message is tagged

  Furthermore, the following flags reflect who the message is addressed
  to.  They can be customized with the ``$to_chars'' variable.

       +       message is to you and you only
       T       message is to you, but also to or cc'ed to others
       C       message is cc'ed to you
       F       message is from you
       L       message is sent to a subscribed mailing list

### The Pager
       <Return>	       go down one line
       <Space>	       display the next page (or next message if at the end of a message)
       -	       go back to the previous page
       /	       search for a regular expression (pattern)
       n	       search for next match
       ^	       jump to the top of the message
       $	       jump to the bottom of the message

  In addition, many of the functions from the index are available in the
  pager, such as delete-message or copy-message (this is one advantage
  over using an external pager to view messages).

### Miscellaneous Functions
  create-alias (default: a)
  
  edit (default: e)
  
  pipe-message (default: |)  
  Asks for an external Unix command and pipes the current or tagged
  message(s) to it.  The variables ``$pipe_decode'', ``$pipe_split'',
  ``$pipe_sep'' and ``$wait_key'' control the exact behaviour of this
  function.

  shell-escape (default: !)  
  Asks for an external Unix command and executes it.

## Sending Mail
	  m	  compose	  compose a new message
	  r	  reply		  reply to sender
	  g	  group-reply	  reply to all recipients
	  L	  list-reply	  reply to mailing list address
	  f	  forward	  forward message
	  b	  bounce	  bounce (remail) message

  Bouncing a message sends the message as is to the recipient you specify.  
  Forwarding a message allows you to add comments or modify the message you are forwarding.

  Forwarding can be done by including the original message in the new
  message's body (surrounded by indicating lines) or including it as a
  MIME attachment, depending on the value of the ``$mime_forward''
  variable.

  Once you have finished editing the body of your mail message, you are
  returned to the compose menu.	 The following options are available:

       a       attach-file	       attach a file
       A       attach-message	       attach message(s) to the message
       d       edit-description	       edit description on attachment
       D       detach-file	       detach a file

### Send mail offline
目录`/usr/share/doc/msmtp-1.4.31/scripts/msmtpqueue` 中的脚本,可以在离线状态下编辑邮件并发送(其实保存在`$MAILDIR/.msmtpqueue` 目录下),
上线时手工运行`msmtp-runqueue.sh`来实现真正的发送

1. msmtp-enqueue.sh  
   This script will save two files for each mail: one contains the mail, the
   other one contains the command line for msmtp (including options and the
   recipients). Thus you can use all msmtp options with this script.  
   Example (using Mutt):
	
		In your Mutt configuration file, replace 
	     set sendmail="/path/to/msmtp [options]"
	     with
	     set sendmail="/path/to/msmtp-enqueue.sh [options]"  
   This script cannot detect errors in its command line or the msmtp
   configuration file. You won't see error messages before msmtp-runqueue.sh 
   runs msmtp to send the mails. So test your msmtp configuration before using msmtp-enqueue.sh.

2. msmtp-runqueue.sh  
   Run this script when you are online to send all mails in the queue
   directory.  
   It will use the saved msmtp command line for each mail.
   Mails sent successfully will be deleted from the queue directory.
   Mails whose delivery failed will be left untouched; you may want to edit
   them, delete them by hand or simply run msmtp-runqueue.sh at a later time.

3. msmtp-listqueue.sh  
   This script lists all the mails in the queue.

# script
## [GUI 发送附件](http://blog.csdn.net/orzorz/article/details/5822680)
一般给别人发送个文档是怎么操作？打开邮件客户端，写好收件人，在层层目录结构中寻找文档，添加附件，发送。——太过麻烦！
在资源管理器里面选定多个文档鼠标右键发送可以不？写个小脚本就搞定！

脚本`~/bin/mutt.sh`

	#!/bin/bash
	# 用mutt来发送多个附件
	MAIL="mutt "
	while [ ! -z $1 ]
	do
	    MAIL=$MAIL"-a $1 "
	    shift
	done
	gnome-terminal -e "$MAIL"
同时做一个软连接:

	$ln -s ~/bin/mutt.sh ~/.gnome2/nautilus-scripts/mutt.sh

然后在nautilus里面选择多个文件,`右键->Scripts->mutt.sh` 即可弹出一个终端窗口让你输入收件人,开始发送

由于没有解决发送中文附件乱码问题，想了另外的方法来解决。主要是发给用windows的编码是GBK的同事们。

脚本`~/bin/mutt-zh.sh`

	#!/bin/bash
	# 用mutt来发送中文文件
	TIME=`date +%H%M%S`
	rm -rf /tmp/doc.zip
	rm -fr /tmp/$TIME
	mkdir -p /tmp/$TIME
	while [ ! -z $1 ]
	do
	    cp $1 /tmp/$TIME/$1
	    convmv -t GBK -f UTF-8 --notest -r /tmp/$TIME
	    shift
	done
	cd /tmp/$TIME
	zip /tmp/doc.zip -r * 
	EXE="mutt -a /tmp/doc.zip "
	gnome-terminal -e "$EXE"
