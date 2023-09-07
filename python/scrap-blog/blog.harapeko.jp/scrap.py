import urllib
import urllib.request as url_request
import re
import os
from bs4 import BeautifulSoup

# 記事一覧を取得し生成
contents = []
page = 1
try:
    while (True):
        url = "https://blog.harapeko.jp/"
        if (page > 1):
            url += f"page/{page}/"
        conn = url_request.urlopen(url)
        data = conn.read()
        soup = BeautifulSoup(data.decode("utf-8"), "lxml")
        title_elems = soup.select('#content > .post > h2 > a[rel="bookmark"]')
        contents += [{"subject": n.text.strip(), "url": n["href"]} for n in title_elems]
        page += 1
except urllib.error.HTTPError:
    pass

# 記事一覧のURLにアクセスし、本文を抜き取って出力する
for page in contents:
    conn = url_request.urlopen(page["url"])
    data = conn.read()
    soup = BeautifulSoup(data.decode("utf-8"), "lxml")
    # タイトル
    subject = soup.select('#content > .post > h2')[0].text.strip()
    # 本文
    entry = soup.select('#content > .post > .entry')[0]
    # 本文の最後の方のお知らせを除去
    entry.select('.postmetadata')[0].extract()
    # タグの一覧もいらないので除去
    parags = entry.find_all("p")
    parags.reverse()
    parags[0].extract()
    # 保存先ディレクトリを掘る
    page_url = re.compile("https://blog.harapeko.jp/(.+)/([^\\/]+)/").match(page["url"])
    outdir = f"out/{page_url[1]}"
    os.makedirs(outdir, mode=0o775, exist_ok=True)
    # 出力ファイル名を控えておく
    html_file = f"{page_url[2]}.html"
    # <a>, <img> タグのリンク先がローカルのものについて、以下の処理を行う。
    # - URL を改変
    # - 添付ファイルはダウンロード
    attached_file_pattern = re.compile("https?://blog.harapeko.jp/wp-content/(?:.+\\/)([^\\/]+)")
    internal_link_pattern = re.compile("https?://blog.harapeko.jp/(\\d{4}/\\d\\d/\\d\\d/.+)/")
    trac_wiki_pattern = re.compile("https?://developer.harapeko.jp/trac/original/([^\\/]+)/wiki/(.+)")
    attached_files = set()
    tag_kinds = (
        { "tag": "a", "url_attr": "href" },
        { "tag": "img", "url_attr": "src" },
    )
    for kind in tag_kinds:
        tags = entry.select(kind["tag"])
        for tag in tags:
            src_url = tag[kind["url_attr"]]
            m = attached_file_pattern.match(src_url)
            if m:
                tag[kind["url_attr"]] = m[1]
                if m[1] in attached_files:
                    continue
                attached_files.add(m[1])
                conn = url_request.urlopen(src_url)
                data = conn.read()
                with open(f"{outdir}/{m[1]}", "wb") as fout:
                    fout.write(data)
                continue
            m = internal_link_pattern.match(src_url)
            if m:
                tag[kind["url_attr"]] = f"/blog/{m[1]}.html"
                continue
            m = trac_wiki_pattern.match(src_url)
            if m:
                tag[kind["url_attr"]] = f"/trac-wiki/{m[1]}/{m[2]}.html"

    # HTMLファイルを吐き出して完了
    with open(f"{outdir}/{html_file}", "w") as fout:
        fout.write(f"<h1>{subject}</h1>\n\n{entry}\n\n<p>(転載元: はらぺこ日誌 〜株式会社はらぺこ 公式ブログ〜)</p>\n")
