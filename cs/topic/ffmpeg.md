# [ffmpeg](http://blog.csdn.net/jixiuffff/article/details/5709976)
格式转换(将file.avi 转换成output.flv)
`ffmpeg -i file.avi output.flv`

[doc](https://www.ffmpeg.org/ffmpeg.html)

To refer to input files in options, you must use their indices(0-based). 
E.g. the first input file is 0, the second is 1, etc. 
Similarly, streams within a file are referred to by their indices. E.g. 2:3 refers to the fourth stream in the third input file. 

By default, ffmpeg includes only one stream of each type (video, audio, subtitle) present in the input files and adds them to each output file.
You can disable some of those defaults by using the -vn/-an/-sn options.

-codec:a:1 ac3 contains the a:1 stream specifier, which matches the second audio stream. Therefore, it would select the ac3 codec for the second audio stream.
-b:a 128k matches all audio streams.

## map
select the stream with index 2 from input file a.mov and stream with index 6 from input b.mov, and copy them to the output file out.mov:
`ffmpeg -i a.mov -i b.mov -c copy -map 0:2 -map 1:6 out.mov`

To select all video and the third audio stream from an input file:
`ffmpeg -i INPUT -map 0:v -map 0:a:2 OUTPUT`

## 合并视频与音频
现在有个视频video.avi,有个音频 audio.mp3,将其合并成output.avi
```
ffmpeg -i video.avi -vcodec copy -an video2.avi   
ffmpeg -i video2.avi -i audio.mp3 -vcodec copy -acodec copy output.avi
```
- -i 表示输入文件
- -vcodec copy: vcodec codec(output), set the video codec, an alias for -codec:v
- -acodec copy 这个说的应该是音频跟上面一样
- -an: 表示disable audio recording, 表示去掉video.avi 原有的音频, 相应的-vn, 表示disable video recording

Stream copy is a mode selected by supplying the copy parameter to the -codec option. 
It makes ffmpeg omit the decoding and encoding step for the specified stream, so it does only demuxing and muxing. 
It is useful for changing the container format or modifying container-level metadata. 

## 声音与视频的分离

1. `ffmpeg -i 人生若只如初见.flv -vn r.mp3`: 从flv 文件中提取声音并保存为mp3 格式
1. `ffmpeg -i 人生若只如初见.flv -an r.flv`: 只留视频不留声音

如果你觉得mp3 文件有点大, 想变小一点那么可以通过-ab 选项改变音频的比特率(bitrate)  
`ffmpeg -i input.mp3 -ab 128 output.mp3`    //这里将比特率设为128

可以用file 命令查看一下源文件的信息

## 切头去尾
`ffmpeg -ss 00:00:10 -t 00:01:22 -i 五月天-突然好想你.mp3  output.mp3` 只要从第10秒开始截取,共截取1:22时长的内容

## 视频文件的连接
好像必须先将文件转成mpg, dv 等格式的文件后才能进行连接

连接复数的AVI影片档之范例(在此范例中须一度暂时将AVI档转换成MPEG-1档(MPEG-1, MPEG-2 PS, DV格式亦可连接))
```
## 上面将input1.avi, input2.avi 合并成outputfile.avi
ffmpeg -i input1.avi -sameq inputfile_01.mpg -r 20
ffmpeg -i input2.avi -sameq inputfile_02.mpg  -r 20
cat inputfile_01.mpg inputfile_02.mpg > inputfile_all.mpg
ffmpeg -i inputfile_all.mpg -sameq outputfile.avi
```
- -sameq 表示相同的质量(可能指的是画面, 不太清楚)
- -r 指频率, frame rate

