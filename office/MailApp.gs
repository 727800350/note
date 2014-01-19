MailApp.sendEmail(recipient, subject, body);

sendEmail(recipient, subject, body, options);

//Send an email with a file from Google Drive attached as a PDF.
var file = DriveApp.getFileById('1234567890abcdefghijklmnopqrstuvwxyz');
GmailApp.sendEmail('mike@example.com', 'Attachment example', 'Please see the attached file.', {
     attachments: [file.getAs(MimeType.PDF)],
     name: 'Automatic Emailer Script'
});


//options	Object	a JavaScript object that specifies advanced parameters, as listed below
//Advanced parameters
//
//Name	Type	Description
//attachments	BlobSource[]	an array of files to send with the email
//bcc	String	a comma-separated list of email addresses to BCC
//cc	String	a comma-separated list of email addresses to CC
//from	String	the address that the email should be sent from, which must be one of the values returned by getAliases()
//htmlBody	String if set, devices capable of rendering HTML will use it instead of the required body argument; you can add an optional inlineImages field in HTML body if you have inlined images for your email
//inlineImages	Object	a JavaScript object containing a mapping from image key (String) to image data (BlobSource); this assumes that the htmlBody parameter is used and contains references to these images in the format <img src="cid:imageKey" />
//name	String	the name of the sender of the email (default: the user's name)
//noReply	Boolean	if set to true, send the email from a generic no-reply email address to discourage recipients from responding to emails (default: false)
//replyTo	String	an email address to use as the default reply-to address (default: the user's email address)
