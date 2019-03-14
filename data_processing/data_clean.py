# -*- coding: utf-8 -*-
import csv
import sys

data = open("/Users/daven/DATASET/EvolvingKG/data_2013.csv", 'r', encoding='utf8')
csvfile = open("../data/data_2013.csv", "w", encoding='utf8')
dict_reader = csv.DictReader(data)

writer = csv.writer(csvfile)
# 先写入columns_name
writer.writerow(["Source","CAMEOCode","Target","Date"])

for dict_line in dict_reader:
	if str(dict_line['Source']).strip() != "" and str(dict_line['CAMEOCode']).strip() != "" and str(dict_line['Target']).strip() != "":
		#写入多行用writerows
		writer.writerow([str(dict_line['Source']).strip(),str(dict_line['CAMEOCode']).strip(),str(dict_line['Target']).strip(),str(dict_line['Date']).strip()])

data.close()
csvfile.close()