#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt



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
	x: float
	d: float
	lz: np.array(float) = []
	ls: np.array(float) = []

	def __init__(self, x: float, d: float, dmax: float, s: float):
		self.x = x
		self.d = d
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

	def adjust_d(self, dmax: float, s: float, lmin: float) -> bool:
		self.d, error = Interval.adjust_d(self, dmax, s, lmin)
		self.find_lz(dmax)
		self.find_ls(dmax, s)
		return error

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
		c2_d: float
	):
		self.dmax_init: float = dmax
		self.lmin: int = lmin
		self.m: float = (c1_x + c2_x) / 2
		self.s: float = abs(c1_x - c2_x) / 2 # TODO between x or x + d/2 ?

		self.c1_init: Couple = Couple(c1_x, c1_d, self.dmax_init, self.s)
		self.c2_init: Couple = Couple(c2_x, c2_d, self.dmax_init, self.s)

		self.dmax_solved: float = self.find_dmax(self.c1_init, self.c2_init, self.dmax_init)

		self.c1_solved: Couple = Couple(c1_x, c1_d, self.dmax_solved, self.s)
		self.c2_solved: Couple = Couple(c2_x, c2_d, self.dmax_solved, self.s)

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
		c1_dmax = Couple.find_dmax(c1, dmax)
		c2_dmax = Couple.find_dmax(c2, dmax)
		if c1_dmax > c2_dmax:
			return c1_dmax
		else:
			return c2_dmax

	@staticmethod
	def adjust_d(c: Couple, dmax: float, s: float, lmin: float, iter_limit = np.inf) -> [float, bool]:
		step = 1000
		d = c.d

		current_lz = c.lz
		current_ls = c.ls

		counter = 0
		error = False
		while(np.size(current_lz) > np.size(current_ls) or np.size(current_ls[:-1]) < lmin):
			d -= d / step
			current_lz = Interval.find_lz(d, c.lmbda, dmax)
			current_ls = Interval.find_ls(d, c.lmbda, dmax, s)

			counter += 1
			if counter >= iter_limit:
				print("adjust_d() | WARNING : iteration limit reached")
				error = True
				break

		print()
		print("adjust_d() | iterations : " + str(counter))
		print("adjust_d() | d : ", d)
		print("adjust_d() | ls : " + str(np.size(current_ls)) + " lines")
		return d, error

	def solve(self):
		# TODO
		# adjust d↓ to satisfy z <= s & ls >= lmin
		# TODO.2 ^^^ why this condition??
		# TODO.2 adjust d↓ to satisfy also ls[-1] = m || R.ls[-1] - L.ls[-1] = dmax_solved ???
		# TODO.2 or between dmax and dmax_solved?
		# adjust lambda↓ to satisfy ls[-1] = m || R.ls[-1] L.ls[-1] = dmax_solved
		self.c1_solved.adjust_d(self.dmax_solved, self.s, self.lmin)
		self.c2_solved.adjust_d(self.dmax_solved, self.s, self.lmin)



class Scene:
	"""
	- positions (edges + middle)
	- rule lines
	- initial fill lines
	- initial stop lines
	- solved fill lines
	- solved stop lines
	"""

	def __init__(self):
		self.position_lines: np.array = []
		self.initial_rule_lines: np.array = []
		self.initial_fill_lines: np.array = []
		self.initial_stop_lines: np.array = []
		self.solved_rule_lines: np.array = []
		self.solved_fill_lines: np.array = []
		self.solved_stop_lines: np.array = []

	def __iadd__(self, other):
		self.position_lines = np.append(self.position_lines, other.position_lines)
		self.initial_rule_lines = np.append(self.initial_rule_lines, other.initial_rule_lines)
		self.initial_fill_lines = np.append(self.initial_fill_lines, other.initial_fill_lines)
		self.initial_stop_lines = np.append(self.initial_stop_lines, other.initial_stop_lines)
		self.solved_rule_lines = np.append(self.solved_rule_lines, other.solved_rule_lines)
		self.solved_fill_lines = np.append(self.solved_fill_lines, other.solved_fill_lines)
		self.solved_stop_lines = np.append(self.solved_stop_lines, other.solved_stop_lines)
		return self

	def append_position_lines(self, position_lines: np.array):
		self.position_lines = np.append(self.position_lines, position_lines)

	def append_initial_rule_lines(self, initial_rule_lines: np.array):
		self.initial_rule_lines = np.append(self.initial_rule_lines, initial_rule_lines)

	def append_initial_fill_lines(self, initial_fill_lines: np.array):
		self.initial_fill_lines = np.append(self.initial_fill_lines, initial_fill_lines)

	def append_initial_stop_lines(self, initial_stop_lines: np.array):
		self.initial_stop_lines = np.append(self.initial_stop_lines, initial_stop_lines)

	def append_solved_rule_lines(self, solved_rule_lines: np.array):
		self.solved_rule_lines = np.append(self.solved_rule_lines, solved_rule_lines)

	def append_solved_fill_lines(self, solved_fill_lines: np.array):
		self.solved_fill_lines = np.append(self.solved_fill_lines, solved_fill_lines)

	def append_solved_stop_lines(self, solved_stop_lines: np.array):
		self.solved_stop_lines = np.append(self.solved_stop_lines, solved_stop_lines)

def to_scene(i: Interval) -> Scene:
	s = Scene()

	c1_x = i.c1_init.x
	c2_x = i.c2_init.x
	c1_init_pos = c1_x + i.c1_init.d/2
	c2_init_pos = c2_x - i.c2_init.d/2

	s.append_position_lines([c1_x, i.m, c2_x])
	s.append_initial_rule_lines([c1_x - i.c1_init.d/2, c1_x + i.c1_init.d/2])
	s.append_initial_rule_lines([c2_x - i.c2_init.d/2, c2_x + i.c2_init.d/2])
#	s.append_initial_stop_lines([i.c1_init.ls[-1]] if np.size(i.c1_init.ls) >= 1 else [])
	if np.size(i.c1_init.ls) >= 1:
		s.append_initial_stop_lines([c1_init_pos + i.c1_init.ls[-1]])
	if np.size(i.c2_init.ls) >= 1:
		s.append_initial_stop_lines([c2_init_pos - i.c2_init.ls[-1]])
	if np.size(i.c1_init.ls) >= 2:
		s.append_initial_fill_lines(c1_init_pos + i.c1_init.ls[:-1])
	if np.size(i.c2_init.ls) >= 2:
		s.append_initial_fill_lines(c2_init_pos - i.c2_init.ls[:-1])

	c1_solved_pos = c1_x + i.c1_solved.d/2
	c2_solved_pos = c2_x - i.c2_solved.d/2
	s.append_solved_rule_lines([c1_x - i.c1_solved.d/2, c1_x + i.c1_solved.d/2])
	s.append_solved_rule_lines([c2_x - i.c2_solved.d/2, c2_x + i.c2_solved.d/2])
	if np.size(i.c1_solved.ls) >= 1:
		s.append_solved_stop_lines([c1_solved_pos + i.c1_solved.ls[-1]])
	if np.size(i.c2_solved.ls) >= 1:
		s.append_solved_stop_lines([c2_solved_pos - i.c2_solved.ls[-1]])
	if np.size(i.c1_solved.ls) >= 2:
		s.append_solved_fill_lines(c1_solved_pos + i.c1_solved.ls[:-1])
	if np.size(i.c2_solved.ls) >= 2:
		s.append_solved_fill_lines(c2_solved_pos - i.c2_solved.ls[:-1])

	return s

def plot_matplotlib(
	axs,
	scene: Scene,
	show_initial_lines: bool = True,
	show_solved_lines: bool = True,
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

	if show_initial_lines:
		for line in scene.initial_rule_lines:
			axs[plot_matplotlib.plot_counter].axvline(
				x=line,
				color="darkorange",
				linestyle="dotted",
				linewidth=2)
		for line in scene.initial_fill_lines:
			axs[plot_matplotlib.plot_counter].axvline(
				x=line,
				color="darkviolet",
				linestyle="dotted",
				linewidth=1)
		if show_stop_lines:
			for line in scene.initial_stop_lines:
				axs[plot_matplotlib.plot_counter].axvline(
					x=line,
					color="lime",
					linestyle="dotted",
					linewidth=1)

	if show_solved_lines:
		for line in scene.solved_rule_lines:
			axs[plot_matplotlib.plot_counter].axvline(
				x=line,
				color="darkorange",
				linestyle="solid",
				linewidth=2)
		for line in scene.solved_fill_lines:
			axs[plot_matplotlib.plot_counter].axvline(
				x=line,
				color="darkviolet",
				linestyle="solid",
				linewidth=1)
		if show_stop_lines:
			for line in scene.solved_stop_lines:
				axs[plot_matplotlib.plot_counter].axvline(
					x=line,
					color="lime",
					linestyle="solid",
					linewidth=1)
	plot_matplotlib.plot_counter += 1
plot_matplotlib.plot_counter = 0

def draw_scene(
	s: Scene,
	title: str = ""
):
	fig = plt.figure(draw_scene.fig_counter, figsize=(25, 8))
	axs = fig.subplots(4, 1, sharex=True)
	fig.suptitle(title, fontsize=16)

	plot_matplotlib(axs, s, show_solved_lines=False)
	plot_matplotlib(axs, s)
	plot_matplotlib(axs, s, show_initial_lines=False)
	plot_matplotlib(axs, s, show_initial_lines=False, show_stop_lines=False)

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
	c2_x: float = 0,
	c2_d_div: float = 0
):
	i = Interval(
		dmax,
		lmin,
		c1_x, dmax / c1_d_div,
		c2_x, dmax / c2_d_div)

	i.solve()
	draw_scene(to_scene(i), title)



if __name__ == "__main__":
	try_scene(
		dmax=0.3,
		lmin=7,
		c1_x=1.3, c1_d_div=100,
		c2_x=6.0, c2_d_div=1.2)

	try_scene(
		dmax=2.5,
		lmin=2,
		c1_x=1.3, c1_d_div=100,
		c2_x=6.0, c2_d_div=3.3)

	try_scene(
		dmax=3.5,
		lmin=2,
		c1_x=1.3, c1_d_div=2,
		c2_x=6.0, c2_d_div=2)
