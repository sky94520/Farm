#!/bin/bash
#author: sky
#desc 选项 cmake clean
#desc 默认为 make
#desc 先创建build文件夹，之后执行cmake ..;如果没有错误则会执行make,之后回到本目录并执行
#eg ./mycmake.sh  make ./main
#eg ./mycmake.sh cmake 先cmake 然后make 然后执行
#eg ./mycmake.sh clean 清楚build

if [ $# == 1 ] && [ $1 == "clean" ]; then
	if [ -e build ]; then
		rm -r build
		echo "remove build dir success"
		exit 0
	fi
fi

#不存在则创建且必须转为cmake
if [ ! -e build ]; then
	mkdir build;
	$1 = "cmake"
fi

cd build
#存在一个参数并且值为cmake
if [ $# -eq 1 ] && [ $1 == "cmake" ]; then
	cmake ..
fi
#检测make是否执行完成
if [ $? ]; then
	make && cd .. && ./main || exit 1
fi
