import urllib
import urllib.request as url_request
from io import StringIO
from lxml import etree

# 記事一覧を取得し生成
contents = []
page = 1
parser = etree.HTMLParser()
try:
    while (True):
        url = "https://blog.harapeko.jp/"
        if (page > 1):
            url += f"page/{page}/"
        conn = url_request.urlopen(url)
        data = conn.read()
        tree = etree.parse(StringIO(data.decode("utf-8")), parser)
        root = tree.getroot()
        title_elems = root.cssselect('#content > .post > h2 > a[rel="bookmark"]')
        contents += [{"subject": n.text, "url": n.attrib["href"]} for n in title_elems]
        page += 1
except urllib.error.HTTPError:
    pass

print(contents)
