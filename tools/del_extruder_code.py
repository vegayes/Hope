


f= open('Li','r')
f_re = open('remaker.txt','w')

while True:
    line = f.readline()
    if not line:
        break
    n = line[:-1].split(' ')

    new_line = []
    x = ''

    for i in n:
        if i[0] == 'E' and i[i].isdigit():
            pass
        else:
            new_line.append(i)
    for j in new_line:
        x += j + ' '
    print(x)
    f_re.write(x+'\n')

f.close()
f_re.close()












