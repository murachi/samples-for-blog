import multiprocessing as mp
import os, sys

def hoge():
    print(os.getresuid())
    print("\n".join(sys.path))

if __name__ == '__main__':
    #mp.set_start_method('spawn')
    ctx = mp.get_context('spawn')
    p = ctx.Process(target=hoge)
    p.start()
    p.join()
    print(p.exitcode)

    os.setuid(1002)
    p = ctx.Process(target=hoge)
    p.start()
    p.join()
    print(p.exitcode)

