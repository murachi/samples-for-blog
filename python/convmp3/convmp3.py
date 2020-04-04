import os
from pydub import AudioSegment

def convSafeName(name):
    ng_chars = r'\:*?"<>|'
    return name.translate(str.maketrans(ng_chars, "_" * len(ng_chars)))

inroot_dir = 'ミュージック'
outroot_dir = 'mp3'

src_extend = '.flac'
dist_extend = '.mp3'

walk = os.walk(os.path.join(os.path.expanduser('~'), inroot_dir))
for src_path, dirs, files in walk:
    print(src_path)
    dist_path = convSafeName(src_path.replace(inroot_dir, outroot_dir))
    os.makedirs(dist_path, mode = 0o755, exist_ok = True)
    for src_file in [n for n in files if n.endswith(src_extend)]:
        dist_file = convSafeName(src_file.replace(src_extend, dist_extend))
        dist_fullpath = os.path.join(dist_path, dist_file)
        if os.path.exists(dist_fullpath):
            continue
        print('  - {0} -> {1}'.format(src_file, dist_file))
        music = AudioSegment.from_file(os.path.join(src_path, src_file), 'flac')
        music.export(dist_fullpath, format = 'mp3', parameters = ['-ar', '44100', '-b:a', '256k'])
