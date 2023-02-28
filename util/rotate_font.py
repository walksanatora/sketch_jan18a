import re, pprint


class symbol:
    pixels: list[list[bool]]
    char: int
    real: str
    index: int

    def __init__(self,p,c,r,i):
        self.pixels = p
        self.char = c
        self.real = r
        self.index = i
    def __repr__ (self): return self.__str__()
    def __str__(self): return f"// {self.real:0>4x} {self.char} {self.index}"
    
    def to_print(self):
        out = "\t"
        a = []
        for row in self.pixels:
            out = f'{out}0x{int("".join(map(str,map(int,row))),2):0>2x}, '
        out = f'{out}  {self.__str__()}'
        return out

    def rotate_90_ccw(self):
        npx = []
        for i in reversed(range(8)):
            new = []
            for b in range(8):
                new.append(self.pixels[b][i])
            npx.append(new)
        self.pixels = npx
        pass

syms: dict[int,symbol] = {}

regex = re.compile("\t0x..,.*")

def strToBin(s):
    return s == "1"

with open("font.h",'r') as f:
    contents = f.readlines()
    for line in contents:
        if regex.match(line):
            sline = line.strip()
            parts = sline.split(" ")
            
            px = []

            for i in range(8):
                bits = []
                bits[:] = f'{int(parts[i][2:-1],16):0>8b}'
                bits = list(map(strToBin,bits))
                px.append(bits)
            print(line,parts)
            sym = symbol(px,parts[12],int(parts[11],16),int(parts[13]))
            sym.rotate_90_ccw()
            syms[int(parts[11],16)] = sym

print(f"const static byte Alphabet[{len(syms)*8}] = {'{'}")
for line in syms.values():
    print(line.to_print())
print("};")
list(map(symbol.to_print,syms.values()))
