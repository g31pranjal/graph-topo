import numpy as np
import random
import argparse

def main() :
	args = parse_args()
	process(args)


def parse_args() :
	parser = argparse.ArgumentParser("scripts to pre-process the data")
	parser.add_argument('inp', action="store")
	parser.add_argument('otp', action="store")
	parser.add_argument('-s', '--separator',
		help='separator between vertices in each line.', default='\t')
	parser.add_argument('--rnd', default=False, action="store_true")
	parser.add_argument('--slf', default=False, action="store_true")

	return parser.parse_args()


# def randomize(fn):

# 	permut = np.random.permutation(len(content))

# 	fno = fn + "-rnd-" + str(random.randint(1,100))
# 	fo = file(fno, 'w')

# 	fo.write(str(len(content))+"\n")
# 	for i in permut :
# 		fo.write(content[i])

# 	fo.close()


def process(args) :
	
	content = file(args.inp, 'r').readlines()
	filterd = list()

	nodes = set()

	for line in content :
		
		# remove comments in the file
		if line[0] == '#' :
			pass
		
		else :
			line = line[:-2].split(args.separator)
			a = int(line[0])
			b = int(line[1])

			# remove connections to node 0 till this is not handled in main code
			if a == 0 or b == 0 :
				continue

			# remove self loops 
			if args.slf :
				if a == b :
					continue

			nodes.add(a)
			nodes.add(b)
			filterd.append((a,b))

	del(content)
	node = sorted(list(nodes))

	permut = np.random.permutation(len(filterd))

	fout = file(args.otp, 'w')
	fout.write(str(node[0]) + '\n')
	fout.write(str(node[-1]) + '\n')
	fout.write(str(len(filterd)) + '\n')

	# permutation handling
	if not args.rnd :
		for c in filterd :
			fout.write(str(c[0]) + '\t' + str(c[1]) + '\n');
	else :
		for ind, val in enumerate(filterd) :
			fout.write(str(filterd[permut[ind]][0]) + '\t' + str(filterd[permut[ind]][1]) + '\n');

	fout.close()
		




if __name__ == "__main__" :
	main()