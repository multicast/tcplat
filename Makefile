
test: lats latc
	#killall lats || true
	#killall latc || true

	#./lats 5678 1111 &
	#./latc 127.0.0.1 5678 1111

lats: lats.c

latc: latc.c
