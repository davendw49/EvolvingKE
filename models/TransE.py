import torch
import torch.autograd as autograd
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.autograd import Variable
import numpy as np
from .Model import Model
import math

class TransE(Model):
	def __init__(self, config):
		super(TransE, self).__init__(config)
		self.ent_embeddings = nn.Embedding(self.config.entTotal, self.config.hidden_size)
		self.rel_embeddings = nn.Embedding(self.config.relTotal, self.config.hidden_size)

		self.criterion = nn.MarginRankingLoss(self.config.margin, False)
		self.init_weights()
		print("**************")
		print("****TransE****")
		print("**************")
		
	def init_weights(self):
		nn.init.xavier_uniform_(self.ent_embeddings.weight.data)
		nn.init.xavier_uniform_(self.rel_embeddings.weight.data)

	def _calc(self, h, t, r):
		return torch.norm(h + r - t, self.config.p_norm, -1)
	
	def loss(self, p_score, n_score, d_influ):
		y = Variable(torch.Tensor([-1]))# .cuda()
		a = self.config.tlmbda*(d_influ - self.config.enddate).float()
		influ = torch.pow(math.exp(1),a)
		# margin不参与时间影响
		return self.criterion(influ*p_score, influ*n_score, y)
		# margin参与时间的影响
		# return self.criterion(influ*p_score, influ*n_score, influ*y)

	def forward(self):
		h = self.ent_embeddings(self.batch_h)
		t = self.ent_embeddings(self.batch_t)
		r = self.rel_embeddings(self.batch_r)
		d = self.batch_d
		
		score = self._calc(h ,t, r)
		d_influ = self.get_date_influence(d)
		p_score = self.get_positive_score(score)
		n_score = self.get_negative_score(score)

		f_score = self.loss(p_score, n_score, d_influ)
		return f_score
	def predict(self):
		h = self.ent_embeddings(self.batch_h)
		t = self.ent_embeddings(self.batch_t)
		r = self.rel_embeddings(self.batch_r)

		score = self._calc(h, t, r)
		return score.cpu().data.numpy()


