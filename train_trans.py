import config
from  models import *
import json
import os
import sys
import cProfile

class Logger(object):
	"""docstring for Logger"""
	def __init__(self, filename="Default.log"):
		self.terminal = sys.stdout
		self.log = open(filename, "a", encoding='utf8')

	def write(self, message):
		self.terminal.write(message)
		self.log.write(message)

	def flush(self):
		pass

def main():
	con = config.Config()
	os.environ['CUDA_VISIBLE_DEVICES']=''
	
	###########################
	##在这里设置要存的文件名和名字##
	###########################
	sys.stdout = Logger("TransE-UTR-Margin01-5000-final.txt")
	trainname = "UTF"
	con.set_in_path("./benchmarks/IE15K-UTR/")
	print("dataset name: ", trainname)

	con.set_train_times(3000)
	con.set_save_steps(100)
	con.set_valid_steps(100)
	con.set_early_stopping_patience(5)

	con.set_margin(0.5)

	# 时间指数变量值
	con.set_tlmbda(0.00)


	con.set_work_threads(8)
	con.set_nbatches(10)
	con.set_alpha(0.001)
	con.set_bern(0)
	con.set_dimension(50)
	con.set_ent_neg_rate(1)
	con.set_rel_neg_rate(0)
	con.set_opt_method("SGD")
	con.set_checkpoint_dir("./checkpoint")
	con.set_result_dir("./result")
	con.set_test_link(True)
	con.set_test_quadruple(True)
	########################
	###Evloving Parameter###
	########################
	# 时间终止影响点
	con.set_enddate(280)
	con.init()
	con.set_train_model(TransE)
	con.train()

cProfile.run('main()', filename='profile')