cmd = {
    "exitc":"9",
    "movrc":"6",
    "right":"33",
    "debug":"8",
    "callc":"14",
    "left":"32",
    "mova":"7",
    "call":"12",
    "cmpr":"24",
    "movr":"4",
    "movc":"4",
    "addr":"2",
    "subr":"16",
    "pshc":"19",
    "mulr":"21",
    "add":"1",
    "mov":"3",
    "ret":"10",
    "jmp":"11",
    "jmr":"13",
    "sub":"15",
    "pop":"17",
    "psh":"18",
    "mul":"20",
    "inc":"22",
    "int":"31",
    "cmp":"23",
    "jae":"25",
    "jqe":"26",
    "jke":"27",
    "jne":"28",
    "jce":"29",
    "jie":"30"
}

while 1:
    g:str = input("> ")
    if g=='exit':
        exit(0)
    try:
        fp = open(g,"r")
    except:
        print("未找到文件\nNot found file.")
        continue
    nodes = fp.readlines()
    fp.close()
    newfp = open(f"{g}.qsm","w")
    for i in range(len(nodes)):
        nodes[i] = nodes[i].split()
    print(nodes)
    for i in range(len(nodes)):
        for j in range(len(nodes[i])):
            nodes[i][j].replace("\n","")
    print(nodes)
    for i in range(len(nodes)):
        for key in cmd:
            if nodes[i][0] == key:
                nodes[i][0] = cmd[key]
    print(nodes)
    for i in range(len(nodes)):
        nodes[i] = "\n".join(nodes[i]) + "\n"
    print(nodes)
    newfp.writelines(nodes)
    newfp.close()
