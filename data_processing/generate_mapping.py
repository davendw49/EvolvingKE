f = open("../benchmarks/IE15K-EKG/origin_tiny.txt", "r", encoding='utf8')


entity_map = {}
relation_map = {}
date_map = []
for line in f:
	line = line[:len(line)-1]
	c = line.split(' ')
	# c[0] head c[2] relation c[1] tail c[3] date
	entity_map[c[0]] = 1
	entity_map[c[1]] = 1
	relation_map[c[2]] = 1
	if c[3] not in date_map:
		date_map.append(c[3])

f_e = open("../benchmarks/IE15K-EKG/entity2id.txt", "w", encoding='utf8')
f_r = open("../benchmarks/IE15K-EKG/relation2id.txt", "w", encoding='utf8')
f_d = open("../benchmarks/IE15K-EKG/date2id.txt", "w", encoding='utf8')

pos = 0
for i in entity_map.keys():
	print(i, pos, file=f_e)
	pos+=1

pos = 0
for i in relation_map.keys():
	print(i, pos, file=f_r)
	pos+=1

date_map.sort()


for i in range(0, len(date_map)):
	print(date_map[i], i, file=f_d)