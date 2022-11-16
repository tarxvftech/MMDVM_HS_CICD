mkdir -p ./out/
for each in $(ls configs/*.h); do
	echo "starting $each";
	cp $each ./Config.h
	make clean
	make hs
	board="$(basename "$each")"
	board="${board%.*}"
	echo $board
	mv bin ./out/$board
done

