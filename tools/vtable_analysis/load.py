import re,struct,sys
RD='/mnt/user-data/attachments/960b120b-c112-5767-8fd9-161f0db5f859/2c77d870-rdata.txt'
DIS='/home/claude/freevgui2/original/vgui2_full_disasm.txt'
mem={}  # addr->byte
sections=[]
cur=None
for line in open(RD,encoding='latin-1'):
    m=re.match(r'\s+([0-9A-F]{8}): ((?:[0-9A-F]{2} ?){1,16})',line)
    if m:
        a=int(m.group(1),16)
        bs=[int(x,16) for x in m.group(2).split()]
        for i,b in enumerate(bs): mem[a+i]=b
def u32(a):
    return struct.unpack('<I',bytes(mem.get(a+i,0) for i in range(4)))[0]
def has(a,n=4): return all((a+i) in mem for i in range(n))
def cstr(a,maxn=200):
    out=[]
    for i in range(maxn):
        b=mem.get(a+i)
        if b is None or b==0: break
        out.append(chr(b))
    return ''.join(out)
dis={}
for line in open(DIS,encoding='latin-1'):
    m=re.match(r'([0-9a-f]{8}):\t([0-9a-f ]+?)\s*\t(.*)',line)
    if m: dis[int(m.group(1),16)]=(m.group(2).strip(),m.group(3).strip())
if __name__=='__main__':
    print(len(mem),'rdata bytes', hex(min(mem)),hex(max(mem)), len(dis),'insns')
