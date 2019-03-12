import pandas as pd

test2id = open("../benchmarks/IE15K-EKG/test2id.txt", "w", encoding='utf8')
valid2id = open("../benchmarks/IE15K-EKG/valid2id.txt", "w", encoding='utf8')

def planA():
	f = open("../benchmarks/IE15K-EKG/origin2all.txt", "r", encoding='utf8')
	f_train = open("../benchmarks/IE15K-EKG/train2id.txt", "w", encoding='utf8')
	testandvalid = []
	for line in f:
		line = line[:len(line)]
		c = line.split(' ')
		c[3] = c[3][:len(c[3])-1]
		if int(c[3])<281:
			print(line, end='', file=f_train)
		else:
			testandvalid.append(c)
	print(testandvalid)
	df = pd.DataFrame(testandvalid, columns=['Source', 'Target', 'CAMEOcode', 'Date'])
	data = df.sample(frac=0.5)
	data2 = df.sample(frac=0.5)
	# data = data.sort_values(by="Date",ascending=True)
	for i in range(0, len(data)):
		print(str(data.iloc[i]['Source'].strip()),str(data.iloc[i]['Target'].strip()),str(data.iloc[i]['CAMEOcode'].strip()),str(data.iloc[i]['Date'].strip()), file=test2id)
	for i in range(0, len(data2)):
		print(str(data2.iloc[i]['Source'].strip()),str(data2.iloc[i]['Target'].strip()),str(data2.iloc[i]['CAMEOcode'].strip()),str(data2.iloc[i]['Date'].strip()), file=valid2id)

def planB():
	f = open("../benchmarks/IE15K-EKG/origin2all.txt", "r", encoding='utf8')
	f_train = open("../benchmarks/IE15K-EKG/train2id.txt", "w", encoding='utf8')
	testandvalid = []
	for line in f:
		line = line[:len(line)]
		c = line.split(' ')
		c[3] = c[3][:len(c[3])-1]
		if int(c[3])<281:
			print(line, end='', file=f_train)
		else:
			testandvalid.append(c)

	pos = 0
	for item in testandvalid:
		if pos%2 == 0:
			print(item[0], item[1], item[2], item[3], file=test2id)
		else:
			print(item[0], item[1], item[2], item[3], file=valid2id)
		pos+=1
planB()
