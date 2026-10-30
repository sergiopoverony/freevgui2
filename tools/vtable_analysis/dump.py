from sig import *
vts=[('VGUI_Input004 / VGUI_InputInternal001 (same object)',0x100204d0,41),
('VGUI_System009',0x10021c64,34),('KeyValues003',0x100221f4,10),('VGUI_ivgui006',0x10021e84,25),
('VGUI_Scheme009',0x10021508,14),('VGUI_Localize003',0x10020f24,21),('VGUI_Localize002',0x10020ec4,18),
('VGUI_Panel007',0x100222f8,60),('VGUI_Surface026',0x100217e4,107)]
out=['# Vtables of the ORIGINAL vgui2.dll (Half-Life), extracted from .rdata + disassembly.',
'# Each slot: index -> address of the method -> number of 4-byte arguments (excluding `this`),',
'# taken from the `ret N` of the method (x86 thiscall: callee pops its arguments).',
'# Slot 0 of every interface is the virtual destructor (1 argument: the delete flag).',
'# "?" = could not be resolved from the linear disassembly.','']
for name,a,n in vts:
    out.append('## %s  vtable @ %08x, %d slots'%(name,a,n))
    for i,f in enumerate(vtable(a,n)):
        ab=argbytes(f)
        out.append('  %3d  %08x  args=%s'%(i,f,'?' if ab is None else ab//4))
    out.append('')
open('/home/claude/freevgui2/original/vtable_signatures.txt','w').write('\n'.join(out))
print(len(out),'lines')
