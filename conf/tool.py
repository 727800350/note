r"""Command-line tool to validate and pretty-print JSON

Usage::

    $ echo '{"json":"obj"}' | python -m json.tool
    {
        "json": "obj"
    }
    $ echo '{ 1.2:3.4}' | python -m json.tool
    Expecting property name enclosed in double quotes: line 1 column 3 (char 2)

https://blog.xyxu.top/2018-05-17-python-json-tool.html
/usr/lib/python2.7/json/tool.py
"""
import sys
import json
import codecs

def main():
    if len(sys.argv) == 1:
        infile = sys.stdin
        outfile = sys.stdout
    elif len(sys.argv) == 2:
        infile = open(sys.argv[1], 'rb')
        outfile = sys.stdout
    elif len(sys.argv) == 3:
        infile = open(sys.argv[1], 'rb')
        outfile = open(sys.argv[2], 'wb')
    else:
        raise SystemExit(sys.argv[0] + " [infile [outfile]]")
    with infile:
        try:
            obj = json.load(infile)
        except ValueError, e:
            raise SystemExit(e)
    with outfile:
        # json.dump(obj, outfile, sort_keys=True, indent=4, separators=(',', ': '))
        s = json.dumps(obj, sort_keys=True, indent=4, separators=(',', ': '), ensure_ascii=False)
        outfile.write(codecs.encode(s, 'utf-8'))
        outfile.write('\n')


if __name__ == '__main__':
    main()
