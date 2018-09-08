## Address
```Python
import socket
import struct
>>> socket.ntohl(struct.unpack('I', socket.inet_aton("201.119.123.249"))[0])
3380050937L
;; 如果用i, 而不是I, 可能会出现负值

>>> socket.inet_ntoa(struct.pack('I',socket.htonl(980942848 )))
'58.120.0.0'
```

