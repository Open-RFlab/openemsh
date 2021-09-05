#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

#def dump(obj):
#	for attr in dir(obj):
#		if hasattr(obj, attr):
#			print("obj.%s = %s" % (attr, getattr(obj, attr)))



class Couple:
	"""
	Couple of lines, describe 3rds rule or 2nds rule lines around an edge.
	- x: Position of the couple (of the edge).
	- d: Distance between 2 rule lines.
	- lmbda: Smoothness factor [0.5, 2].
	"""

	def __init__(self, x: float, d: float, lmbda: float):
		self.x = x
		self.d = d
		self.lmbda = lmbda

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
		self.rule_lines: np.array = []
		self.initial_fill_lines: np.array = []
		self.initial_stop_lines: np.array = []
		self.solved_fill_lines: np.array = []
		self.solved_stop_lines: np.array = []

	def __iadd__(self, other):
		self.position_lines = np.append(self.position_lines, other.position_lines)
		self.rule_lines = np.append(self.rule_lines, other.rule_lines)
		self.initial_fill_lines = np.append(self.initial_fill_lines, other.initial_fill_lines)
		self.initial_stop_lines = np.append(self.initial_stop_lines, other.initial_stop_lines)
		self.solved_fill_lines = np.append(self.solved_fill_lines, other.solved_fill_lines)
		self.solved_stop_lines = np.append(self.solved_stop_lines, other.solved_stop_lines)
		return self

	def append_position_lines(self, position_lines: np.array):
		self.position_lines = np.append(self.position_lines, position_lines)

	def append_rule_lines(self, rule_lines: np.array):
		self.rule_lines = np.append(self.rule_lines, rule_lines)

	def append_initial_fill_lines(self, initial_fill_lines: np.array):
		self.initial_fill_lines = np.append(self.initial_fill_lines, initial_fill_lines)

	def append_initial_stop_lines(self, initial_stop_lines: np.array):
		self.initial_stop_lines = np.append(self.initial_stop_lines, initial_stop_lines)

	def append_solved_fill_lines(self, solved_fill_lines: np.array):
		self.solved_fill_lines = np.append(self.solved_fill_lines, solved_fill_lines)

	def append_solved_stop_lines(self, solved_stop_lines: np.array):
		self.solved_stop_lines = np.append(self.solved_stop_lines, solved_stop_lines)

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
	for line in scene.rule_lines:
		axs[plot_matplotlib.plot_counter].axvline(
			x=line,
			color="darkorange",
			linestyle="solid",
			linewidth=2)

	if show_initial_lines:
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

def solve_conflict(
	algorithm: str,
	lmin: float,
	dmax: float,
	c1: Couple,
	c2: Couple,
	solve_c1: bool = True,
	solve_c2: bool = True
) -> Scene:
	mid = (c1.x + c2.x) / 2
	scene: Scene = Scene()
	scene.append_position_lines(c1.x)
	scene.append_position_lines(c2.x)
	scene.append_position_lines(mid)

	scene.append_rule_lines(c1.x - c1.d / 2)
	scene.append_rule_lines(c1.x + c1.d / 2)
	scene.append_rule_lines(c2.x - c2.d / 2)
	scene.append_rule_lines(c2.x + c2.d / 2)

	if(algorithm == "A"):
		if(solve_c1):
			scene += fill_a(c1.x, mid, c1.d, c1.lmbda, dmax)
		if(solve_c2):
			scene += fill_a(c2.x, mid, c2.d, c2.lmbda, dmax)
	return scene



# Algorithm A functions
################################################################################

def findlines_a(
	fromm: float,
	to: float,
	d: float,
	lmbda: float,
	dmax: float
) -> np.array:
	if(fromm < to):
		dir_factor = 1
	else:
		dir_factor = -1

	lines: np.array = []
	n_line = 0
	last_pos = fromm + d / 2 * dir_factor
	current_d = d * lmbda
	while((dir_factor == 1 and last_pos < to)
		or (dir_factor == -1 and last_pos > to)
	):
		n_line += 1
		if(current_d > dmax):
			current_d = dmax

		current_pos = last_pos + current_d * dir_factor

		if(n_line != 0):
			lines = np.append(lines, current_pos)

		current_d = abs(current_pos - last_pos) * lmbda
		last_pos = current_pos
	return lines

def fill_a(
	fromm: float,
	to : float,
	d: float,
	lmbda: float,
	dmax: float
) -> Scene:
	"""
	Smoothness min algorithm.
	Try to find a suitable set of lines to fill the space between fromm and to
	by increasing the smoothness of resolution transition (reducing lmbda, aka
	the smoothness factor down to 1).
	"""
	step = lmbda / 1000

	lines = findlines_a(fromm, to, d, lmbda, dmax)
	n_lines = np.size(lines)
	space = abs(lines[-1] - to)

	scene = Scene()
	scene.append_initial_fill_lines(lines[0:-1])
	scene.append_initial_stop_lines(lines[-1])

	while(lmbda >= 1):
		lmbda = lmbda - step;
		newlines = findlines_a(fromm, to, d, lmbda, dmax)
		newspace = abs(newlines[-1] - to)
		if(newspace < space):
			space = newspace
			lines = newlines
		else:
			break

	scene.append_solved_fill_lines(lines[0:-1])
	scene.append_solved_stop_lines(lines[-1])
	return scene

def draw_algorithm_a(
	lmin: float,
	dmax: float,
	c1: Couple,
	c2: Couple,
	title: str = ""
):
	"""
	Draw a board with each step of the resolution of a conflict through the
	algorithm A.
	"""
	s = solve_conflict("A", lmin, dmax, c1, c2,
		solve_c1=True,
		solve_c2=True)

	fig = plt.figure(draw_algorithm_a.fig_counter, figsize=(25, 8))
	axs = fig.subplots(4, 1, sharex=True)
	fig.suptitle("Algorithm A : Increase smoothness only\n" + title, fontsize=16)

	plot_matplotlib(axs, s, show_solved_lines=False)
	plot_matplotlib(axs, s)
	plot_matplotlib(axs, s, show_initial_lines=False)
	plot_matplotlib(axs, s, show_initial_lines=False, show_stop_lines=False)

	plt.show()

	plot_matplotlib.plot_counter = 0
	draw_algorithm_a.fig_counter += 1
draw_algorithm_a.fig_counter = 1



if __name__ == "__main__":
	lmin = 2
	dmax = 0.3
	c1 = Couple(1.3, dmax / 100, 2)
	c2 = Couple(6.0, dmax / 1.2, 2)
	draw_algorithm_a(lmin, dmax, c1, c2, "Does not work if d/dmax is too near of 1")

	lmin = 2
	dmax = 2.5
	c1 = Couple(1.3, dmax / 100, 2)
	c2 = Couple(6.0, dmax / 3.3, 2)
	draw_algorithm_a(lmin, dmax, c1, c2)

	lmin = 2
	dmax = 3.5
	c1 = Couple(1.3, dmax / 2, 2)
	c2 = Couple(6.0, dmax / 2, 2)
	draw_algorithm_a(lmin, dmax, c1, c2)
