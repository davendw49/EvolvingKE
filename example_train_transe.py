import config
from  models import *
import json
import os 
os.environ['CUDA_VISIBLE_DEVICES']=''
con = config.Config()
con.set_in_path("./benchmarks/IE15K-EKG/")
con.set_work_threads(8)
con.set_train_times(500)
con.set_nbatches(10)
con.set_alpha(0.001)
con.set_bern(0)
con.set_dimension(100)
con.set_margin(1.0)
con.set_ent_neg_rate(1)
con.set_rel_neg_rate(0)
con.set_opt_method("SGD")
con.set_save_steps(10)
con.set_valid_steps(10)
con.set_early_stopping_patience(10)
con.set_checkpoint_dir("./checkpoint")
con.set_result_dir("./result")
con.set_test_link(True)
con.set_test_quadruple(True)

########################
###Evloving Parameter###
########################
# 时间指数变量值
con.set_tlmbda(0.001)
# 时间终止影响点
con.set_enddate(280)

con.init()
con.set_train_model(TransE)
con.train()