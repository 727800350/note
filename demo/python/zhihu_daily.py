#!/usr/bin/python
# -*- coding:utf-8 -*-
 
import urllib2
import re
import HTMLParser
import sys
 
reload(sys)
sys.setdefaultencoding('utf8')
 
def getHtml(url):
    header={'User-Agent' : 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:14.0) Gecko/20100101 Firefox/14.0.1','Referer' : '******'}
    request=urllib2.Request(url,None,header)
    response=urllib2.urlopen(request)
    text=response.read()
    return text
 
def getUrls(html):
    pattern = re.compile('http://daily.zhihu.com/story/(.*?)" >',re.S)
    items = re.findall(pattern,html)
    urls = []
    for item in items:
        urls.append('http://daily.zhihu.com/story/' + item)
    return urls
 
def getContent(url):
    html = getHtml(url)
    pattern = re.compile('<h1 class="headline-title">(.*?)</h1>')
    items = re.findall(pattern,html)
    print '********************************************************************************************************************************************'
    print '****************************************************'+items[0]+'****************************************************'
    print '********************************************************************************************************************************************'
    # print items[0]
    pattern = re.compile('<div.*?content">\n(.*?)</div>',re.S)
    items_withtag = re.findall(pattern,html)
    # print items_withtag[0]
    for item in items_withtag:
        for content in characterProcessing(item):
            print content
 
def characterProcessing(html):
    htmlParser = HTMLParser.HTMLParser()
    pattern = re.compile('<p>(.*?)</p>|<li>(.*?)</li>.*?',re.S)
    items = re.findall(pattern,html)
    result = []
    for index in items:
        if index != '':
            for content in index:
                tag = re.search('<.*?>',content)
                http = re.search('<.*?http.*?',content)
                html_tag = re.search('&',content)
                if html_tag:
                    content = htmlParser.unescape(content)
                if http:
                    continue
                elif tag:
                    pattern = re.compile('(.*?)<.*?>(.*?)</.*?>(.*)')
                    items = re.findall(pattern,content)
                    content_tags = ''
                    if len(items)>0:
                        for item in items:
                            if len(item)>0:
                                for item_s in item:
                                    content_tags = content_tags + item_s
                            else:
                                content_tags = content_tags + item_s
                        content_tags = re.sub('<.*?>','',content_tags)
                        result.append(content_tags)
                    else:
                        continue
                else:
                    result.append(content)
    return result
 
def main():
    url = "http://zhihudaily.ahorn.me"
    html = getHtml(url)
    urls = getUrls(html)
    for url in urls:
        getContent(url)
 
 
 
if __name__ == "__main__":
    main()
