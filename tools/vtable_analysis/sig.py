from load import *
import re
keys=sorted(dis); idx={k:i for i,k in enumerate(keys)}
def argbytes(fn,depth=0):
    if fn not in idx or depth>4: return None
    j=idx[fn]
    for k in keys[j:j+300]:
        t=dis[k][1]
        m=re.match(r'ret\s*(0x[0-9a-f]+)?',t)
        if m: return int(m.group(1),16) if m.group(1) else 0
        m=re.match(r'jmp\s+0x([0-9a-f]+)$',t)
        if m: return argbytes(int(m.group(1),16),depth+1)
    return None
def vtable(a,n):
    return [u32(a+4*i) for i in range(n)]
if __name__=='__main__':
    import sys
    vts={'Input+InputInternal(100204d0)':(0x100204d0,41),'System009(10021c64)':(0x10021c64,34),'KeyValues003(100221f4)':(0x100221f4,10),'ivgui006(10021e84)':(0x10021e84,25),'Scheme009?(10021508)':(0x10021508,14),'Localize003?(10020f24)':(0x10020f24,21),'Localize002?(10020ec4)':(0x10020ec4,18)}
    for name,(a,n) in vts.items():
        print('==',name)
        row=[]
        for i,f in enumerate(vtable(a,n)):
            ab=argbytes(f)
            row.append('%d:%s'%(i,'?' if ab is None else ab//4))
        print('  slot:args(dwords excluding this) ->',' '.join(row))
