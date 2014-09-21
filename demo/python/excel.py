#encoding: utf-8
#!/usr/bin/python

import time, datetime
import xlrd, xlwt
from xlutils.copy import copy

data = xlrd.open_workbook('demo.xls') #注意这里的workbook首字母是小写

## 查看文件中包含sheet的名称
data.sheet_names()

## 得到第一个工作表,或者通过索引顺序 或 工作表名称
table = data.sheets()[0]
table = data.sheet_by_index(0)
table = data.sheet_by_name(u'Sheet1')

## 获取行数和列数
nrows = table.nrows
ncols = table.ncols

## 获取整行和整列的值(数组)
table.row_values(i)
table.col_values(i)

## 循环行,得到索引的列表
for rownum in range(table.nrows):
	print table.row_values(rownum)

## 单元格
cell_A1 = table.cell(0,0).value
cell_C4 = table.cell(2,3).value

## 分别使用行列索引
cell_A1 = table.row(0)[0].value
cell_A2 = table.col(1)[0].value

## python 中取得到excel 日期是一个浮点数(一个小数, 从1900年1月1日开始的天数), 而python中的日期也是计从某一个日期开始的天数, 但是和excel的基准不一样
## 把它转换成一个正确的日期字符串
## example: 41778 就是 2014-05-19
## http://blog.csdn.net/suofiya2008/article/details/5587986
__s_date = datetime.date(1899, 12, 31).toordinal() - 1
def getdate(date):
	if isinstance(date, float):
		date = int(date )
	d = datetime.date .fromordinal(__s_date + date )
	return d.strftime("%Y-%m-%d")
