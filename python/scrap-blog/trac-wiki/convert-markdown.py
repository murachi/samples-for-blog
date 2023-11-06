import os
import re
import yaml

def touchWantedImageFile(outdir, img_file):
    # ファイル名が / を含む場合は何もしない
    if re.search(r"\/", img_file):
        return

    outpath = os.path.join(outdir, f"{img_file}.wanted")
    with open(outpath, "w") as fout:
        print("WANTED!!", file = fout)

def convertInlineMarkup(outdir, trac_title, line):
    seek_pattern = re.compile(r"[\w\s]+")
    escape_pattern = re.compile(r"\!(\w+|\W)")
    link_pattern = re.compile(r"\[(\w+\:|#)(\S+)\s+(.+?)\]")
    img_pattern = re.compile(r"\[\[Image\((\S+?)(?:,\s*.*?)?\)\]\]")
    footnote_pattern = re.compile(r"\[\[FootNote\((.*?)\)\]\]")
    em_pattern_1 = re.compile(r"''([^'].*?)''")
    em_pattern_2 = re.compile(r"\/\/([^\/].*?)\/\/")
    strong_pattern_1 = re.compile(r"'''([^'].*?)'''")
    strong_pattern_2 = re.compile(r"\*\*([^\*].*?)\*\*")
    em_strong_pattern_1 = re.compile(r"'''''([^'].*?)'''''")
    em_strong_pattern_2 = re.compile(r"\*\*\/\/(.+?)\/\/\*\*")
    code_pattern = re.compile(r"\`([^\`]+)\`")
    pass_pattern = re.compile(r"\W")

    result = ''

    while (line):
        m = seek_pattern.match(line)
        if m:
            result += m[0]
            line = line[m.end():]
        m = escape_pattern.match(line)
        if m:
            line = line[m.end():]
            content = m[1]
            m = seek_pattern.match(content)
            result += ('' if m else '\\') + content
            continue
        m = link_pattern.match(line)
        if m:
            line = line[m.end():]
            (scheme, path, content) = m.groups()
            url = f'/{trac_title}/{path}' if scheme == 'wiki' \
                else f'#{path}' if scheme == '#' \
                else f'{scheme}:{path}'
            result += f'[{content}]({url})'
            continue
        m = img_pattern.match(line)
        if m:
            line = line[m.end():]
            result += f'![image]({m[1]})'
            touchWantedImageFile(outdir, m[1])
            continue
        m = footnote_pattern.match(line)
        if m:
            line = line[m.end():]
            result += f'<small> ({convertInlineMarkup(outdir, trac_title, m[1])}) </small>'
            continue
        m = em_pattern_1.match(line) or em_pattern_2.match(line)
        if m:
            line = line[m.end():]
            result += f'*{convertInlineMarkup(outdir, trac_title, m[1])}*'
            continue
        m = strong_pattern_1.match(line) or strong_pattern_2.match(line)
        if m:
            line = line[m.end():]
            result += f'**{convertInlineMarkup(outdir, trac_title, m[1])}**'
            continue
        m = em_strong_pattern_1.match(line) or em_strong_pattern_2.match(line)
        if m:
            line = line[m.end():]
            result += f'***{convertInlineMarkup(outdir, trac_title, m[1])}***'
            continue
        m = code_pattern.match(line)
        if m:
            line = line[m.end():]
            result += f'`{m[1]}`'
            continue
        m = pass_pattern.match(line)
        if m:
            result += m[0]
            line = line[m.end():]

    return result


def tracWiki2Markdown(outdir, trac_title, text):
    itemize_pattern = re.compile(r"^(\s+)\*\s+(.+?)$")
    numberize_pattern = re.compile(r"^(\s+)(?:\d+|[a-zA-Z])\.\s+(.+?)$")
    codeblock_pattern = re.compile(r"^\s*\{\{\{(?:\s*#\!(\w+))?")
    hashbang_pattern = re.compile(r"^#\!(\w+)")
    codeblock_end_pattern = re.compile(r"\s*\}\}\}$")
    heading_pattern = re.compile(r"^(\=+)\s+(.+?)\s+\=+(?:\s*#(\S+))?")
    lines = text.split('\n')
    result = ''
    indent = 0
    while (lines):
        line = lines.pop(0)
        line = line.rstrip()
        if line == '[[PageOutline]]':
            continue
        if not line:
            indent = 0
            result += '\n'
            continue
        m = itemize_pattern.match(line)
        if m:
            indent = len(m[1]) - 1 + 2
            result += f"{' ' * (indent - 2)}* {convertInlineMarkup(outdir, trac_title, m[2])}\n"
            continue
        m = numberize_pattern.match(line)
        if m:
            indent = len(m[1]) - 1 + 3
            result += f"{' ' * (indent - 3)}1. {convertInlineMarkup(outdir, trac_title, m[2])}\n"
            continue
        m = codeblock_pattern.match(line)
        if m:
            if m[1]:
                code_type = m[1]
            else:
                m = hashbang_pattern.match(lines[0])
                if m:
                    code_type = m[1]
                    lines.pop(0)
                else:
                    code_type = ''
            result += f"{' ' * indent}```{code_type}\n"
            while (lines):
                line = lines.pop(0)
                line = line.rstrip()
                m = codeblock_end_pattern.match(line)
                if m:
                    result += f"{' ' * indent}```\n"
                    break
                result += f"{' ' * indent}{line}\n"
            continue
        m = heading_pattern.match(line)
        if m:
            indent = 0
            heading_level = len(m[1])
            if m[3]:
                result += f'<h{heading_level} id="{m[3]}">{convertInlineMarkup(outdir, trac_title, m[2])}</h{heading_level}>\n'
            else:
                result += f"{'#' * heading_level} {convertInlineMarkup(outdir, trac_title, m[2])}\n"
            continue

        result += f"{' ' * indent}{convertInlineMarkup(outdir, trac_title, line.strip())}\n"

    return result.rstrip()

wikidata_filepattern = re.compile(r"^(\w+?)_wiki\.yaml$")

for file, path in ((n.name, n.path) for n in os.scandir("src")):
    m = wikidata_filepattern.match(file)
    if not m:
        continue
    trac_title = m[1]
    with open(path, "r") as fin:
        wiki_data = yaml.safe_load(fin)
    for name, text, author in ((n["name"], n["text"], n["author"]) for n in wiki_data):
        outdir = os.path.join("out", trac_title, *(name.split("/")[:-1]))
        os.makedirs(outdir, mode = 0o755, exist_ok = True)
        outpath = os.path.join(outdir, name.split("/")[-1] + ".md")
        with open(outpath, "w") as fout:
            fout.write(f"[author: {author}]\n{tracWiki2Markdown(outdir, trac_title, text)}\n")
