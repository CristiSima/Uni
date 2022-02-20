#!/bin/python
import sys

# print(sys.argv)

for fn in sys.argv[1:]:
	# print(fn)
	fo=open(fn,"r")
	data=fo.read()
	fo.close()

	data=data.replace("="," = ")
	data=data.replace("<"," < ")
	data=data.replace(">"," > ")
	# data=data.replace("!"," !")
	data=data.replace(";","; ")
	data=data.replace(",",", ")
	data=data.replace("if(","if (")
	data=data.replace("  "," ")
	data=data.replace("  "," ")
	data=data.replace("  "," ")
	data=data.replace("  "," ")
	data=data.replace("- >","->")
	data=data.replace("% =","%=")
	data=data.replace("+ =","+=")
	data=data.replace("= =","==")
	data=data.replace("! =","!=")
	data=data.replace("> =",">=")
	data=data.replace("< =","<=")
	data=data.replace("< <","<<")
	data=data.replace("> >",">>")
	data=data.replace(" \n","\n")
	data=data.replace(" \n","\n")
	data=data.replace(" \n","\n")
	# data=data.replace("Atentie !","Atentie!")
	data=data.replace("#include < string.h >","#include <string.h>")
	data=data.replace("#include < stdio.h >","#include <stdio.h>")
	data=data.replace("#include < stdlib.h >","#include <stdlib.h>")
	data=data.replace("#include < errno.h >","#include <errno.h>")
	data=data.replace("#include < ","#include <").replace(".h >\n",".h>\n")
	data=data.replace("#endif //","#endif  //")
	data=data.replace("}\n\twhile","}while")




	fo=open(fn,"w")
	fo.write(data)
	fo.close()
