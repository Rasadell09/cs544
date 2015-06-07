#Yunfan Li, Zhicheng Fu, Xiang Li
#CS544-001
#Final Project: Fuzzing
obj-m := fuzzing.o
KERNELDIR = /scratch/spring2015/cs444-group35/linux/

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	rm -f *~

clean:
	make -C $(KERNELDIR) M=$(PWD) clean