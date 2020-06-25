from only_my_railgun import play_music

# 曲を記録
freq = []
length = []
cut = []

def record(f, l, c):
    global freq,length,cut
    freq.append(f)
    length.append(l)
    cut.append(c)

# 曲のデータを読み込む
play_music(record)
n = len(freq)
int_type = "uint8_t"
if n > 255:
    int_type = "uint16_t"
if n > 65535:
    int_type = "uint32_t"
print("int_type:",int_type)

def conv_freq(f):
    if f == 0:
        return 0
    pulce = (20e6 / f)
    return int(pulce)

def decl_type(i, name):
    return "const uint"+str(i)+"_t " + name + "[] PROGMEM"

def joiner(str_list):
    ans = ""
    N = len(str_list)
    n = 10
    for i in range(N//n):
        ans += "    " + ", ".join(str_list[n*i:n*i+n]) + ",\n"
    last = str_list[n*i+n:]
    if(len(last)) > 0:
        ans += "    " + ", ".join(str_list[n*i+n:]) + ",\n"
    return ans

flags = [int(x) for x in cut]

for i in range(len(freq)):
    freq[i] = conv_freq(freq[i])
    if freq[i] >= (2**16):
        freq[i] = (freq[i]//8)
        flags[i] += 2

# ヘッダファイルを作成
directory = "../music"
hpp = open(directory + "/score.h", "w")
hpp.write("#pragma once\n")
hpp.write("#include <avr/pgmspace.h>\n")
hpp.write("#define SCORE_LENGTH "+str(n)+"\n")
hpp.write(decl_type(16, "score_freq")+" = {\n")
hpp.write(joiner([str(int(x)) for x in freq]) + "};\n")
hpp.write(decl_type(8, "score_len")+" = {\n")
hpp.write(joiner([str(x) for x in length]) + "};\n")
hpp.write(decl_type(8, "score_cut") + " = {\n")
hpp.write(joiner([str(x) for x in flags]) + "};\n")

hpp.close()
# ソースファイルを作成
exit()

