#### 多线程出core
libcurl 有个很好的特性,它甚至可以控制域名解析的超时.
但是在默认情况下,它是使用alarm + siglongjmp 实现的. 用alarm在多线程下做超时,本身就几乎不可能.如果只是使用alarm,并不会导致程序崩溃,但是,再加上siglongjmp,就要命了(程序崩溃的很可怕,core中几乎看不出有用信息),
因为其需要一个`sigjmp_buf`型的全局变量, 多线程修改它. (通常情况下, 可以每个线程一个`sigjmp_buf` 型的变量, 这种情况下, 多线程中使用 siglongjmp 是没有问题的, 但是libcurl只有一个全局变量, 所有的线程都会用).

具体是类似`curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L)`(默认是0, 也就是不超市) 的超时设置, 导致alarm的使用(估计发生在域名解析阶段). 如前所述,这在多线程中是不行的.
解决方式是禁用掉alarm这种超时 `curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L)`(default 0)

这样,多线程中使用超时就安全了.但是域名解析就没了超时机制,碰到很慢的域名解析,也很麻烦.
文档的建议是 Consider building libcurl with c-ares support to enable asynchronous DNS lookups, which enables nice timeouts for name resolves without signals. c-ares 是异步的 DNS 解决方案.

- `CURLOPT_CONNECTTIMEOUT`: the maximum time in seconds that you allow the connection phase to the server to take. This only limits the connection phase, it has no impact once it has connected.
	Set to zero to switch to the default built-in connection timeout - 300 seconds. In unix-like systems, this might cause signals to be used unless `CURLOPT_NOSIGNAL` is set.
- `CURLOPT_TIMEOUT`: the maximum time in seconds that you allow the libcurl transfer operation to take. In unix-like systems, this might cause signals to be used unless `CURLOPT_NOSIGNAL` is set.

