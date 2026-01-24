import os
import shutil
import subprocess
from openEMS import openEMS

def run_openemsh(fdtd: openEMS, csx_file: str, args: list[str] = ["-Gv"]):
	"""
	Invokes OpenEMSH mesher

	:param csx_file: CSX XML file path.
	"""
	fdtd.Write2XML(csx_file)
	subprocess.run(["openemsh", "-i", csx_file] + args, capture_output=False, text=True, check=True, stderr=subprocess.STDOUT)
	fdtd.ReadFromXML(csx_file)

def run_appcsxcad(fdtd: openEMS, csx_file: str, edit: bool = False, render_disc_material: bool = False):
	"""
	Invokes AppCSXCAD

	:param csx_file: CSX XML file path.
	:param edit: Edit mode, `--disableEdit`.
	:param render_disc_material: Render discrete material, `--RenderDiscMaterial`.
	"""
	fdtd.Write2XML(csx_file)
	command = ["AppCSXCAD"]
	if render_disc_material:
		command += ["--RenderDiscMaterial"]
	if not edit:
		command += ["--disableEdit"]
	command += [csx_file]
	subprocess.run(
		command,
		capture_output=False,
		text=True,
		check=False)
	if edit:
		fdtd.ReadFromXML(csx_file)

def ensure_sim_path(sim_path: str, cleanup: bool = False):
	"""
	Ensure simulation path exists, optionaly cleanup old simulation files, if any.
	Use this instead of regular openEMS.Run cleanup param, before writing CSX file.

	:param sim_path: Simulation path.
	:param cleanup: Remove old result in simulation path.
	"""
	if cleanup and os.path.exists(sim_path):
		shutil.rmtree(sim_path, ignore_errors=True)
		os.mkdir(sim_path)
	if not os.path.exists(sim_path):
		os.mkdir(sim_path)
