
f_e = open("../benchmarks/QG18K-EKG/entity2id.txt", "r", encoding='utf8')
f_r = open("../benchmarks/QG18K-EKG/relation2id.txt", "r", encoding='utf8')
f_d = open("../benchmarks/QG18K-EKG/date2id.txt", "r", encoding='utf8')

f = open("../benchmarks/QG18K-EKG/origin_tiny.txt", "r", encoding='utf8')
f_out = open("../benchmarks/QG18K-EKG/origin2all.txt", "w", encoding='utf8')

entity_map = {}
relation_map = {}
date_map = {}
for line in f_e:
	line = line[:len(line)-1]
	c = line.split(' ')
	entity_map[c[0]] = c[1]

for line in f_r:
	line = line[:len(line)-1]
	c = line.split(' ')
	relation_map[c[0]] = c[1]

for line in f_d:
	line = line[:len(line)-1]
	c = line.split(' ')
	date_map[c[0]] = c[1]


f_e.close()
f_r.close()
f_d.close()

for line in f:
	line = line[:len(line)-1]
	c = line.split(' ')
	print(entity_map[c[0]], entity_map[c[1]], relation_map[c[2]], date_map[c[3]], file=f_out)
f.close()
f_out.close()