import time
total = 0
erro = 0


dicti = {}

arq1 = open('ascii_noaccent_noduplicates_FIXED_v2.txt','r')
texto = arq1.readline()

while texto:
    text = texto.rstrip()

    
    if text not in dicti.keys():
        dicti[text] = text
        
    texto = arq1.readline()

arq1.close()

arq2 = open('constituicaofederal.txt','r')
arq3 = open('result.txt','a')

texto2 = arq2.readline()
initTime = time.time()*1000
while texto2:
    text = texto2.split()
    for string in text:
        total += 1
        if string not in dicti:
            arq3.write(string + "\n")
            erro += 1

    texto2 = arq2.readline()
endTime = time.time()*1000
print(endTime - initTime)
arq2.close()
arq3.close()