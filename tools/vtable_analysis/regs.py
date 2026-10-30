from load import *
import re
keys=sorted(dis)
regs=[]
for a in keys:
    ins=dis[a][1]
    if ins.startswith('call') and '0x10017510' in ins:
        # look back up to 6 insns for pushes and mov ecx
        j=keys.index(a)
        pushes=[];ecx=None
        for k in keys[max(0,j-6):j]:
            t=dis[k][1]
            m=re.match(r'push\s+(0x[0-9a-f]+)',t)
            if m: pushes.append(int(m.group(1),16))
            m=re.match(r'mov\s+ecx,(0x[0-9a-f]+)',t)
            if m: ecx=int(m.group(1),16)
        regs.append((a,pushes,ecx))
for a,p,e in regs:
    if len(p)>=2:
        name=cstr(p[-2]); fn=p[-1]
    else: name='?';fn=0
    print('%08x name=%r(@%x) createfn=%x ecx=%s'%(a,name,p[-2] if len(p)>=2 else 0,fn,hex(e) if e else None))
