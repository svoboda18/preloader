#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import argparse
import os
import subprocess
import shutil
import tempfile

def PrintError_RaiseException_StopBuild(err) :
    print("[Error] " + err)
    raise Exception("[Error] " + err)

def main():
	parser = argparse.ArgumentParser(description='Security library object parser')
	parser.add_argument('--obj_list_file', '-b',
						required=True,
						help='Provide the file path of the object')
	parser.add_argument('--preloader_root_folder', '-r',
						required=True,
						help='Provide the root folder path of preloader')
	parser.add_argument('--preloader_seclib_obj_out_dir', '-p',
						required=True,
						help='Provide the platform source folder path of preloader')
	parser.add_argument('--seclib_path', '-s',
						required=True,
						help='Provide the security library path of preloader')
	parser.add_argument('--preloader_out', '-t',
						required=True,
						help='Provide the preloader out folder path')


	args = parser.parse_args()

	if not os.path.isfile(args.obj_list_file) :
		PrintError_RaiseException_StopBuild("Seclib object list file not found: " + args.obj_list_file)
	if not os.path.isfile(args.seclib_path) :
		PrintError_RaiseException_StopBuild("Seclib file not found: " + args.seclib_path)
	if not os.path.isdir(args.preloader_root_folder) :
		PrintError_RaiseException_StopBuild("Preloader root folder not found: " + args.preloader_root_folder)
	if not os.path.isdir(args.preloader_seclib_obj_out_dir) :
		PrintError_RaiseException_StopBuild("Preloader seclib tmp out folder not found: " + args.preloader_seclib_obj_out_dir)

	lib_obj_file = open(args.obj_list_file, 'r')
	cwd = os.getcwd()
	ar_dir = tempfile.mkdtemp("_", "_", args.preloader_out)
	os.chdir(ar_dir)
	for line in lib_obj_file:
		obj_item = line.replace("\n", "")
		cmd = "ar -x " + args.seclib_path + " " + obj_item
		subprocess.call(cmd, shell=True)
		shutil.move(os.path.join(ar_dir, obj_item), os.path.join(args.preloader_seclib_obj_out_dir, obj_item))
	os.chdir(cwd)
	shutil.rmtree(ar_dir)



if __name__ == '__main__':
	main()

