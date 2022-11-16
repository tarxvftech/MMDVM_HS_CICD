for each in $(ls configs/*.h); do
	echo "starting $each";
	cp $each ./Config.h
	make clean
	make hs
	board="$(basename "$each")"
	board="${board%.*}"
	mkdir -p /out/$board/
	echo $board
	cp bin/* /out/$board/
done

