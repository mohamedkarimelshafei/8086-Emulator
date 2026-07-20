ct.obj: ct.asm
 masm  /W2 ct.asm,,,,

mdahgc.obj: mdahgc.asm
 masm /W2 mdahgc.asm,,,,

subr.obj: subr.asm
 masm /W2 subr.asm,,,,

ct.exe: ct.obj mdahgc.obj subr.obj
 link /CP:1 ct.obj + mdahgc.obj + subr.obj,,,,


#Generating files with images for tests
NTSCDATA.OBJ: NTSCDATA.ASM
 MASM /W2 NTSCDATA.ASM,,,,

NTSCDATA.EXE : NTSCDATA.OBJ
 LINK NTSCDATA.OBJ,,,,

NTSCDATA.IMG : NTSCDATA.EXE
 EXE2BIN NTSCDATA.EXE NTSCDATA.IMG


C160100D.OBJ: C160100D.ASM
 MASM /W2 C160100D.ASM,,,,

C160100D.EXE : C160100D.OBJ
 LINK C160100D.OBJ,,,,

C160100D.IMG : C160100D.EXE
 EXE2BIN C160100D.EXE C160100D.IMG

