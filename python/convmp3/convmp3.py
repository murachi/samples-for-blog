import os, re, shutil
from pydub import AudioSegment
import audio_metadata

def convSafeName(name):
    ng_chars = r'\:*?"<>|'
    return name.translate(str.maketrans(ng_chars, "_" * len(ng_chars)))

out_format = 'mp4'
out_codec = 'libfdk_aac'
out_bitrate = '256k'

inroot_dir = 'ミュージック'
#outroot_dir = out_format
outroot_dir = 'm4a'

src_extend = '.flac'
#dist_extend = '.{}'.format(out_format)
dist_extend = '.m4a'

default_group = 'normal'
special_groups = {
    'AliceSoft': 'abn',
    'AngeLink': 'abn',
    'NINJA ACTION TEAM': 'neta',
    'PING-Music Label': 'neta',
    'XROGER': 'neta',
    'chiyoda DJ union': 'neta',
    'luvtrax': 'neta',
    'しまじろう': 'kids',
    'はんだやレイブ': 'neta',
    'わんわん・ゆきちゃん・うーたん': 'kids',
    'サイドプロテア': 'neta',
    '再生ハイパーべるーヴ': 'neta',
    '八卦商会': 'neta',
    '不気味社音響基礎研究所': 'neta',
}

walk = os.walk(os.path.join(os.path.expanduser('~'), inroot_dir))
first_path, _, _ = walk.__next__()
re_dir = re.compile(r'{}/([^/]+)(?:/(.*))?'.format(first_path))

for src_path, dirs, files in walk:
    match = re_dir.match(src_path)
    artist, dir_suffix = match.groups() if match else ''
    group_dir = special_groups.get(artist, default_group)
    dist_path = convSafeName(os.path.join(os.path.expanduser('~'), outroot_dir, group_dir, artist))
    if dir_suffix:
        dist_path = os.path.join(dist_path, convSafeName(dir_suffix))
    print("{} -> {}".format(src_path, dist_path))
    os.makedirs(dist_path, mode = 0o755, exist_ok = True)

    # 変換対象ファイルを探して変換するループ
    for src_file in [n for n in files if n.endswith(src_extend)]:
        dist_file = convSafeName(src_file.replace(src_extend, dist_extend))
        dist_fullpath = os.path.join(dist_path, dist_file)
        if os.path.exists(dist_fullpath):
            continue
        print('  - {0} -> {1}'.format(src_file, dist_file))
        src_fullpath = os.path.join(src_path, src_file)
        music = AudioSegment.from_file(src_fullpath, 'flac')
        metadata = audio_metadata.load(src_fullpath)
        music.export(dist_fullpath,
            format = out_format, codec = out_codec, bitrate = out_bitrate,
            tags = {n[0]: ' - '.join(n[1]) for n in metadata['tags'].items()})
        #music.export(dist_fullpath, format = 'mp3', parameters = ['-ar', '44100', '-b:a', '256k'])

    # 変換後の形式と同じ形式のファイルがあればそのままコピーするループ
    for src_file in [n for n in files if n.endswith(dist_extend)]:
        dist_fullpath = os.path.join(dist_path, src_file)
        if os.path.exists(dist_fullpath):
            continue
        src_fullpath = os.path.join(src_path, src_file)
        shutil.copy2(src_fullpath, dist_fullpath)
