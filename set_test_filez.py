import os

os.mkdir('templates/test_files')

data = ['a' for i in range(500)]
letter = 'a'

for i in range(500):
    for j in range(i):
        data[j] = letter
    f = open('templates/test_files/' + str(i) + '.txt', 'w')
    for j in range(10):
        f.write(' '.join(data[t] for t in range(j * 50, (j + 1) * 50)))
        f.write('\n')
    f.close()
    letter = chr((ord(letter) + (i % 26)) % ord('z'))
    if ord(letter) < 97:
        letter = 'a'

