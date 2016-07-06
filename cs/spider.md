# Beautiful Soup
```
from bs4 import BeautifulSoup as bs
soup = BeautifulSoup(html)
soup = BeautifulSoup(open('index.html'))
```

```html
html = """
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title" name="dromouse"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1"><!-- Elsie --></a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
"""
```

## 四大对象种类
Beautiful Soup将复杂HTML文档转换成一个复杂的树形结构,每个节点都是Python对象,所有对象可以归纳为4种:

- Tag
- NavigableString
- BeautifulSoup
- Comment

### Tag
eg:

1. print soup.title
1. print soup.head
1. print soup.a, soup.html.body.a, soup.html.a, soup.body.a // 这几个都是同样的
1. print soup.p

查找的是在所有内容中的第一个符合要求的标签

print type(soup.a) 结果为: bs4.element.Tag

对于 Tag,它有两个重要的属性,是 name 和 attrs

- `print soup.p.attrs`: 得到`{'class': ['title'], 'name': 'dromouse'}`
- `print soup.p.['class']` 获得单个的class 属性, 也可以使用`soup.p.get('class')`
- `soup.p['class']="newClass"`: 修改属性
- `del soup.p['class']`: 删除属性

### NavigableString
- `print soup.p.string` 获取标签内部的文字
- `print type(soup.p.string)` 得到<class 'bs4.element.NavigableString'>

### BeautifulSoup
BeautifulSoup 对象表示的是一个文档的全部内容.大部分时候,可以把它当作 Tag 对象,是一个特殊的 Tag,我们可以分别获取它的类型,名称,以及属性.

### Comment
Comment 对象是一个特殊类型的 NavigableString 对象,其实输出的内容仍然不包括注释符号

- print soup.a `<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>`
- print soup.a.string `Elsie`
- print type(soup.a.string) `<class 'bs4.element.Comment'>`

a 标签里的内容实际上是注释,但是如果我们利用 .string 来输出它的内容,我们发现它已经把注释符号去掉了,所以这可能会给我们带来不必要的麻烦.
另外我们打印输出下它的类型，发现它是一个 Comment 类型，所以，我们在使用前最好做一下判断，判断代码如下
```python
if type(soup.a.string) == bs4.element.Comment:
    print soup.a.string
```

## 遍历文档树
直接子节点

- tag 的 .content 属性可以将tag的子节点以列表的方式输出
- tag 的 .children 它返回的不是一个 list，不过我们可以通过遍历获取所有子节点。

	```python
	for child in  soup.body.children:
		print child
	```

.descendants 属性可以对所有tag的子孙节点进行递归循环，和 children类似，我们也需要遍历获取其中的内容

.string 属性: 如果一个标签里面没有标签了，那么 .string 就会返回标签里面的内容。如果标签里面只有唯一的一个标签了，那么 .string 也会返回最里面的内容。如果有多个, 就会返回None.

- `print soup.title.string` 得到The Dormouse's story
- `print soup.head.string` 也得到The Dormouse's story, 因为head 里面就title 一个节点了

父节点

- .parent 属性
- .parents 属性可以递归得到元素的所有父辈节点, `for parent in object.parents`

兄弟节点

- `.next\_sibling, .previous\_sibling` 属性
- `.next\_siblings, .previous\_siblings` 全部兄弟节点, `for sibling in soup.a.next_siblings:`

前后节点

- `.next\_element, .previous\_element` 属性
- `.next\_elements, .previous\_elements` 全部前后节点, `for element in soup.a.next_elements:`

## 搜索文档树
`find_all(name, attrs, recursive, text, **kwargs)`

name 参数可以查找所有名字为 name 的tag,字符串对象会被自动忽略掉

- name 为单个tag: `soup.find_all('b')`
- name 为tag 的列表: `soup.find_all(["a", "b"])`: 会匹配 a, b 两个tag
- name 为正则表达式: `soup.find_all(re.compile("^b"))`: 会匹配body, b 等tag
- name 为方法(普通方法和lambda 都行): 方法只接受一个元素参数,如果这个方法返回 True 表示当前元素匹配并且被找到,如果不是则反回 False

attrs: 通过属性来查找

- `soup.find_all(id='link2')`
- `soup.find_all("a", class_="sister")`: 这里使用class_, 因为class 是python的关键字
- `soup.find_all(attrs={"data-foo": "value"})`: 也可以通过字典的方式来指定属性


text 参数可以搜搜文档中的字符串内容.与 name 参数的可选值一样, text 参数接受 字符串 , 正则表达式 , 列表, 返回boolean 值的方法

- soup.find_all(text="Elsie")

find_all() 方法返回全部的搜索结构,如果文档树很大那么搜索会很慢.如果我们不需要全部结果,可以使用 limit 参数限制返回结果的数量
调用tag的 find_all() 方法时,Beautiful Soup会检索当前tag的所有子孙节点,如果只想搜索tag的直接子节点,可以使用参数 recursive=False

其他的find 方法

- find_parents()  find_parent()
- find_next_siblings()  find_next_sibling()
- find_previous_siblings()  find_previous_sibling()
- find_all_next()  find_next()
- find_all_previous() 和 find_previous()

## CSS选择器
我们在写 CSS 时，标签名不加任何修饰，类名前加点，id名前加 #，在这里我们也可以利用类似的方法来筛选元素，用到的方法是 soup.select()，返回类型是 list

- print soup.select('a')
- print soup.select('.sister')
- print soup.select('#link1')
- print soup.select('p #link1')
- print soup.select("head > title"): 子标签
- print soup.select('a[class="sister"]'): 加了属性的
- print soup.select('p a[href="http://example.com/elsie"]')
 
