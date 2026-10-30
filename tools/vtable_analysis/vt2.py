from load import *
def in_text(v): return 0x10001000<=v<0x10020000
runs=[]
a=0x10020000
end=0x100256D2
while a<end:
    n=0
    while a+4*n<end and in_text(u32(a+4*n)): n+=1
    if n>=3:
        runs.append((a,n)); a+=4*n
    else: a+=4
for a,n in runs:
    prev=u32(a-4)
    print(hex(a),n,'prev=%08x'%prev)
print(len(runs))
