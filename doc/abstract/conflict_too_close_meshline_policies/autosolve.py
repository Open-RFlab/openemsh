#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from copy import copy



class Couple:
	"""
	Couple of lines, describe 3rds rule or 2nds rule lines around an edge.
	- x: Position of the couple (of the edge).
	- d: Distance between 2 rule lines.
	- lmbda: Factor between 2 adjacent d.
	- lz: List of lines until dmax. Z is distance between rule line and last
	  line before d = dmax. Last z line is after dmax.
	- ls: List of lines until m. S is distance between rule line and m. Last
	  s line is after m.
	"""
	lmbda: float = 2
	lmin: int 
	x: float
	d: float
	lz: np.array(float) = []
	ls: np.array(float) = []

	def __init__(self, x: float, d: float, dmax: float, s: float, lmin: int):
		self.x = x
		self.d = d
		self.lmin = lmin
		self.find_lz(dmax)
		self.find_ls(dmax, s)

	def find_dmax(self, dmax: float) -> float:
		if np.size(self.lz) <= np.size(self.ls): 
			return dmax
		else:
			return self.lz[np.size(self.ls)-1] - self.lz[np.size(self.ls)-2]

	def find_lz(self, dmax: float):
		self.lz = Interval.find_lz(self.d, self.lmbda, dmax)

	def find_ls(self, dmax: float, s: float):
		self.ls = Interval.find_ls(self.d, self.lmbda, dmax, s)

	def adjust_d_for_dmax_lmin(self, dmax: float, s: float):
		print()
		self.d, error = Interval.adjust_d_for_dmax_lmin(self, dmax, s, self.lmin)
		self.find_lz(dmax)
		self.find_ls(dmax, s)

	def adjust_d_for_s(self, dmax: float, s: float):
		print()
		self.d, error = Interval.adjust_d_for_s(self, dmax, s, self.lmin)
		self.find_lz(dmax)
		self.find_ls(dmax, s)

	def adjust_lmbda_for_s(self, dmax: float, s: float):
		print()
		self.lmbda, error = Interval.adjust_lmbda_for_s(self, dmax, s)
		self.find_lz(dmax)
		self.find_ls(dmax, s)

class Interval:
	"""
	- dmax: Local maximal distance between 2 adjacent lines.
	- lmin: Local minimum of lines.
	- m: Middle between c1.x and c2.x.
	- s: Distance between c1.x and c2.x (or c1 and c2?). Must consider d during
	  calcul about s.
	"""

	def __init__(self,
		dmax: float,
		lmin: int,
		c1_x: float,
		c1_d: float,
		c2_x: float,
		c2_d: float,
		c1_lmin: int = 0, # Defaults to lmin
		c2_lmin: int = 0  # Defaults to lmin
	):
		self.dmax_init: float = dmax
		self.lmin: int = lmin
		self.m: float = (c1_x + c2_x) / 2
		self.s: float = abs(c1_x - c2_x) / 2 # TODO between x or x + d/2 ?
		self.c1: list(Couple) = []
		self.c2: list(Couple) = []

		c1 = Couple(
			c1_x,
			c1_d,
			self.dmax_init,
			self.s,
			c1_lmin if c1_lmin else self.lmin)
		c2 = Couple(
			c2_x,
			c2_d,
			self.dmax_init,
			self.s,
			c2_lmin if c2_lmin else self.lmin)

		self.dmax_solved: float = self.find_dmax(c1, c2, self.dmax_init)
		print()
		print("Interval | dmax : " + str(dmax) + " -> " + str(self.dmax_solved))

		self.c1.append(Couple(
			c1_x,
			c1_d,
			self.dmax_solved,
			self.s,
			c1_lmin if c1_lmin else self.lmin))
		self.c2.append(Couple(
			c2_x,
			c2_d,
			self.dmax_solved,
			self.s,
			c2_lmin if c2_lmin else self.lmin))

	@staticmethod
	def find_lz(d: float, lmbda: float, dmax: float) -> np.array(float):
		if lmbda == 1:
			return [float('inf')]

		z = 0
		lz: np.array = []
		current_d = d
		while current_d < dmax:
			current_d *= lmbda
			if current_d < dmax:
				z += current_d
				lz = np.append(lz, z)

		return lz

	@staticmethod
	def find_ls(d: float, lmbda: float, dmax: float, s: float) -> np.array(float):
		current_s = 0
		ls: np.array = []
		current_d = d
		while current_s < s - d / 2:
			if current_d < dmax:
				current_d *= lmbda
				if current_d > dmax:
					current_d = dmax
			current_s += current_d
			ls = np.append(ls, current_s)

		return ls

	@staticmethod
	def find_dmax(c1: Couple, c2: Couple, dmax: float) -> float:
		c1_dmax = c1.find_dmax(dmax)
		c2_dmax = c2.find_dmax(dmax)
		if c1_dmax > c2_dmax:
			return c1_dmax
		else:
			return c2_dmax

	@staticmethod
	def adjust_d_for_dmax_lmin(c: Couple, dmax: float, s: float, lmin: int, iter_limit = np.inf) -> [float, bool]:
		step = 1000
		d = c.d

		current_lz = c.lz
		current_ls = c.ls

		counter = 0
		error = False
		while np.size(current_lz) > np.size(current_ls) or np.size(current_ls[:-1]) < lmin:
			d -= d / step
			current_lz = Interval.find_lz(d, c.lmbda, dmax)
			current_ls = Interval.find_ls(d, c.lmbda, dmax, s)

			counter += 1
			if counter >= iter_limit:
				print("adjust_d() | WARNING : iteration limit reached")
				error = True
				break

		print("adjust_d_for_dmax_lmin() | iterations : " + str(counter))
		print("adjust_d_for_dmax_lmin() | d : ", d)
		print("adjust_d_for_dmax_lmin() | ls : " + str(np.size(current_ls)) + " lines")
		return d, error

	@staticmethod
	def adjust_d_for_s(c: Couple, dmax: float, s: float, lmin: int, iter_limit = np.inf) -> [float, bool]:
		step = 1000
		d = c.d

		nlines = np.size(c.ls)

		counter = 0
		error = False
		while True:
			current_d = d - d / step
			current_ls = Interval.find_ls(d, c.lmbda, dmax, s)
			if np.size(current_ls) > nlines:
				break

			d = current_d

			counter += 1
			if counter >= iter_limit:
				print("adjust_d_for_s() | WARNING : iteration limit reached")
				error = True
				break

		print("adjust_d_for_s() | iterations : " + str(counter))
		print("adjust_d_for_s() | d : ", d)
		print("adjust_d_for_s() | spaces around middle line : " + str(s - current_ls[-2]) + " <- s -> " + str(current_ls[-1] - s))
		return d, error

	@staticmethod
	def adjust_lmbda_for_s(c: Couple, dmax: float, s: float, iter_limit = np.inf) -> [float, bool]:
		step = 10000
		lmbda = c.lmbda

		nlines = np.size(c.ls)
		current_ls = c.ls

		# TODO lmbda limits are based on lz
		counter = 0
		error = False
		while lmbda > 1 and np.size(current_ls) <= nlines:
			lmbda -= lmbda / step
			if lmbda < 1:
				lmbda = 1
				print("adjust_lmbda_for_s() | WARNING : lmbda=1 limit reached")

			current_ls = Interval.find_ls(c.d, lmbda, dmax, s)

			counter += 1
			if counter >= iter_limit:
				print("adjust_lmbda_for_s() | WARNING : iteration limit reached")
				error = True
				break

		print("adjust_lmbda_for_s() | iterations : " + str(counter))
		print("adjust_lmbda_for_s() | lmbda : ", lmbda)
		print("adjust_lmbda_for_s() | spaces around middle line : " + str(s - current_ls[-2]) + " <- s -> " + str(current_ls[-1] - s))
		return lmbda, error

	def new_step(self):
		self.c1.append(copy(self.c1[-1]))
		self.c2.append(copy(self.c2[-1]))

	def solve(self,
		with_c1: bool = True,
		with_c2: bool = True,
		with_adjust_d_for_dmax_lmin: bool = True,
		with_adjust_d_for_s: bool = True,
		with_adjust_lmbda_for_s: bool = True
	):
		# TODO
		# adjust d↓ to satisfy z <= s & ls >= lmin
		# TODO.2 ^^^ why this condition??
		# TODO.2 adjust d↓ to satisfy also ls[-1] = m || R.ls[-1] - L.ls[-1] = dmax_solved ???
		# TODO.2 or between dmax and dmax_solved?
		# adjust lambda↓ to satisfy ls[-1] = m || R.ls[-1] L.ls[-1] = dmax_solved

		if not with_c1 and not with_c2:
			return

		if with_adjust_d_for_dmax_lmin:
			self.new_step()
			if with_c1:
				self.c1[-1].adjust_d_for_dmax_lmin(self.dmax_solved, self.s)
			if with_c2:
				self.c2[-1].adjust_d_for_dmax_lmin(self.dmax_solved, self.s)

		if with_adjust_d_for_s:
			self.new_step()
			if with_c1:
				self.c1[-1].adjust_d_for_s(self.dmax_solved, self.s)
			if with_c2:
				self.c2[-1].adjust_d_for_s(self.dmax_solved, self.s)

		if with_adjust_lmbda_for_s:
			self.new_step()
			if with_c1:
				self.c1[-1].adjust_lmbda_for_s(self.dmax_solved, self.s)
			if with_c2:
				self.c2[-1].adjust_lmbda_for_s(self.dmax_solved, self.s)



class Scene:
	"""
	- positions (edges + middle)
	- rule lines
	- fill lines
	- stop lines
	"""

	def __init__(self):
		self.position_lines: np.array = []
		self.rule_lines: list(np.array) = []
		self.fill_lines: list(np.array) = []
		self.stop_lines: list(np.array) = []

	def new_step(self):
		self.rule_lines.append([])
		self.fill_lines.append([])
		self.stop_lines.append([])

	def append_position_lines(self, position_lines: np.array):
		self.position_lines = np.append(self.position_lines, position_lines)

	def append_rule_lines(self, rule_lines: np.array):
		self.rule_lines[-1] = np.append(self.rule_lines[-1], rule_lines)

	def append_fill_lines(self, fill_lines: np.array):
		self.fill_lines[-1] = np.append(self.fill_lines[-1], fill_lines)

	def append_stop_lines(self, stop_lines: np.array):
		self.stop_lines[-1] = np.append(self.stop_lines[-1], stop_lines)

def to_scene(i: Interval, with_c1: bool = True, with_c2: bool = True) -> Scene:
	s = Scene()

	c1_x = i.c1[0].x
	c2_x = i.c2[0].x
	s.append_position_lines([c1_x, i.m, c2_x])

	size = len(i.c1) if len(i.c1) == len(i.c2) else 0
	for step in range(size):
		s.new_step()

		c1_pos = c1_x + i.c1[step].d/2
		c2_pos = c2_x - i.c2[step].d/2

		if with_c1:
			s.append_rule_lines([c1_x - i.c1[step].d/2, c1_x + i.c1[step].d/2])
			if np.size(i.c1[step].ls) >= 1:
				s.append_stop_lines([c1_pos + i.c1[step].ls[-1]])
			if np.size(i.c1[step].ls) >= 2:
				s.append_fill_lines(c1_pos + i.c1[step].ls[:-1])

		if with_c2:
			s.append_rule_lines([c2_x - i.c2[step].d/2, c2_x + i.c2[step].d/2])
			if np.size(i.c2[step].ls) >= 1:
				s.append_stop_lines([c2_pos - i.c2[step].ls[-1]])
			if np.size(i.c2[step].ls) >= 2:
				s.append_fill_lines(c2_pos - i.c2[step].ls[:-1])

	return s

def plot_matplotlib(
	axs,
	scene: Scene,
	step: int,
	show_stop_lines: bool = True
):
	axs[plot_matplotlib.plot_counter].set_xlim(1, 7)
	axs[plot_matplotlib.plot_counter].tick_params(
		axis='y',
		which='both',
		right=False,
		left=False,
		labelleft=False)

	for line in scene.position_lines:
		axs[plot_matplotlib.plot_counter].axvline(
			x=line,
			color="grey",
			linestyle="dotted",
			linewidth=2)

	for line in scene.rule_lines[step]:
		axs[plot_matplotlib.plot_counter].axvline(
			x=line,
			color="darkorange",
			linestyle="solid",
			linewidth=2)
	for line in scene.fill_lines[step]:
		axs[plot_matplotlib.plot_counter].axvline(
			x=line,
			color="darkviolet",
			linestyle="solid",
			linewidth=1)
	if show_stop_lines:
		for line in scene.stop_lines[step]:
			axs[plot_matplotlib.plot_counter].axvline(
				x=line,
				color="lime",
				linestyle="dotted",
				linewidth=1)
	plot_matplotlib.plot_counter += 1
plot_matplotlib.plot_counter = 0

def draw_scene(
	s: Scene,
	title: str = ""
):
	size = len(s.rule_lines) if len(s.rule_lines) == len(s.fill_lines) == len(s.stop_lines) else 0

	fig = plt.figure(draw_scene.fig_counter, figsize=(25, 8))
	axs = fig.subplots(size, 1, sharex=True)
	fig.suptitle(title, fontsize=16)

	for i in range(size):
		plot_matplotlib(axs, s, i)

	plt.show()

	plot_matplotlib.plot_counter = 0
	draw_scene.fig_counter += 1
draw_scene.fig_counter = 1

def try_scene(
	title: str = "",
	dmax: float = 0,
	lmin: int = 0,
	c1_x: float = 0,
	c1_d_div: float = 0,
	c1_lmin: int = 0,
	c2_x: float = 0,
	c2_d_div: float = 0,
	c2_lmin: int = 0,
	with_c1: bool = True,
	with_c2: bool = True,
	with_adjust_d_for_dmax_lmin: bool = True,
	with_adjust_d_for_s: bool = True,
	with_adjust_lmbda_for_s: bool = True
):
	i = Interval(
		dmax,
		lmin,
		c1_x, dmax / c1_d_div,
		c2_x, dmax / c2_d_div,
		c1_lmin,
		c2_lmin)

	i.solve(
		with_c1,
		with_c2,
		with_adjust_d_for_dmax_lmin,
		with_adjust_d_for_s,
		with_adjust_lmbda_for_s)

	draw_scene(to_scene(i, with_c1, with_c2), title)



if __name__ == "__main__":

	with_c1 = True
	with_c2 = True
	with_adjust_d_for_dmax_lmin = True
	with_adjust_d_for_s = True
	with_adjust_lmbda_for_s = True

	try_scene(
		dmax=0.3,
		lmin=37,
		c1_x=1.3, c1_d_div=100, c1_lmin=10,
		c2_x=6.0, c2_d_div=1.2,
		with_c1=with_c1,
		with_c2=with_c2,
		with_adjust_d_for_dmax_lmin=with_adjust_d_for_dmax_lmin,
		with_adjust_d_for_s=with_adjust_d_for_s,
		with_adjust_lmbda_for_s=with_adjust_lmbda_for_s)

	try_scene(
		dmax=2.5,
		lmin=10,
		c1_x=1.3, c1_d_div=100,
		c2_x=6.0, c2_d_div=3.3, c2_lmin=3,
		with_c1=with_c1,
		with_c2=with_c2,
		with_adjust_d_for_dmax_lmin=with_adjust_d_for_dmax_lmin,
		with_adjust_d_for_s=with_adjust_d_for_s,
		with_adjust_lmbda_for_s=with_adjust_lmbda_for_s)

	try_scene(
		dmax=3.5,
		lmin=2,
		c1_x=1.3, c1_d_div=2,
		c2_x=6.0, c2_d_div=2, c2_lmin=20,
		with_c1=with_c1,
		with_c2=with_c2,
		with_adjust_d_for_dmax_lmin=with_adjust_d_for_dmax_lmin,
		with_adjust_d_for_s=with_adjust_d_for_s,
		with_adjust_lmbda_for_s=with_adjust_lmbda_for_s)
